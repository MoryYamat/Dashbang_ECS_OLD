#include "Engine/Debug/Public/DebugDrawAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include "Engine/Graphics/Public/Types.hpp"

namespace Engine::Debug
{
	void InitAllDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& buf = ctx.ww.CreateResource<CommandBuffer>();
		buf.Clear();
		auto& config = ctx.ww.CreateResource<Config>();
	}

	void UpdateAllDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Graphics::RenderContext& rc,
		DebugLineRenderer& lineRenderer)
	{
		RenderDebugHitBoxes(ctx, rc, lineRenderer);
	}


	DebugLineRenderer::DebugLineRenderer(const std::string& vsPath,
		const std::string& fsPath)
		: shader_(vsPath, fsPath)
	{
		initGL();
	}

	DebugLineRenderer::~DebugLineRenderer()
	{
		destroyGL();
	}

	void DebugLineRenderer::initGL()
	{
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		// layout(location = 0) vec3 aPos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE,
			sizeof(DebugLineVertex),
			reinterpret_cast<void*>(offsetof(DebugLineVertex, pos))
		);

		// layout(location = 1) vec4 aColor
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1, 4, GL_FLOAT, GL_FALSE,
			sizeof(DebugLineVertex),
			reinterpret_cast<void*>(offsetof(DebugLineVertex, color))
		);

		glBindVertexArray(0);

	}

	void DebugLineRenderer::destroyGL()
	{
		if (vbo_)
		{
			glDeleteBuffers(1, &vbo_);
			vbo_ = 0;
		}
		if (vao_)
		{
			glDeleteVertexArrays(1, &vao_);
			vao_ = 0;
		}
	}

	void DebugLineRenderer::Begin(const glm::mat4& view,
		const glm::mat4& projection)
	{
		view_ = view;
		proj_ = projection;
		vertices_.clear();
	}

	void DebugLineRenderer::AddLine(const glm::vec3& a,
		const glm::vec3& b,
		const glm::vec4& color)
	{
		vertices_.push_back(DebugLineVertex{ a, color });
		vertices_.push_back(DebugLineVertex{ b, color });
	}

	void DebugLineRenderer::Flush()
	{
		if (vertices_.empty())
			return;

		shader_.Use();
		shader_.setMat4("uView", view_);
		shader_.setMat4("uProjection", proj_);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(DebugLineVertex) * vertices_.size(),
			vertices_.data(),
			GL_DYNAMIC_DRAW
		);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// 深度はそのままにしたいなら GL_DEPTH_TEST は ON のままで OK

		//GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
		//if (depthTestEnabled) glDisable(GL_DEPTH_TEST);

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices_.size()));

		//if (depthTestEnabled) glEnable(GL_DEPTH_TEST);

		glBindVertexArray(0);
	}

}

namespace
{
	using Engine::Physics::Circle2D;
	using Engine::Physics::Box2D;
	using Engine::Physics::Obb2D;
	using Engine::Debug::DebugLineRenderer;
	using Engine::Debug::DebugLineVertex;

	inline glm::vec3 To3D(const glm::vec2& xz, float y = 0.05f)
	{
		return { xz.x, y, xz.y };
	}

	void EmitCircle(DebugLineRenderer& dr,
		const Circle2D& c,
		const glm::vec4& color,
		int segments = 24)
	{
		const float twoPi = 6.28318530718f;
		const float step = twoPi / static_cast<float>(segments);

		glm::vec2 prev2 = c.center + glm::vec2(c.radius, 0.0f);

		for (int i = 1; i <= segments; ++i)
		{
			float angle = step * static_cast<float>(i);
			glm::vec2 cur2 = c.center + glm::vec2(
				c.radius * std::cos(angle),
				c.radius * std::sin(angle)
			);

			dr.AddLine(To3D(prev2), To3D(cur2), color);
			prev2 = cur2;
		}
	}

	void EmitBox(DebugLineRenderer& dr,
		const Box2D& b,
		const glm::vec4& color)
	{
		glm::vec2 c = b.center;
		glm::vec2 h = b.halfExtents;

		glm::vec2 p0 = c + glm::vec2(h.x, h.y);
		glm::vec2 p1 = c + glm::vec2(-h.x, h.y);
		glm::vec2 p2 = c + glm::vec2(-h.x, -h.y);
		glm::vec2 p3 = c + glm::vec2(h.x, -h.y);

		dr.AddLine(To3D(p0), To3D(p1), color);
		dr.AddLine(To3D(p1), To3D(p2), color);
		dr.AddLine(To3D(p2), To3D(p3), color);
		dr.AddLine(To3D(p3), To3D(p0), color);
	}

	void EmitObb(DebugLineRenderer& dr,
		const Obb2D& o,
		const glm::vec4& color)
	{
		glm::vec2 cx = o.axisX * o.halfExtents.x;
		glm::vec2 cz = o.axisZ * o.halfExtents.y;

		glm::vec2 p0 = o.center + cx + cz;
		glm::vec2 p1 = o.center - cx + cz;
		glm::vec2 p2 = o.center - cx - cz;
		glm::vec2 p3 = o.center + cx - cz;

		dr.AddLine(To3D(p0), To3D(p1), color);
		dr.AddLine(To3D(p1), To3D(p2), color);
		dr.AddLine(To3D(p2), To3D(p3), color);
		dr.AddLine(To3D(p3), To3D(p0), color);
	}
}

namespace Engine::Debug
{
	void RenderDebugHitBoxes(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Graphics::RenderContext& rc,
		DebugLineRenderer& lineRenderer)
	{
		auto& cfg = ctx.ww.GetResource<Config>();
		if (!cfg.enabled)
			return;

		auto& buf = ctx.ww.GetResource<CommandBuffer>();

		if (buf.cmds.empty())
			return;

		lineRenderer.Begin(rc.viewMatrix, rc.projectionMatrix);

		for (const auto& cmd : buf.cmds)
		{
			std::visit(
				[&](const auto& shape)
				{
					using T = std::decay_t<decltype(shape)>;

					if constexpr (std::is_same_v<T, Circle2D>)
					{
						EmitCircle(lineRenderer, shape, cmd.color);
					}
					else if constexpr (std::is_same_v<T, Box2D>)
					{
						EmitBox(lineRenderer, shape, cmd.color);
					}
					else if constexpr (std::is_same_v<T, Obb2D>)
					{
						EmitObb(lineRenderer, shape, cmd.color);
					}
					else
					{
						// std::monostate 等は無視
					}
				},
				cmd.shape
			);
		}

		lineRenderer.Flush();

		buf.Clear();
	}
}

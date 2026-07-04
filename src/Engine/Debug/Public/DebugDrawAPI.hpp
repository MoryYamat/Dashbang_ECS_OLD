#pragma once
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Physics/Public/Geometry2DTypes.hpp"

#include "Engine/Graphics/Public/Types.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <cstdint>
#include <string>


namespace Engine::Debug
{
	//
	struct Config
	{
		bool enabled = true;
	};

	struct Command
	{
		Physics::ShapeVariant shape;			// Circle2D / Box2D / Obb2D etc.
		glm::vec4 color{ 1.0f,1.0f,1.0f, 1.0f};// RGBA
	};

	struct CommandBuffer
	{
		std::vector<Command> cmds;

		void Clear() { cmds.clear(); }
		void AddCmd(Command cmd) { cmds.push_back(std::move(cmd)); }
	};

	inline void AddCircle(
		CommandBuffer& buf,
		const Engine::Physics::Circle2D& c,
		const glm::vec4& color = { 0,1,0, 1}
	)
	{
		Command cmd{};
		cmd.shape = c;
		cmd.color = color;
		buf.AddCmd(cmd);
	}

	inline void AddBox(
		CommandBuffer& buf,
		const Engine::Physics::Box2D& b,
		const glm::vec4& color = { 0,1,0,1 }
	)
	{
		Command cmd{};
		cmd.shape = b;
		cmd.color = color;
		buf.AddCmd(cmd);
	}

	inline void AddObb(
		CommandBuffer& buf,
		const Engine::Physics::Obb2D& o,
		const glm::vec4& color = { 0,1,0,1 }
	)
	{
		Command cmd{};
		cmd.shape = o;
		cmd.color = color;
		buf.AddCmd(cmd);
	}

	struct DebugDrawToggleSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx_;

		void Update()
		{
			// auto& cfg = ctx_.ww.GetResource<Engine::Debug::Config>();

			// input による Config の 切り替え
		}
	};

	struct DebugLineVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	class DebugLineRenderer
	{
	public:
		DebugLineRenderer(const std::string& vsPath, const std::string& fsPath);
		~DebugLineRenderer();

		//
		void Begin(const glm::mat4& view, const glm::mat4& projection);
		void AddLine(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color);

		void Flush();
	private:
		Engine::Graphics::Shader shader_;
		GLuint vao_ = 0;
		GLuint vbo_ = 0;

		glm::mat4 view_;
		glm::mat4 proj_;
		std::vector<DebugLineVertex> vertices_;

		void initGL();
		void destroyGL();
	};

	void RenderDebugHitBoxes(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Graphics::RenderContext& rc,
		DebugLineRenderer& lineRenderer);


	// public
	void InitAllDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateAllDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Graphics::RenderContext& rc,
		DebugLineRenderer& lineRenderer);
}
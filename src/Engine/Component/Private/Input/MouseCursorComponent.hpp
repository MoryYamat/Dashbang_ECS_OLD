#pragma once



namespace Engine::Component
{
	struct MouseCursorComponent
	{
		bool isVisible = true;
		bool isHoveringUI = false;
		float debugDrawSize = 0.02f;
	};
}
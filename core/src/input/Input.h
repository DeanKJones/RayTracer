#pragma once

#include "KeyCodes.h"
#include "GLFW/glfw3.h"
#include "functional"

#include <glm/glm.hpp>

namespace Walnut {

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode);
        static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonDown(MouseButton button);

		static glm::vec2 GetMousePosition();

		static void SetCursorMode(CursorMode mode);

        static void addMainCallback(GLFWwindow* window);
        static void addKeyPressCallback(const std::function<void(int, int)> &callback);


    private:
        static void onKeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static std::function<void(int, int)> callbackKeyPressed;
	};
}
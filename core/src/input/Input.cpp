#include "Input.h"

#include "../Application.h"

#include <GLFW/glfw3.h>

namespace Walnut {

    std::function<void(int, int)> Input::callbackKeyPressed = {};

	bool Input::IsKeyDown(KeyCode keycode)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle();
		int state = glfwGetKey(windowHandle, (int)keycode);
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle();
		int state = glfwGetMouseButton(windowHandle, (int)button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle();

		double x, y;
		glfwGetCursorPos(windowHandle, &x, &y);
		return { (float)x, (float)y };
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle();
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
	}

    void Input::addMainCallback(GLFWwindow* window)
    {
        glfwSetKeyCallback(window, Input::onKeyPressedCallback);
    }

    void Input::onKeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Input::callbackKeyPressed(key, action);
    }

    void Input::addKeyPressCallback(const std::function<void(int, int)> &callback)
    {
        Input::callbackKeyPressed = callback;
    }

}
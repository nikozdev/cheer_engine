#ifndef CH_WINDOW_H
#define CH_WINDOW_H

#include <GL_header.hpp>
#include <GL/GL_Camera.h>

#include <string>

#include <support/imgui/imgui.h>
#include <support/imgui/imgui_impl_opengl3.h>
#include <support/imgui/imgui_impl_glfw.h>

//Predefenitions
namespace GL
{
	class GL_Camera;
}

namespace GL
{
	/// Class for glfw, glew and window initialization
	/// --Handler of glfwwindow
	/// --Main window has camera instance
	/// --Initializes all relevant GL stuff
	/// --Termnates everything in destruction
	class GL_Window
	{
	private:
		//GL static functions
		static void mouse_pos_callback(GLFWwindow* window, double posX, double posY);
		static void frame_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_scroll_callback(GLFWwindow* window,
			double offsetX, double offsetY);
		static void glfw_error_callback(int error, const char* description);
		
		GLFWwindow* m_wnd = nullptr;
		GL_Camera* m_camera = nullptr;
		GL_Window* m_parrentWnd = nullptr;

		static GL_Window* currWnd;
	public:
		float SCR_WIDTH = 800, SCR_HEIGHT = 600;
		bool cursorIsDisabled = false, mouseFirst = true;
		int scrWidth = 800, scrHeight = 600,
			lastX = scrHeight / 2, lastY = scrWidth / 2;
		/// Constructor for the main window
		GL_Window(int width, int height, const std::string& title);
		/// Constructor for child windows
		GL_Window(int width, int height, const std::string& title,
			GL_Window* parrentWnd);
		~GL_Window();

		GL_Camera* getCamera() { return m_camera; };
		GLFWwindow* getWnd() const { return m_wnd; };

		static void setContext(GL_Window* wnd);
		static void closeWindow(GL_Window* wnd);
	};
}

#endif
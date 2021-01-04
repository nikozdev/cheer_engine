#include <GL\GL_Window.h>
//#include <CheerEngine.h>

namespace GL
{
	GL::GL_Window* GL::GL_Window::currWnd = nullptr;

	GL_Window::GL_Window(int width, int height, const std::string& title) :
		scrWidth(width), scrHeight(height)
	{//Initialize GLFW(window manipulations), window, GLEW(glFunctions) and camera
		if (!glfwInit())
			return;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_wnd = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!m_wnd)
		{//We can't draw anything without main window
			glfwTerminate();
			printf("GL::WINDOW::CONSTRUCTOR: GL_Window %s isn't created.\n", title.c_str());
			assert(m_wnd);
			return;
		}
		else
		{
			//Bind static functions for GLFWwindow
			glViewport(0, 0, scrWidth, scrHeight);
			glfwSetWindowSizeCallback(m_wnd, frame_size_callback);
			glfwSetErrorCallback(glfw_error_callback);
			glfwSetCursorPosCallback(m_wnd, GL_Window::mouse_pos_callback);
			glfwSetScrollCallback(m_wnd, GL_Window::mouse_scroll_callback);

			GL_Window::setContext(this);
			//glEnable functions
			{
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glLineWidth(1.0f);
			}
		}
		//Initialize GLEW(glFunctions access)
		bool err = glewInit() != GLEW_OK;
		if (err)
		{
			printf("GL_Window::CONSTRUCTOR: GLEW isn't initialized.\n");
			glfwTerminate();
			return;
		}
		glewExperimental = true;
		//Camera initialization
		m_camera = new GL_Camera(glm::vec3(0.0f, 0.0f, 0.0f));
		getCamera()->setSpeed(5.0f);
		//ImGUI initialization
		{
			ImGui::CreateContext();
			ImGui_ImplOpenGL3_Init("#version 130");
			ImGui_ImplGlfw_InitForOpenGL(m_wnd, true);
		}
		printf("C_WINDOW::CONSTRUCTOR: Window is created.\n");
	}
	
	GL_Window::GL_Window(int width, int height, const std::string& title,
		GL_Window* parrentWnd) :
		scrWidth(width), scrHeight(height), m_parrentWnd(parrentWnd)
	{//Initialize GLFW(window manipulations), window, GLEW(glFunctions) and camera
		assert(currWnd); //We don't have any window. Thus, we even don't have main window

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_wnd = glfwCreateWindow(width, height, title.c_str(), NULL, parrentWnd->getWnd());
		if (!m_wnd)
		{
			printf("GL::WINDOW::CONSTRUCTOR: GL_Window %s isn't created.\n", title.c_str());
			return;
		}
		else
		{
			//Bind static functions for GLFWwindow
			glViewport(0, 0, width, height);
			glfwSetWindowSizeCallback(m_wnd, frame_size_callback);
			glfwSetErrorCallback(glfw_error_callback);
			glfwSetCursorPosCallback(m_wnd, mouse_pos_callback);
			glfwSetScrollCallback(m_wnd, mouse_scroll_callback);

			GL_Window::setContext(this);
			//glEnable functions
			{
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
		//Camera initialization
		m_camera = new GL_Camera(glm::vec3(0.0f, 0.0f, 0.0f));
		getCamera()->setSpeed(5.0f);
		//ImGUI initialization
		{
			ImGui::CreateContext();
			ImGui_ImplOpenGL3_Init("#version 130");
			ImGui_ImplGlfw_InitForOpenGL(m_wnd, true);
		}
		printf("C_WINDOW::CONSTRUCTOR: Window is created.\n");
	}

	GL_Window::~GL_Window()
	{//Destroy ImGUI resources and free glfw
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		delete m_camera;
		closeWindow(this);
		if (!m_parrentWnd)
		{
			glfwTerminate();
		}
		printf("C_WINDOW::DESTRUCTOR: Window is destroyed.\n");
	}

	//Custom static functions
	void GL_Window::setContext(GL_Window* wnd)
	{
		currWnd = wnd;
		glfwMakeContextCurrent(wnd->getWnd());
	}
	void GL_Window::closeWindow(GL_Window* wnd)
	{
		glfwSetWindowShouldClose(wnd->getWnd(), GL_TRUE);
		glfwDestroyWindow(wnd->getWnd());
	}

	//GL static callback functions
	void GL_Window::frame_size_callback(GLFWwindow* window, int width, int height)
	{
		currWnd->scrWidth = width;
		currWnd->scrHeight = height;
		glViewport(0, 0, width, height);
	}

	void GL_Window::glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "GL::WINDOW::GLFW_ERROR_%d: %s;\n", error, description);
	}

	void GL_Window::mouse_pos_callback(GLFWwindow* window,
		double posX, double posY)
	{
		if (currWnd->mouseFirst)
		{
			posX = currWnd->lastX = static_cast<double>(currWnd->scrHeight / 2);
			posY = currWnd->lastY = static_cast<double>(currWnd->scrHeight / 2);
			currWnd->mouseFirst = false;
		}

		float offsetX = currWnd->lastX - static_cast<float>(posX);
		float offsetY = static_cast<float>(posY) - currWnd->lastY;

		currWnd->lastX = (float)posX;
		currWnd->lastY = (float)posY;

		currWnd->getCamera()->mousePosProc(offsetX, offsetY, true);
	}

	void GL_Window::mouse_scroll_callback(GLFWwindow* window,
		double offsetX, double offsetY)
	{
		currWnd->getCamera()->scrollProc((float)offsetY);
	}
}
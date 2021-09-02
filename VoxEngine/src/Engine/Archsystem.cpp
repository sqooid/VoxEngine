#include "voxpch.h"
#include "Archsystem.h"

namespace vox
{
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Archsystem* manager = reinterpret_cast<Archsystem*>(glfwGetWindowUserPointer(window));
		// Passing back to ArchSystem instance
		manager->keyCallback(key, scancode, action, mods);
	}

#ifdef DEBUG
	// Callback function for printing debug statements
	void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* msg, const void* data)
	{
		std::string _source;
		std::string _type;
		std::string _severity;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			break;

		default:
			_severity = "UNKNOWN";
			break;
		}

		std::cout << id << ": " << _type << " of " << _severity << ", raised from " << _source << ": " << msg << "\n";
	}
#endif

	Archsystem::~Archsystem()
	{
	}

	void Archsystem::initializeWindow(std::string name, int width, int height)
	{
		glfwInit();
		glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		m_Window = glfwCreateWindow(
			width,
			height,
			name.c_str(),
			NULL,
			NULL
		);
		if (!m_Window)
		{
			std::cerr << "[ERROR] Couldn't create a GLFW window\n";
			return;
		}
		glfwSetWindowUserPointer(m_Window, static_cast<void*>(this));
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);

			});
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "[ERROR] Couldn't initialize GLAD\n";
			destroyWindow();
			return;
		}

		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

		// Enabling debugging
#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, NULL);
		GLuint ids[] ={ 131185 };
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, ids, GL_FALSE);
#endif
	}

	void Archsystem::destroyWindow()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
		}
		glfwTerminate();
	}

	void Archsystem::startProgramLoop()
	{
		while (!glfwWindowShouldClose(m_Window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();

			for (auto& ptr : m_userSubs)
			{
				ptr->onUpdate();
			}

			m_Renderer->onUpdate();

			glfwSwapBuffers(m_Window);
		}
	}

	void Archsystem::keyCallback(int key, int scancode, int action, int mods)
	{
	}

	void Archsystem::initializeSubsystems()
	{
		m_Renderer = std::make_unique<Renderer>();
		m_Renderer->setParent(this);

		m_EventHandler = std::make_unique<EventHandler>();
		m_EventHandler->setParent(this);
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Archsystem* arch = static_cast<Archsystem*>(glfwGetWindowUserPointer(window));
				KeyEvent event(key, action);	// 0 - release; 1 - press; 2 - repeat
				arch->getEventHandler()->sendEvent<KeyEvent>(event);
			});
	}

	Subsystem* Archsystem::getSub(SubType type)
	{
		switch (type)
		{
		case SubType::Renderer:
			return m_Renderer.get();
		case SubType::EventHandler:
			return m_EventHandler.get();
		default:
			return nullptr;
		}
	}

	Renderer* Archsystem::getRenderer()
	{
		return m_Renderer.get();
	}

	EventHandler* Archsystem::getEventHandler()
	{
		return m_EventHandler.get();
	}

}

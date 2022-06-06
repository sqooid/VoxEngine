#pragma once

#include <GLFW/glfw3.h>
#include "Subsystem.h"
#include "Subsystems/Render/Renderer.h"
#include "Subsystems/EventHandler/EventHandler.h"
#include "Subsystems/Input/Input.h"
#include "Subsystems/Scene/Scene.h"

namespace vox
{
	class Archsystem
	{
	public:
		~Archsystem();

		void initializeWindow(std::string name, int width, int height);
		void destroyWindow();
		void startProgramLoop();

		void keyCallback(int key, int scancode, int action, int mods);

		template<class T>
		void pushSubsystem();

		void initializeSubsystems();

		Subsystem* getSub(SubType type);
		Renderer* getRenderer();
		Scene* getScene();
		EventHandler* getEventHandler();

	private:
		GLFWwindow* m_Window;

		// Native Subsystems
		std::shared_ptr<Renderer> m_Renderer;
		std::shared_ptr<Scene> m_Scene;
		std::shared_ptr<EventHandler> m_EventHandler;

		// List of user subsystems;
		std::vector<std::shared_ptr<Subsystem>> m_userSubs;

	};

	template<class T>
	void vox::Archsystem::pushSubsystem()
	{
		m_userSubs.emplace_back(std::make_shared<T>());
		m_userSubs.back()->setParent(this);
		m_userSubs.back()->onAttach();
	}
}


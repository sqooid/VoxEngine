#pragma once

namespace vox
{
	class Renderer;
	class Scene;
	class EventHandler;


	class Archsystem;

	enum class SubType
	{
		Renderer,
		EventHandler,
	};

	class Subsystem
	{
	public:
		Subsystem();
		Subsystem(Archsystem* parent);
		virtual ~Subsystem() = default;

		void setParent(Archsystem* parent);

		virtual void onAttach() {};

		virtual void onUpdate() {};

		Subsystem* getSub(SubType type);
		Renderer* getRenderer();
		Scene* getScene();
		EventHandler* getEventHandler();
	private:
		Archsystem* m_Parent;
	};
}


#pragma once
#include "tarapch.h"
#include "Tara/Input/EventListener.h"


namespace Tara {
	class Layer;
	class Entity;

	/// <summary>
	/// The Component class is designed be the root class of all components.
	/// Components are designed to hold behavioral code for Entites, attaching a component to an Entity would give it that behavior.
	/// IE, attaching a FirstPersonCameraControllerComponent to a CameraEntity would make it move as a first person camera, based on keyboard input
	/// </summary>
	class Component : public std::enable_shared_from_this<Component>, public EventListener {

		//so that Entity can call private methods.
		friend class Entity;

	public:
		/// <summary>
		/// Reference to component
		/// </summary>
		using ComponentRef = std::shared_ptr<Component>;

		/// <summary>
		/// non-owning (No) refrerence to component
		/// </summary>
		using ComponentNoRef = std::weak_ptr<Component>;

	public:
		/// <summary>
		/// Create a new component. Should not be called directly. Use static Create(...) instead.
		/// </summary>
		/// <param name="parent">The owning Entity</param>
		/// <param name="owningLayer">The owning Layer</param>
		/// <param name="name">The name</param>
		Component(std::weak_ptr<Entity> parent, const std::string& name);

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~Component() {}

		/// <summary>
		/// Create a new component.
		/// </summary>
		/// <param name="parent">the parent entity of the component</param>
		/// <param name="name">The name of the component</param>
		/// <returns></returns>
		static ComponentRef Create(std::weak_ptr<Entity> parent, const std::string& name);

		/// <summary>
		/// Register a new component (does the actual attaching)
		/// </summary>
		/// <param name="component"></param>
		static void Register(ComponentRef component);

		/// <summary>
		/// Enable/Disable Listening for native window events
		/// </summary>
		/// <param name="enable"></param>
		virtual void ListenForEvents(bool enable = true) override;
	public:
		/// <summary>
		/// Component Update function. Run every frame by parent entity
		/// </summary>
		/// <param name="deltaTime">time since last update in seconds</param>
		inline virtual void OnUpdate(float deltaTime) {}

		/// <summary>
		/// OnEvent function. 
		/// Events are forwarded by parent, unless both are listening for native window events and that event is one.
		/// </summary>
		/// <param name="e">The event</param>
		inline virtual void OnEvent(Event& e) {}

	public:
		/// <summary>
		/// Get the name of the Entity
		/// </summary>
		/// <returns>the name</returns>
		inline const std::string& GetName() const { return m_Name; }

		/// <summary>
		/// Get the parent of the Componment
		/// </summary>
		/// <returns>the parent</returns>
		inline const std::weak_ptr<Entity>& GetParent() const { return m_Parent; }

	private:

		inline void SetParent(std::weak_ptr<Entity> newParent) { m_Parent = newParent; }

	private:
		const std::string m_Name;
		std::weak_ptr<Entity> m_Parent;
	};

	/// <summary>
	/// Reference to component
	/// </summary>
	using ComponentRef = std::shared_ptr<Component>;

	/// <summary>
	/// non-owning (No) refrerence to component
	/// </summary>
	using ComponentNoRef = std::weak_ptr<Component>;
}
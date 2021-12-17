#pragma once
#include "tarapch.h"
#include "Tara/Input/EventListener.h"
#include <sol/sol.hpp>

namespace Tara {
	REFTYPE(Layer);
	NOREFTYPE(Layer);

	REFTYPE(Entity);
	NOREFTYPE(Entity);

	REFTYPE(Component);
	NOREFTYPE(Component);


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
		/// Create a new component. Should not be called directly. Use static Create(...) instead.
		/// </summary>
		/// <param name="parent">The owning Entity</param>
		/// <param name="owningLayer">The owning Layer</param>
		/// <param name="name">The name</param>
		Component(EntityNoRef parent, const std::string& name = "Component");

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~Component() {}

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
		/// A function called after this Component is added to its parent
		/// </summary>
		inline virtual void OnBeginPlay() {}

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
		inline virtual void OnEvent(Event& e) override {}

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
		inline const EntityNoRef& GetParent() const { return m_Parent; }

		/// <summary>
		/// Script version of GetParent. DO NOT CALL FROM C++
		/// </summary>
		/// <returns></returns>
		inline const EntityRef __SCRIPT__GetParent() const { return GetParent().lock(); }



	public:
		/// <summary>
		/// Register the lua type
		/// </summary>
		/// <param name="lua"></param>
		static void RegisterLuaType(sol::state& lua);

	private:

		inline void SetParent(std::weak_ptr<Entity> newParent) { m_Parent = newParent; }

	private:
		const std::string m_Name;
		EntityNoRef m_Parent;
	};

	/// <summary>
	/// Create a new component
	/// </summary>
	/// <typeparam name="ComponentType">The subclass of component to create</typeparam>
	/// <typeparam name="...VA_ARGS">Varaible arguments</typeparam>
	/// <param name="...args">the arguments to that class's constrcutor</param>
	/// <returns>A reference to that Component</returns>
	template<class ComponentType, typename... VA_ARGS>
	inline std::shared_ptr<ComponentType> CreateComponent(VA_ARGS&&... args) {
		static_assert(std::is_base_of<Component, ComponentType>::value, "Error: Tara::CreateComponent:: Provided class is not a subclass of Tara::Component");
		static_assert(std::is_constructible<ComponentType, VA_ARGS...>::value, "Error: Tara::CreateComponent:: cannot compile due to paramaters passed not matching constructor of that component type!");
		std::shared_ptr<ComponentType> entity = std::make_shared<ComponentType>(std::forward<VA_ARGS>(args)...);
		Component::Register(entity);
		entity->OnBeginPlay();
		return entity;
	}


}
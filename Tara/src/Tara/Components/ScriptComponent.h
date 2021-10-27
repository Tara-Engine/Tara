#pragma once
#include "Tara/Core/Script.h"
#include "Tara/Core/Component.h"

namespace Tara {

	REFTYPE(ScriptComponent)
	NOREFTYPE(ScriptComponent)

	/// <summary>
	/// A component with a luascript attached.
	/// </summary>
	class ScriptComponent : public Component {
	public:
		/// <summary>
		/// Construct a new ScriptComponent. Do not use directly, use Tara::CreateComponenet<Tara::ScriptComponent>(...) instead
		/// </summary>
		/// <param name="parent">The parent entity</param>
		/// <param name="path">the path to the luafile that will make up this component</param>
		/// <param name="name">the name of this component, defaults to "ScriptComponent"</param>
		ScriptComponent(EntityNoRef parent, const std::string& path, const std::string& name = "ScriptComponent");

		virtual ~ScriptComponent();

		/// <summary>
		/// Run when play begins for the component. Here, the lua file is run
		/// </summary>
		virtual void OnBeginPlay() override;

		/// <summary>
		/// Runs every frame for the component. If a callback has been registered in lua, it is called.
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void OnUpdate(float deltaTime) override;

		/// <summary>
		/// Runs for every event given to the component. If a callback has been registered in lua, it is called.
		/// </summary>
		/// <param name="e"></param>
		virtual void OnEvent(Event& e) override;

	public:
		//Functions to be called from Lua

		/// <summary>
		/// To call from lua, not C++. Called to set the OnUpdate callback function in lua. This function will then be called every update
		/// </summary>
		/// <param name="func">the function to call every update</param>
		void SetOnUpdateCallbackFunction(sol::safe_function func);

		/// <summary>
		/// To call from lua, not C++. Called to set the OnEvent callback function in lua. This function will then be called every event.
		/// </summary>
		/// <param name="func">the function to call every event</param>
		void SetOnEventCallbackFunction(sol::safe_function func);
	public:
		/// <summary>
		/// Register this lua type. Called by the State singleton
		/// </summary>
		/// <param name="lua">the lua state</param>
		static void RegisterLuaType(sol::state& lua);

		/// <summary>
		/// Make a passed WPI lua usertype access the functions from ScriptComponent. Should be called from this class and any subclasses in their RegisterLuaType functions.
		/// </summary>
		/// <typeparam name="T">the actual type being turned into a lua usertype</typeparam>
		/// <param name="type">the WIP lua usertype</param>
		template<class T> static void ExtendLuaType(sol::usertype<T>& type);

	private:
		const std::string m_Path;
		sol::safe_function m_OnUpdateCallbackFunction;
		sol::safe_function m_OnEventCallbackFunction;
	};



	template<class T>
	inline void ScriptComponent::ExtendLuaType(sol::usertype<T>& type)
	{
		static_assert(std::is_base_of<ScriptComponent, T>::value, "Error: Tara::ScriptComponent::ExtendLuaType:: Provided class is not a subclass of Tara::ScriptComponent");
		//Component::ExtendLuaType<ScriptComponent>(type);
		type["SetOnUpdateCallbackFunction"] = &ScriptComponent::SetOnUpdateCallbackFunction;
		type["SetOnEventCallbackFunction"] = &ScriptComponent::SetOnEventCallbackFunction;
	}

}
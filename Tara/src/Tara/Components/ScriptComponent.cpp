#include "tarapch.h"
#include "ScriptComponent.h"

namespace Tara{

	ScriptComponent::ScriptComponent(EntityNoRef parent, const std::string& path, const std::string& name)
		: Component(parent, name), m_Path(path), m_OnUpdateCallbackFunction(), m_OnEventCallbackFunction()
	{
	}

	ScriptComponent::~ScriptComponent()
	{
		m_OnUpdateCallbackFunction.abandon(); //for some reason, prevents a crash on close.
		m_OnEventCallbackFunction.abandon();
		LOG_S(INFO) << "ScriptComponent Destroyed!";
	}

	void ScriptComponent::OnBeginPlay()
	{
		
		//run the script as a file
		Script::Get()->GetState()["CurrentComponent"] = std::dynamic_pointer_cast<ScriptComponent>(shared_from_this());
		auto results = Script::Get()->GetState().safe_script_file(m_Path);
		if (!results.valid()) {
			sol::error err = results;
			LOG_S(ERROR) << "Error in Lua Script: " << err.what();
		}
		//Script::Get()->GetState()["CurrentComponent"] = nullptr;
	}

	void ScriptComponent::OnUpdate(float deltaTime)
	{
		if (m_OnUpdateCallbackFunction.valid()) {
			Script::Get()->GetState()["CurrentComponent"] = std::dynamic_pointer_cast<ScriptComponent>(shared_from_this());
			auto results = m_OnUpdateCallbackFunction(deltaTime);
			if (!results.valid()) {
				sol::error err = results;
				LOG_S(ERROR) << "Error in Lua Script [OnUpdate]: " << err.what();
			}
		}
	}

	void ScriptComponent::OnEvent(Event& e)
	{
		if (m_OnEventCallbackFunction.valid()) {
			Script::Get()->GetState()["CurrentComponent"] = std::dynamic_pointer_cast<ScriptComponent>(shared_from_this());
			auto results = m_OnEventCallbackFunction(Event::GetScriptEvent(e));
			if (!results.valid()) {
				sol::error err = results;
				LOG_S(ERROR) << "Error in Lua Script [OnEvent]: " << err.what();
			}
		}
	}

	void ScriptComponent::SetOnUpdateCallbackFunction(sol::safe_function func)
	{
		m_OnUpdateCallbackFunction = func;
	}

	void ScriptComponent::SetOnEventCallbackFunction(sol::safe_function func)
	{
		m_OnEventCallbackFunction = func;
	}

	void ScriptComponent::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<ScriptComponent> type = lua.new_usertype<ScriptComponent>("ScriptComponent"); //no constructors. for now.
		Component::ExtendLuaType<ScriptComponent>(type);
		ScriptComponent::ExtendLuaType<ScriptComponent>(type);
	}



}

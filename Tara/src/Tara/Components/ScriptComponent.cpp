#include "tarapch.h"
#include "ScriptComponent.h"
#include "Tara/Core/Entity.h"

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
		//Script::Get()->GetState()["CurrentComponent"] = shared_from_this();
		

		//auto comp = std::make_shared<int>(1);
		//sol::usertype<int> t = Script::Get()->GetState().new_usertype<int>("inttype");
		//t["is"] = &is_check<int>;
		
		//Script::Get()->GetState()["CurrentComponent"] = comp;
		
		
		auto results = Script::Get()->GetState().safe_script_file(m_Path);
		if (!results.valid()) {
			sol::error err = results;
			LOG_S(ERROR) << "Error in Lua Script: " << err.what();
			LOG_S(ERROR) << loguru::stacktrace().c_str();
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
			//LOG_S(INFO) << "Calling Script event function!\n" << loguru::stacktrace(0).c_str();
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
		sol::usertype<ScriptComponent> type = lua.new_usertype<ScriptComponent>("ScriptComponent", sol::base_classes, sol::bases<Component>()); //MUST manually list all base classes!		
		CONNECT_METHOD(ScriptComponent, SetOnUpdateCallbackFunction);
		CONNECT_METHOD(ScriptComponent, SetOnEventCallbackFunction);
	}



}

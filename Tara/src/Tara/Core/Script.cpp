#include "tarapch.h"
#include "Script.h"

#include "Tara/Core/Component.h"
#include "Tara/Components/ScriptComponent.h"
#include "Tara/Core/Entity.h"
#include "Tara/Entities/CameraEntity.h"
#include "Tara/Entities/SpriteEntity.h"

namespace Tara{
	Script::Script()
		: m_LuaState(), m_DefaultLibPath("lua")
	{}

	Script* Script::Get()
	{
		static Script script;
		return &script;
	}

	void Script::Init()
	{
		//load lua libraries
		m_LuaState.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::debug, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::utf8);
		
		//Load libraries
		ReloadDefaultLibrary();

		//load engine default types
		RegisterType<Component>("Tara::Component");
		RegisterType<ScriptComponent>("Tara::ScriptComponent");
		RegisterType<Entity>("Tara::Entity");
		RegisterType<CameraEntity>("Tara::CameraEntity");
	}

	void Script::ReloadDefaultLibrary()
	{
		m_LuaState["LibraryPath"] = m_DefaultLibPath;
		auto results = m_LuaState.safe_script_file(m_DefaultLibPath + "/init.lua");
		if (!results.valid()) {
			sol::error err = results;
			LOG_S(ERROR) << "Error loading default library: \n" << err.what() << R"(
				This error will likely cause most lua scripts to also be in error. 
				To fix, set the default library path using Tara::Script::Get->SetDefaultLibraryPath(...). 
				If this is done after Tara::Application::Get->Init(...), then call Tara::Script::Get->ReloadDefaultLibrary()
			)" << "Current Path: \"" << m_DefaultLibPath << "\"";
		}
	}


}
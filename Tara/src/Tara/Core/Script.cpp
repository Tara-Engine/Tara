#include "tarapch.h"
#include "Script.h"

#include "Tara/Core/Component.h"
#include "Tara/Components/ScriptComponent.h"

namespace Tara{
	Script::Script()
		: lua()
	{}

	Script* Script::Get()
	{
		static Script script;
		return &script;
	}

	void Script::Init()
	{
		//load lua libraries
		lua.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::debug, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::utf8);

		//load engine default types
		RegisterType<Component>("Component");
		RegisterType<ScriptComponent>("ScriptComponent");
	}


}

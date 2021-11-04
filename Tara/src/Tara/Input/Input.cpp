#include "tarapch.h"
#include "Input.h"
#include "Tara/Core/Script.h"

namespace Tara{

	void Input::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<Input> type = lua.new_usertype<Input>("Input");
		CONNECT_STATIC(Input, Get);
		CONNECT_METHOD(Input, IsKeyPressed);
		CONNECT_METHOD(Input, IsMouseDown);
		CONNECT_METHOD(Input, GetMouseX);
		CONNECT_METHOD(Input, GetMouseY);
		CONNECT_METHOD(Input, GetPrevMouseX);
		CONNECT_METHOD(Input, GetPrevMouseY);
		CONNECT_METHOD(Input, GetMouseDeltaX);
		CONNECT_METHOD(Input, GetMouseDeltaY);
		CONNECT_METHOD_OVERRIDE(Input, GetMousePos);
		CONNECT_METHOD_OVERRIDE(Input, GetPrevMousePos);
		CONNECT_METHOD_OVERRIDE(Input, GetMouseDelta);
	}


	sol::table Input::__SCRIPT__GetMousePos()
	{
		auto table = NEW_TABLE;
		auto pos = GetMousePos();
		table["x"] = pos.x;
		table["y"] = pos.y;
		return table;
	}
	sol::table Input::__SCRIPT__GetPrevMousePos()
	{
		auto table = NEW_TABLE;
		auto pos = GetPrevMousePos();
		table["x"] = pos.x;
		table["y"] = pos.y;
		return table;
	}
	sol::table Input::__SCRIPT__GetMouseDelta()
	{
		auto table = NEW_TABLE;
		auto pos = GetMouseDelta();
		table["x"] = pos.x;
		table["y"] = pos.y;
		return table;
	}
}
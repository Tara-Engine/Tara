#include "tarapch.h"
#include "Tara/Core/Script.h"
#include "Tara/Input/Event.h"
#include "Tara/Input/ApplicationEvents.h"



const static std::string eventCategoryNames[] = {
	"EventCategoryNative",
	"EventCategoryApplication",
	"EventCategoryInput",
	"EventCategoryKeyboard",
	"EventCategoryMouse",
	"EventCategoryMouseButton",
	"EventCategoryOverlap",
	"EventCategoryHirarchy"
};

namespace Tara{

	sol::table Event::GetScriptEvent(Event& e)
	{
		auto table = sol::table(Script::Get()->GetState(), sol::create);

		//do the basics that don't need special information
		table["Type"] = e.GetName();
		table["String"] = e.ToString();
		table["Categories"] = sol::table(Script::Get()->GetState(), sol::create);

		uint32_t catFlags = e.GetCategoryFlags();
		uint32_t counter = 1;
		for (int i = 0; i < 8; i++) {
			if (catFlags & BIT(i)) {
				table["Categories"][counter++] = eventCategoryNames[i];
			}
		}
		if (counter == 1) {
			table["Categories"][counter] = "None";
		}

		//use event filters and lambdas to get the particulars
		EventFilter filter(e);

		filter.Call<WindowResizeEvent>([&table](WindowResizeEvent& ee) {
			table["Width"] = (int)ee.GetWidth();
			table["Height"] = (int)ee.GetHeight();
			return false;
			});

		filter.Call<KeyPressEvent>([&table](KeyPressEvent& ee) {
			table["Key"] = (int)ee.GetKey();
			table["Mods"] = (int)ee.GetMods();
			table["RepeatCount"] = ee.GetRepeatCount();
			return false;
			});

		filter.Call<KeyReleaseEvent>([&table](KeyReleaseEvent& ee) {
			table["Key"] = (int)ee.GetKey();
			table["Mods"] = (int)ee.GetMods();
			return false;
			});

		filter.Call<KeyTypeEvent>([&table](KeyTypeEvent& ee) {
			table["Key"] = (int)ee.GetKey();
			table["Mods"] = (int)ee.GetMods();
			char c[2] = { (char)ee.GetKey(), '\0' };
			table["Char"] = std::string(c);
			return false;
			});

		filter.Call<MouseButtonPressEvent>([&table](MouseButtonPressEvent& ee) {
			table["Button"] = (int)ee.GetButton();
			return false;
			});

		filter.Call<MouseButtonReleaseEvent>([&table](MouseButtonReleaseEvent& ee) {
			table["Button"] = (int)ee.GetButton();
			return false;
		});

		filter.Call<MouseMoveEvent>([&table](MouseMoveEvent& ee) {
			table["X"] = ee.GetXPos();
			table["Y"] = ee.GetYPos();
			return false;
			});

		filter.Call<MouseScrollEvent>([&table](MouseScrollEvent& ee) {
			table["X"] = ee.GetXOffset();
			table["Y"] = ee.GetYOffset();
			return false;
			});

		//Application Events

		filter.Call<OverlapEvent>([&table](OverlapEvent& ee) {
			table["Self"] = ee.GetSelf();
			table["Other"] = ee.GetOther();
			table["Penetration"] = ee.GetPenetration();
			table["Normal"] = ee.GetNormal(); //TODO: use some sort of vector conversion!
			return false;
			});

		filter.Call<ChildAddedEvent>([&table](ChildAddedEvent& ee) {
			table["Parent"] = ee.GetParent();
			table["Child"] = ee.GetChild();
			return false;
			});

		filter.Call<ChildRemovedEvent>([&table](ChildRemovedEvent& ee) {
			table["Parent"] = ee.GetParent();
			table["Child"] = ee.GetChild();
			return false;
			});

		filter.Call<ParentSwapedEvent>([&table](ParentSwapedEvent& ee) {
			table["OldParent"] = ee.GetOldParent();
			table["NewParent"] = ee.GetNewParent();
			return false;
			});

		filter.Call<ComponentAddedEvent>([&table](ComponentAddedEvent& ee) {
			table["Entity"] = ee.GetEntity();
			table["Component"] = ee.GetComponent();
			return false;
			});

		filter.Call<ComponentRemovedEvent>([&table](ComponentRemovedEvent& ee) {
			table["Entity"] = ee.GetEntity();
			table["Component"] = ee.GetComponent();
			return false;
			});
		

		//return
		return table;
	}

}
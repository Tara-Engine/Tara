#include "tarapch.h"
#include "Component.h"
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Core/Script.h"

namespace Tara{
	Component::Component(EntityNoRef parent, const std::string& name)
		: m_Name(name), m_Parent(parent)
	{}

	void Component::Register(ComponentRef component){
		component->GetParent().lock()->AddComponent(component);
	}

	void Component::ListenForEvents(bool enable){
		m_Parent.lock()->GetOwningLayer().lock()->EnableListener(weak_from_this(), enable);
	}

	void Component::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<Component> type = lua.new_usertype<Component>("Component"); //no constructors. for now.
		CONNECT_FUNCTION(Component, ListenForEvents);
		CONNECT_FUNCTION(Component, GetName);
		CONNECT_FUNCTION(Component, GetListeningForEvents);
		CONNECT_FUNCTION_OVERRIDE(Component, GetParent);
	}

}

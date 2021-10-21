#include "tarapch.h"
#include "Component.h"
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
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

}

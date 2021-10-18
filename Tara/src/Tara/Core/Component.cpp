#include "tarapch.h"
#include "Component.h"
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
namespace Tara{
	Component::Component(std::weak_ptr<Entity> parent, const std::string& name)
		: m_Name(name), m_Parent(parent)
	{}

	ComponentRef Component::Create(std::weak_ptr<Entity> parent, const std::string& name){
		std::shared_ptr<Component> component = std::make_shared<Component>(parent, name);
		Register(component);
		return component;
	}

	void Component::Register(ComponentRef component){
		component->GetParent().lock()->AddComponent(component);
	}

	void Component::ListenForEvents(bool enable){
		m_Parent.lock()->GetOwningLayer().lock()->EnableListener(weak_from_this(), enable);
	}

}

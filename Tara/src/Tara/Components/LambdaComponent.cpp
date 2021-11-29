#include "tarapch.h"
#include "LambdaComponent.h"

namespace Tara{
	LambdaComponent::LambdaComponent(
		EntityNoRef parent, 
		std::function<void(LambdaComponent*)> beginPlayCallback, 
		std::function<void(LambdaComponent*, float)> onUpdateCallback, 
		std::function<void(LambdaComponent*, Event&)> onEventCallback, 
		const std::string& name
	)
		:Component(parent, name),
		m_BeginPlayCallback(beginPlayCallback),
		m_OnUpdateCallback(onUpdateCallback),
		m_OnEventCallback(onEventCallback)
	{}

	void LambdaComponent::OnBeginPlay()
	{
		m_BeginPlayCallback(this);
	}

	void LambdaComponent::OnUpdate(float deltaTime)
	{
		m_OnUpdateCallback(this, deltaTime);
	}

	void LambdaComponent::OnEvent(Event& e)
	{
		m_OnEventCallback(this, e);
	}


}

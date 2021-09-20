#include "tarapch.h"
#include "Layer.h"

namespace Tara{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::Activate()
	{
	}

	void Layer::Deactivate()
	{
	}

	void Layer::Update(float deltaTime)
	{
		for (auto entity : m_Entities) {
			entity->Update(deltaTime);
		}
	}

	void Layer::Draw(float deltaTime)
	{
		for (auto entity : m_Entities) {
			entity->Draw(deltaTime);
		}
	}

	void Layer::OnEvent(Event& e)
	{
		std::list<EventListenerNoRef> removable;
		for (auto listener : m_Listeners) {
			auto lockedListener = listener.lock();
			if (lockedListener) {
				//valid non-null listener
				lockedListener->OnEvent(e);
			}
			else {
				//add to removable list (don't want to remove now b/c of possible issues)
				removable.push_back(listener);
			}
		}
		//remove any that are no longer valid
		for (auto listener : removable) {
			m_Listeners.remove(listener);
		}
	}

	bool Layer::AddEntity(EntityRef ref)
	{
		if (!IsEntityRoot(ref)) {
			m_Entities.push_back(ref);
			return true;
		}
		return false;
	}

	bool Layer::RemoveEntity(EntityRef ref)
	{
		if (!IsEntityRoot(ref)) {
			return false;
		}
		m_Entities.remove(ref);
		return true;
	}

	bool Layer::IsEntityRoot(EntityRef ref)
	{
		return std::find(m_Entities.begin(), m_Entities.end(), ref) != m_Entities.end();
	}

	bool Layer::EnableListener(EventListenerNoRef ref, bool enable)
	{
		if (enable){
			if (std::find(m_Listeners.begin(), m_Listeners.end(), ref) == m_Listeners.end()) {
				return true;
			}
			m_Listeners.push_back(ref);
		}
		else {
			m_Listeners.remove(ref);
		}
		return true;
	}

}

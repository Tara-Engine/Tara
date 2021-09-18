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

}

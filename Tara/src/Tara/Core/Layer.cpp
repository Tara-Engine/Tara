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
		//LOG_S(INFO) << "Layer OnEvent called!";
		std::list<EventListenerNoRef> removable;
		for (auto listener : m_Listeners) {
			//LOG_S(INFO) << "Event listener OnEvent called!";
			auto lockedListener = listener.lock();
			if (lockedListener) {
				//valid non-null listener
				lockedListener->ReceiveEvent(e);
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
				m_Listeners.push_back(ref);
				//LOG_S(INFO) << "Adding Event Listener!";
				return true;
			}
		}
		else {
			m_Listeners.remove(ref);
		}
		return true;
	}


	void Layer::RunOverlapChecks()
	{
		//clear manifolds
		m_FrameManifoldQueue.clear();

		// Set up queue
		std::list<std::pair<EntityRef, EntityRef>> overlapQueue;

		//for all entities, check their own children
		for (auto iter1 = m_Entities.begin(); iter1 != m_Entities.end(); iter1++) {
			EntityRef entity1 = *iter1;
			entity1->SelfOverlapChecks();
			
			//run all root x root, full AABB overlap check
			auto iter2 = iter1;
			iter2++;
			for (; iter2 != m_Entities.end(); iter2++) {
				EntityRef entity2 = *iter2;
				if (entity1->GetFullBoundingBox().Overlaping(entity2->GetFullBoundingBox())) {
					//for all that have overlaps, queue up
					overlapQueue.push_back(std::make_pair(entity1, entity2 ));
				}
			}
		}

		//when done, run OtherOverlapChecks on them
		for (auto pair : overlapQueue) {
			pair.first->OtherOverlapChecks(pair.second);
		}

		//Now, deal with all the manifolds that have been made
		for (Manifold m : m_FrameManifoldQueue) {
			m.Resolve();
		}
	}


	std::list<EntityRef> Layer::GetAllEntitiesInBox(const BoundingBox& box)
	{
		std::list<EntityRef> Overlaps;
		for (auto entity : m_Entities) {
			if (box.Overlaping(entity->GetFullBoundingBox())) {
				entity->GetAllChildrenInBox(box, Overlaps);
			}
		}
		return Overlaps;
	}

	std::list<EntityRef> Layer::GetAllEntitiesInRadius(Vector origin, float radius)
	{
		std::list<EntityRef> Overlaps;
		for (auto entity : m_Entities) {
			if (entity->GetFullBoundingBox().OverlappingSphere(origin, radius)) {
				entity->GetAllChildrenInRadius(origin, radius, Overlaps);
			}
		}
		return Overlaps;
	}

	

}

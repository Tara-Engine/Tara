#include "tarapch.h"
#include "Layer.h"
#include "Tara/Renderer/Renderer.h"

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
			if (entity) {
				entity->Update(deltaTime);
			}
		}
		uint32_t cleanCount = 0;
		for (auto it = m_DestroyedEntities.begin(); it != m_DestroyedEntities.end();) {
			if (!it->lock()){
				cleanCount++;
				m_DestroyedEntities.erase(it++);
			}
			else {
				it++;
			}
		}
		if (cleanCount > 0) {
			LOG_S(INFO) << "Entities Cleaned since last frame: " << cleanCount;
		}
	}

	void Layer::Draw(float deltaTime)
	{
		for (auto& lightRef : m_LightQueue) {
			Tara::Renderer::BeginModelDepthScene(lightRef);
			uint32_t cameraBits = ~0;
			if (m_LayerCamera) {
				cameraBits = m_LayerCamera->GetCamera()->GetRenderFilterBits();
			}
			for (auto entity : GetEntityList()) {
				if (entity) {
					entity->Draw(deltaTime, cameraBits);
				}
			}
			Tara::Renderer::EndScene();
		}

		//first, draw all lights
		for (auto& cameranoref : m_CameraQueue) {
			auto camera = cameranoref.lock();
			if (camera) {
				Tara::Renderer::BeginScene(camera->GetCamera());
				uint32_t cameraBits = camera->GetCamera()->GetRenderFilterBits();
				for (auto entity : m_Entities) {
					if (entity) {
						entity->Draw(deltaTime, cameraBits);
					}
				}

				Tara::Renderer::EndScene();
			}
		}
		m_LightQueue.clear();
		m_CameraQueue.clear();
	}

	void Layer::OnEvent(Event& e)
	{
		//LOG_S(INFO) << "Layer OnEvent called!";
		m_InEventHandler = true;
		std::list<EventListenerNoRef> removable;
		for (auto& listener : m_Listeners) {
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
			if (e.Handled()) {
				break;
			}
		}
		//remove any that are no longer valid
		for (auto listener : removable) {
			m_Listeners.remove(listener);
		}
		m_InEventHandler = false;

		//handle any enqued listener changes
		for (auto ls : m_ListenerQueue) {
			EnableListener(ls.first, ls.second);
		}
		m_ListenerQueue.clear();
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

	bool Layer::MoveEntityDown(EntityRef ref, bool toBottom)
	{
		auto f = std::find(m_Entities.begin(), m_Entities.end(), ref);
		if (f == m_Entities.end()) {
			//not a child
			return false;
		}
		if (f == m_Entities.begin()) {
			//it is already top
			return true;
		}
		if (toBottom) {
			//remove and re-insert at top
			//erase is used so not to search the list again
			m_Entities.erase(f);
			m_Entities.push_front(ref);
		}
		else {
			std::iter_swap(std::prev(f), f);
		}
		return true;
	}
	
	bool Layer::MoveEntityUp(EntityRef ref, bool toTop)
	{
		auto f = std::find(m_Entities.begin(), m_Entities.end(), ref);
		if (f == m_Entities.end()) {
			//not a child
			return false;
		}
		if (f == std::prev(m_Entities.end())) {
			//it is already bottom
			return true;
		}
		if (toTop) {
			//remove and re-insert at top
			//erase is used so not to search the list again
			m_Entities.erase(f);
			m_Entities.push_back(ref);
		}
		else {
			std::iter_swap(f, std::next(f));
		}
		return true;
	}

	bool Layer::EnableListener(EventListenerNoRef ref, bool enable)
	{
		if (m_InEventHandler) {
			//enqueue, as running now will break Listener list
			m_ListenerQueue.push_back(std::make_pair(ref, enable));
			return true;
		}
		if (enable){
			//if the element is already in the list, move to front
			auto f = std::find(m_Listeners.begin(), m_Listeners.end(), ref);
			if (f != m_Listeners.end()) {
				if (f == m_Listeners.begin()) {
					return true;
				}
				m_Listeners.remove(ref);
			}
			m_Listeners.push_front(ref); //listeners are updated in REVERSE order
			return true;
		}
		else {
			m_Listeners.remove(ref);
		}
		return true;
	}


	void Layer::MarkDestroyed(EntityNoRef ref)
	{
		m_DestroyedEntities.push_back(ref);
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

	void Layer::EnqueLight(LightBaseRef light)
	{
		if (light->ShouldDrawDepth()) {
			m_LightQueue.insert(light);
		}
	}

	

}

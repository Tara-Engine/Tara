#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"
#include "Tara/Core/Entity.h"
#include "Tara/Input/EventListener.h"
#include "Tara/Input/Manifold.h"

namespace Tara {

	struct Manifold;

	/// <summary>
	/// Layer Class. Layers are "Draw slices" of a scene. 
	/// This class should be subclassed to make a custom application, and 
	/// a instance of that subclass pushed to a scene.
	/// </summary>
	class Layer : public std::enable_shared_from_this<Layer> {

		friend class Entity;

	public:
		/// <summary>
		/// Layer Constructor
		/// </summary>
		Layer(); 
		virtual ~Layer();

		/// <summary>
		/// Soft Activation of a Layer.
		/// Automatically called when added to a scene
		/// </summary>
		virtual void Activate();
		/// <summary>
		/// Soft Deactivation of a Layer.
		/// </summary>
		virtual void Deactivate();

		/// <summary>
		/// Update function for a layer
		/// </summary>
		virtual void Update(float deltaTime);
		/// <summary>
		/// Draw function for a Layer
		/// </summary>
		virtual void Draw(float deltaTime);
		/// <summary>
		/// Event function for a Layer
		/// Used to pass events to the layer.
		/// </summary>
		/// <param name="e">the event</param>
		virtual void OnEvent(Event& e);

		

		/// <summary>
		/// Add an entity to the layer as a Root entity
		/// </summary>
		/// <param name="ref">the entity to add</param>
		/// <returns>false if failed, true otherwise</returns>
		bool AddEntity(EntityRef ref);

		/// <summary>
		/// Remove an entity form the layer, if it is a root entity
		/// </summary>
		/// <param name="ref">the entity to remove</param>
		/// <returns>false if failed, true otherwise</returns>
		bool RemoveEntity(EntityRef ref);

		/// <summary>
		/// check if a particular entity is root
		/// </summary>
		/// <param name="ref">the entity to check for</param>
		/// <returns>true if present, false otherwise</returns>
		bool IsEntityRoot(EntityRef ref);

		/// <summary>
		/// Enable/Disable a specific reference as a listener
		/// </summary>
		/// <param name="ref">the reference</param>
		/// <param name="enable">if it should be enabled</param>
		/// <returns>true if successful</returns>
		bool EnableListener(EventListenerNoRef ref, bool enable = true);

		/// <summary>
		/// Run the overlap checks for this layer.
		/// Do not call manually, it is called by Scene
		/// </summary>
		void RunOverlapChecks();

		/// <summary>
		/// Get a list of all the entities that overlap a bounding box
		/// </summary>
		/// <param name="box">The box to check with</param>
		/// <returns>a list of all entities</returns>
		std::list<EntityRef> GetAllEntitiesInBox(const BoundingBox& box);

		/// <summary>
		/// Get a list of all the entities that overlap a circle/sphere
		/// </summary>
		/// <param name="origin">the position</param>
		/// <param name="radius"></param>
		/// <returns></returns>
		std::list<EntityRef> GetAllEntitiesInRadius(Vector origin, float radius);
		

		/// <summary>
		/// Get a list of all the entities that overlap a bounding box that are a specific templated subclass of Entity
		/// </summary>
		/// <typeparam name="T">some subclass of Entity</typeparam>
		/// <param name="box">The box to check with</param>
		/// <returns>a list of all entities</returns>
		template<class T>
		std::list<std::shared_ptr<T>> GetAllEntitiesOfClassInBox(const BoundingBox& box);

		/// <summary>
		/// Get a list of all the entities that overlap a circle/sphere that are a specific templated subclass of Entity
		/// </summary>
		/// <typeparam name="T">some subclass of Entity</typeparam>
		/// <param name="origin">the position</param>
		/// <param name="radius">the distance</param>
		/// <returns></returns>
		template<class T> 
		std::list<std::shared_ptr<T>> GetAllEntitiesOfClassInRadius(Vector origin, float radius);


	protected:
		/// <summary>
		/// Get a const ref to the list of entities that are root in this layer
		/// </summary>
		/// <returns>const ref to list of entities that are root in this layer</returns>
		const std::list<EntityRef>& GetEntityList() const { return m_Entities; }


	private:
		inline void AddManifoldToQueue(Manifold&& m) {m_FrameManifoldQueue.push_back(m);}

	private:

		std::list<EntityRef> m_Entities;

		std::list<EventListenerNoRef> m_Listeners;

		std::list<Manifold> m_FrameManifoldQueue;
	};

	/// <summary>
	/// Reference to the Layer Class. Layers are "Draw slices" of a scene. 
	/// This class should be subclassed to make a custom application, and 
	/// a instance of that subclass pushed to a scene.
	/// </summary>
	using LayerRef = std::shared_ptr<Layer>;
	
	/// <summary>
	/// Non-Owning Reference to the Layer Class. Layers are "Draw slices" of a scene. 
	/// This class should be subclassed to make a custom application, and 
	/// a instance of that subclass pushed to a scene.
	/// </summary>
	using LayerNoRef = std::weak_ptr<Layer>;





	/****************************************************************
	 *                     Implementations                          *
	 ****************************************************************/


	template<class T>
	inline std::list<std::shared_ptr<T>> Layer::GetAllEntitiesOfClassInBox(const BoundingBox& box)
	{
		static_assert(std::is_base_of<Entity, T>::value, "Provided class is not a subclass of Tara::Entity");

		std::list<std::shared_ptr<T>> Overlaps;
		std::list<EntityRef> PotentialOverlaps;

		//initial state of queue
		for (auto entity : m_Entities) {
			if (box.Overlaping(entity->GetFullBoundingBox())) {
				PotentialOverlaps.push_back(entity);
			}
		}

		while (!PotentialOverlaps.empty()) {
			EntityRef entity = PotentialOverlaps.front();
			PotentialOverlaps.pop_front();

			//if the specific box overlaps, then add
			if (box.Overlaping(entity->GetSpecificBoundingBox())) {
				std::shared_ptr<T> typedEntity = std::dynamic_pointer_cast<T>(entity);
				if (typedEntity) {
					Overlaps.push_back(typedEntity);
				}
			}

			//enqueue children if they overlap
			for (auto child : entity->GetChildren()) {
				if (box.Overlaping(child->GetFullBoundingBox())) {
					PotentialOverlaps.push_back(child);
				}
			}
		}

		return Overlaps;
	}

	template<class T>
	inline std::list<std::shared_ptr<T>> Layer::GetAllEntitiesOfClassInRadius(Vector origin, float radius)
	{
		static_assert(std::is_base_of<Entity, T>::value, "Provided class is not a subclass of Tara::Entity");

		std::list<std::shared_ptr<T>> Overlaps;
		std::list<EntityRef> PotentialOverlaps;

		//initial state of queue
		for (auto entity : m_Entities) {
			if (entity->GetFullBoundingBox().OverlappingSphere(origin, radius)) {
				PotentialOverlaps.push_back(entity);
			}
		}

		while (!PotentialOverlaps.empty()) {
			EntityRef entity = PotentialOverlaps.front();
			PotentialOverlaps.pop_front();

			//if the specific box overlaps, then add
			if (entity->GetSpecificBoundingBox().OverlappingSphere(origin, radius)) {
				std::shared_ptr<T> typedEntity = std::dynamic_pointer_cast<T>(entity);
				if (typedEntity) {
					Overlaps.push_back(typedEntity);
				}
			}

			//enqueue children if they overlap
			for (auto child : entity->GetChildren()) {
				if (child->GetFullBoundingBox().OverlappingSphere(origin, radius)) {
					PotentialOverlaps.push_back(child);
				}
			}
		}

		return Overlaps;
	}


}
#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"
#include "Tara/Core/Entity.h"
#include "Tara/Input/EventListener.h"
#include "Tara/Input/Manifold.h"
#include "Tara/Entities/CameraEntity.h"

namespace Tara {

	struct Manifold;

	REFTYPE(Layer)

	/// <summary>
	/// Layer Class. Layers are "Draw slices" of a scene. 
	/// This class should be subclassed to make a custom application, and 
	/// a instance of that subclass pushed to a scene.
	/// </summary>
	class Layer : public std::enable_shared_from_this<Layer> {

		friend class Entity;
		friend class Scene;
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
		/// Move an entity up by one or to top in the entity list. Check if the entity is root.
		/// </summary>
		/// <param name="ref">the entity to move</param>
		/// <param name="toTop">if the move should be to the top or normal. defaults to false (normal)</param>
		/// <returns>true if the operations was successful</returns>
		bool MoveEntityUp(EntityRef ref, bool toTop = false);

		/// <summary>
		/// Move an entity up by one or to top in the entity list. Check if the entity is root.
		/// </summary>
		/// <param name="ref">the entity to move</param>
		/// <param name="toTop">if the move should be to the top or normal. defaults to false (normal)</param>
		/// <returns>true if the operations was successful</returns>
		bool MoveEntityDown(EntityRef ref, bool toBottom = false);

		/// <summary>
		/// Enable/Disable a specific reference as a listener
		/// </summary>
		/// <param name="ref">the reference</param>
		/// <param name="enable">if it should be enabled</param>
		/// <returns>true if successful</returns>
		bool EnableListener(EventListenerNoRef ref, bool enable = true);

		/// <summary>
		/// Mark an entity destroyed. This does not destroy it, but just loggs it for destrution when all strong refs are gone.
		/// No strong refs should remain in tree. Random things that have a strong ref should be eliminated if possible.
		/// </summary>
		/// <param name="ref"></param>
		void MarkDestroyed(EntityNoRef ref);

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

		/// <summary>
		/// Set the layer camera. Default camera used for drawing.
		/// </summary>
		/// <param name="camera">the new camera entity</param>
		inline void SetLayerCamera(const CameraEntityRef& camera) { 
			m_LayerCamera = camera;
			m_LayerCamera->SetRenderEveryFrame(true);
		}
		
		/// <summary>
		/// Get a weak pointer to the owned camera.
		/// </summary>
		/// <returns></returns>
		CameraEntityNoRef GetLayerCamera() { return m_LayerCamera; }

		/// <summary>
		/// Add a camera to render the scene next frame
		/// </summary>
		/// <param name="camera"></param>
		void EnqueCamera(CameraEntityNoRef camera) {
			m_CameraQueue.insert(camera);
		}

	protected:
		/// <summary>
		/// Get a const ref to the list of entities that are root in this layer
		/// </summary>
		/// <returns>const ref to list of entities that are root in this layer</returns>
		const std::list<EntityRef>& GetEntityList() const { return m_Entities; }


	private:
		inline void AddManifoldToQueue(Manifold&& m) {m_FrameManifoldQueue.push_back(m);}

	private:

		struct CameraHasher {
			std::size_t operator()(const CameraEntityNoRef& t) const {
				return std::hash<void*>()(t.lock().get());
			}
		};

		std::list<EntityRef> m_Entities;
		std::list<EntityNoRef> m_DestroyedEntities;
		std::list<EventListenerNoRef> m_Listeners;
		std::list<Manifold> m_FrameManifoldQueue;
		std::unordered_set<CameraEntityNoRef, CameraHasher> m_CameraQueue;
		CameraEntityRef m_LayerCamera; //intentonally an owning pointer
		bool m_Dead = false; //used by Scene to destroy layers
	};


	/****************************************************************
	 *                     Implementations                          *
	 ****************************************************************/


	template<class T>
	inline std::list<std::shared_ptr<T>> Layer::GetAllEntitiesOfClassInBox(const BoundingBox& box)
	{
		static_assert(std::is_base_of<Entity, T>::value, "Provided class is not a subclass of Tara::Entity");

		std::list<EntityRef> PotentialOverlaps = GetAllEntitiesInBox(box);
		std::list<std::shared_ptr<T>> Overlaps;
		for (auto entity : PotentialOverlaps) {
			std::shared_ptr<T> target = std::dynamic_pointer_cast<T>(entity);
			if (target) {
				Overlaps.push_back(target);
			}
		}
		return Overlaps;
	}

	template<class T>
	inline std::list<std::shared_ptr<T>> Layer::GetAllEntitiesOfClassInRadius(Vector origin, float radius)
	{
		static_assert(std::is_base_of<Entity, T>::value, "Provided class is not a subclass of Tara::Entity");

		std::list<EntityRef> PotentialOverlaps = GetAllEntitiesInRadius(origin, radius);
		std::list<std::shared_ptr<T>> Overlaps;
		for (auto entity : PotentialOverlaps) {
			std::shared_ptr<T> target = std::dynamic_pointer_cast<T>(entity);
			if (target) {
				Overlaps.push_back(target);
			}
		}
		return Overlaps;
	}


}

#undef LISTTYPE
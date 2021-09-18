#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"
#include "Tara/Core/Entity.h"

namespace Tara {

	/// <summary>
	/// Layer Class. Layers are "Draw slices" of a scene. 
	/// This class should be subclassed to make a custom application, and 
	/// a instance of that subclass pushed to a scene.
	/// </summary>
	class Layer : public std::enable_shared_from_this<Layer> {
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

		//TODO: stuff relating to entities and components

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


	protected:
		/// <summary>
		/// Get a const ref to the list of entities that are root in this layer
		/// </summary>
		/// <returns>const ref to list of entities that are root in this layer</returns>
		const std::list<EntityRef>& GetEntityList() const { return m_Entities; }

	private:

		std::list<EntityRef> m_Entities;

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
}
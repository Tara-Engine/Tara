#pragma once

#include "tarapch.h"
#include <glm/glm.hpp>
#include "Tara/Math/Types.h"



namespace Tara {

	class Layer;

	/// <summary>
	/// Base class for all entities in the game. 
	/// Entities are game objects that have a location in the world.
	/// </summary>
	class Entity : public std::enable_shared_from_this<Entity> {
		/// <summary>
		/// Reference to entity
		/// </summary>
		using EntityRef = std::shared_ptr<Entity>;
	public:
		/// <summary>
		/// Basic entity constructor.
		/// </summary>
		/// <param name="name">the name of the entity</param>
		Entity(std::weak_ptr<Entity> parent, std::weak_ptr<Layer> owningLayer,  std::string name = "Entity")
			:m_Parent(parent), m_OwningLayer(owningLayer), m_Name(name)
		{};

		/// <summary>
		/// basic entity destructor
		/// </summary>
		virtual ~Entity() = default;

		/// <summary>
		/// The overridable update function. This should be overriden for all subclasses
		/// </summary>
		/// <param name="deltaTime">the time between frames, in seconds</param>
		virtual void OnUpdate(float deltaTime) = 0;

		/// <summary>
		/// The overridable draw function. This should be overriden for all subclasses
		/// </summary>
		/// <param name="deltaTime">The time between frames, in seconds</param>
		virtual void OnDraw(float deltaTime) = 0;


	public:

		/// <summary>
		/// Get the transform of the Entity
		/// </summary>
		/// <returns>the current transform</returns>
		const Transform& GetTransform() const { return m_Transform; }
		
		/// <summary>
		/// Set the transform of the entity
		/// </summary>
		/// <param name="transform">the new transform</param>
		void SetTransform(const Transform& transform) { m_Transform = transform; }

		/// <summary>
		/// Get the name of the entity
		/// </summary>
		/// <returns>the name as string</returns>
		const std::string& GetName() const { return m_Name; }

		/// <summary>
		/// Check if a particular entity reference is a child.
		/// </summary>
		/// <param name="ref">the reference to check</param>
		/// <returns>true if it is a child, false otherwise</returns>
		bool IsChild(EntityRef ref, bool recursive = false) const;

		/// <summary>
		/// Get the first child of a particular name.
		/// </summary>
		/// <param name="name">the name to check for</param>
		/// <returns>the child as reference, or nullptr if not found</returns>
		EntityRef GetFirstChildOfName(const std::string& name) const;
		
		/// <summary>
		/// Remove a child by its name. Only removes the first child with this name
		/// </summary>
		/// <param name="name">The child name</param>
		/// <returns>the child, if removed. nullptr otherwise</returns>
		EntityRef RemoveChildByName(const std::string& name);
		
		/// <summary>
		/// Remove a specific reference from the children list
		/// </summary>
		/// <param name="ref">the child to remove</param>
		/// /// <param name="setToLayer">if false, then the entity is not given to the layer</param>
		/// <returns>should be true if remove, false otherwise, but currently always true</returns>
		bool RemoveChildByRef(EntityRef ref, bool setToLayer = true);
		
		/// <summary>
		/// Add a new child to an entity
		/// </summary>
		/// <param name="ref">the child to add</param>
		/// <returns>true if added, false if already a child</returns>
		bool AddChild(EntityRef ref);

		/// <summary>
		/// Swap the current parent for a new one. Does not work if the current entity is root. You cannot parent to your own child.
		/// </summary>
		/// <param name="newParent">the new parent enetity</param>
		/// <returns>true if operation was successful</returns>
		bool SwapParent(std::weak_ptr<Entity> newParent);

		/// <summary>
		/// Get the parent of an entity.
		/// </summary>
		/// <returns>The parent</returns>
		std::weak_ptr<Entity> GetParent() const { return m_Parent; }


		/// <summary>
		/// Update the entity. Should not be manually called
		/// </summary>
		/// <param name="deltaTime"></param>
		void Update(float deltaTime);
		
		/// <summary>
		/// Draw the entity. Should not be manually called.
		/// </summary>
		/// <param name="deltaTime"></param>
		void Draw(float deltaTime);
	
protected:

		/// <summary>
		/// Set the parent of an entity. Should not be manually called
		/// </summary>
		/// <param name="newParent"> the new parent</param>
		void SetParent(std::weak_ptr<Entity> newParent, bool ignoreChecks = false);


	protected:
		const std::string m_Name;
		Transform m_Transform;
		std::weak_ptr<Entity> m_Parent;
		const std::weak_ptr<Layer> m_OwningLayer;
		std::list<EntityRef> m_Children;
	};

	/// <summary>
	/// Reference to the base class for all entities in the game. 
	/// Entities are game objects that have a location in the world.
	/// </summary>
	using EntityRef = std::shared_ptr<Entity>;
}
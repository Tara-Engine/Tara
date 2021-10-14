#pragma once

#include "tarapch.h"
#include <glm/glm.hpp>
#include "Tara/Math/Types.h"
#include "Tara/Math/BoundingBox.h"
#include "Tara/Input/EventListener.h"

namespace Tara {

	class Layer;
	struct Manifold;

	/// <summary>
	/// Base class for all entities in the game. 
	/// Entities are game objects that have a location in the world.
	/// </summary>
	class Entity : public std::enable_shared_from_this<Entity>, public EventListener {
		/// <summary>
		/// So that Layer can access protected functions from Entity.
		/// </summary>
		friend class Layer;
		
		

	public: //so as to not break subclasses

		/// <summary>
		/// Reference to entity
		/// </summary>
		using EntityRef = std::shared_ptr<Entity>;
		
		/// <summary>
		/// non-owning (No) refrerence to entity
		/// </summary>
		using EntityNoRef = std::weak_ptr<Entity>;
		

	public:
		/***********************************************************************************
		*                          Construction Functions                                  *
		************************************************************************************/


		/// <summary>
		/// Basic entity constructor.
		/// Since entities cannot add themselves to their parents or the layer (due to shared pointer strangeness)
		/// they way to contstruct them is the Create static function. Thus, this is protected
		/// </summary>
		/// <param name="name">the name of the entity</param>
		Entity(EntityNoRef parent, std::weak_ptr<Layer> owningLayer, Transform transform,  std::string name);

		/// <summary>
		/// Create a new entity
		/// </summary>
		/// <param name="parent">the parent entity, may be null</param>
		/// <param name="owningLayer">the owning layer, may not be null</param>
		/// <param name="transform">the transform, defaults to origin, no rot, and a scale of 1</param>
		/// <param name="name">the name</param>
		/// <returns>A reference to the new entity</returns>
		static EntityRef Create(EntityNoRef parent, std::weak_ptr<Layer> owningLayer, Transform transform = TRANSFORM_DEFAULT, std::string name = "Entity");

		/// <summary>
		/// Register an externally created entity. This adds it to the parent, etc.
		/// </summary>
		/// <param name="ref">the entity to register</param>
		static void Register(EntityRef ref);

		/// <summary>
		/// basic entity destructor
		/// </summary>
		virtual ~Entity() = default;
	
	public:

		/***********************************************************************************
		*                          Overridable Functions                                   *
		************************************************************************************/

		/// <summary>
		/// The overridable update function. This should be overriden for all subclasses
		/// </summary>
		/// <param name="deltaTime">the time between frames, in seconds</param>
		inline virtual void OnUpdate(float deltaTime) {};

		/// <summary>
		/// The overridable draw function. This should be overriden for all subclasses
		/// </summary>
		/// <param name="deltaTime">The time between frames, in seconds</param>
		inline virtual void OnDraw(float deltaTime) {};

		/// <summary>
		/// Get the specific bounding box of the entity. Should be overridden for subtypes
		/// </summary>
		/// <returns>the bounding box</returns>
		inline virtual BoundingBox GetSpecificBoundingBox() const { return { 0,0,0,-1,-1,-1 }; }

	public:
		/***********************************************************************************
		*                          Basic Utility Funcions                                  *
		************************************************************************************/

		/// <summary>
		/// Get the transform of the Entity relative to parent
		/// </summary>
		/// <returns>the current transform</returns>
		inline const Transform& GetRelativeTransform() const { return m_Transform; }
		
		/// <summary>
		/// Get the world transform of the Entity
		/// </summary>
		/// <returns>the current world transform</returns>
		Transform GetWorldTransform() const;

		/// <summary>
		/// Set the relative transform of the entity
		/// </summary>
		/// <param name="transform">the new transform</param>
		inline inline void SetRelativeTransform(const Transform& transform) { m_Transform = transform; }

		/// <summary>
		/// set the world transform of an entity
		/// </summary>
		void SetWorldTransform(const Transform& transform);

		/// <summary>
		/// Get the relative position of the Entity
		/// </summary>
		/// <returns>Position as a vector</returns>
		inline const Vector& GetRelativePosition() const { return m_Transform.Position; }

		/// <summary>
		/// Set the relative position of the Entity
		/// </summary>
		/// <param name="pos">the new position, as a vector</param>
		inline void SetRelativePosition(const Vector& pos) { m_Transform.Position = pos; }

		/// <summary>
		/// Get the world position of the Entity
		/// </summary>
		/// <returns>World position as a vector</returns>
		inline Vector GetWorldPosition() const { return GetWorldTransform().Position; };

		/// <summary>
		/// Set the world position of the Entity
		/// </summary>
		/// <param name="pos">the new position, as a vector</param>
		void SetWorldPosition(const Vector& pos);

		/// <summary>
		/// Get the relative rotation of the Entity
		/// </summary>
		/// <returns>The relative rotation, as Rotator</returns>
		inline const Rotator& GetRelativeRotation() const { return m_Transform.Rotation; }

		/// <summary>
		/// Set the relative rotation of the Entity
		/// </summary>
		/// <param name="rot">the new rotation, as Rotator</param>
		inline void SetRelativeRotation(const Rotator& rot) { m_Transform.Rotation = rot; }

		/// <summary>
		/// Get the world rotation of the Entity
		/// </summary>
		/// <returns>The relative rotation, as Rotator</returns>
		inline Rotator GetWorldRotation() const { return GetWorldTransform().Rotation; }

		/// <summary>
		/// Set the world rotation of the Entity
		/// </summary>
		/// <param name="rot">the new rotation, as Rotator</param>
		void SetWorldRotation(const Rotator& rot);

		/// <summary>
		/// Get the relative scale of the Entity
		/// </summary>
		/// <returns>The relative scale, as Vector</returns>
		inline const Vector& GetRelativeScale() const { return m_Transform.Scale; }

		/// <summary>
		/// Set the relative scale of the Entity
		/// </summary>
		/// <param name="rot">the new scale, as Vector</param>
		inline void SetRelativeScale(const Vector& scale) { m_Transform.Scale = scale; }

		/// <summary>
		/// Get the world scale of the Entity
		/// </summary>
		/// <returns>The relative scale, as Vector</returns>
		inline Vector GetWorldScale() const { return GetWorldTransform().Scale; }

		/// <summary>
		/// Set the world scale of the Entity
		/// </summary>
		/// <param name="rot">the new scale, as Vector</param>
		void SetWorldScale(const Vector& scale);

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
		bool SwapParent(EntityNoRef newParent);

		/// <summary>
		/// Get the parent of an entity.
		/// </summary>
		/// <returns>The parent</returns>
		EntityNoRef GetParent() const { return m_Parent; }

		
		
		/// <summary>
		/// Debug function, logs the name of every child to the output
		/// </summary>
		/// <param name="recursive">if it should be done recursively.</param>
		/// <param name="indentLevel">Internal Use Only</param>
		void DebugLogAllChildren(bool recursive = false, int indentLevel = 0) const;

		/// <summary>
		/// Get the bounding box around this and all children entities.
		/// </summary>
		/// <returns></returns>
		BoundingBox GetFullBoundingBox() const;

		/// <summary>
		/// Enable/Disable Listening for application window native events
		/// </summary>
		/// <param name="enabled"></param>
		virtual void ListenForEvents(bool enable = true) final override;

		/// <summary>
		/// Get a weak ref to the owning layer
		/// </summary>
		/// <returns>a weak ref to the owning layer</returns>
		inline std::weak_ptr<Layer> GetOwningLayer() const { return m_OwningLayer; }


		/// <summary>
		/// Set if the chilren of this entity should be updated first
		/// </summary>
		/// <param name="v">true if they should be, false otherwise.</param>
		inline void SetUpdateChildrenFirst(bool v) { m_UpdateChildrenFirst = v; }

		/// <summary>
		/// Get if the chilren of this entity should be updated first
		/// </summary>
		/// <returns></returns>
		inline bool GetUpdateChildrenFirst() const { return m_UpdateChildrenFirst; }

		/// <summary>
		/// Set if the chilren of this entity should be drawn first
		/// </summary>
		/// <param name="v">true if they should be, false otherwise.</param>
		inline void SetDrawChildrenFirst(bool v) { m_DrawChildrenFirst = v; }

		/// <summary>
		/// Get if the chilren of this entity should be drawn first
		/// </summary>
		/// <returns></returns>
		inline bool GetDrawChildrenFirst() const { return m_DrawChildrenFirst; }


protected:
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


		/// <summary>
		/// Get a const ref to the list of children
		/// </summary>
		/// <returns>the list of children</returns>
		const std::list<EntityRef>& GetChildren() const { return m_Children; }

		
	public: //must be public to properly inherit

		/// <summary>
		/// Run Overlap checks for oneself and one's children.
		/// This function should not be called manually, except in an overloaded version of this function
		/// override to make a custom manner of handling overlap checks for a specific type of entity
		/// </summary>
		virtual void SelfOverlapChecks();

		/// <summary>
		/// Run an overlap check between self+children and another, entity
		/// This function should not be called manually, except in an overloaded version of this function or an overloaded version of SelfOverlapChecks()
		/// override to make a custom manner of handling overlap checks for a specific type of entity
		/// Responsible for generating manifolds between two objects
		/// </summary>
		/// <param name="other"></param>
		virtual void OtherOverlapChecks(EntityRef other);


		/// <summary>
		/// Apppend to a list of all the children that overlap a bounding box
		/// </summary>
		/// <param name="box">the box to check with</param>
		/// <param name="list">the list to append to</param>
		virtual void GetAllChildrenInBox(const BoundingBox& box, std::list<EntityRef>& list);
		
		
		/// <summary>
		/// Append to a list of all the children that overlap a circle/sphere
		/// </summary>
		/// <param name="origin">the position</param>
		/// <param name="radius">the radius</param>
		/// <param name="list">the list to append to</param>
		virtual void GetAllChildrenInRadius(Vector origin, float radius, std::list<EntityRef>& list);

	private:
		/// <summary>
		/// Set the parent of an entity. Should not be manually called
		/// </summary>
		/// <param name="newParent"> the new parent</param>
		void SetParent(EntityNoRef newParent, bool ignoreChecks = false);

		


	private:
		const std::string m_Name;
		Transform m_Transform;
		const std::weak_ptr<Layer> m_OwningLayer;
		EntityNoRef m_Parent;
		std::list<EntityRef> m_Children;
		bool m_UpdateChildrenFirst = true;
		bool m_DrawChildrenFirst = false;
	};

	/// <summary>
	/// Reference to the base class for all entities in the game. 
	/// Entities are game objects that have a location in the world.
	/// </summary>
	using EntityRef = std::shared_ptr<Entity>;

	/// <summary>
	/// non-owning (No) Reference to the base class for all entities in the game. 
	/// Entities are game objects that have a location in the world.
	/// </summary>
	using EntityNoRef = std::weak_ptr<Entity>;

}


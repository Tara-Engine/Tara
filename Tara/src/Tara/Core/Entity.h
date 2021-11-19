#pragma once

#include "tarapch.h"
#include <glm/glm.hpp>
#include "Tara/Math/Types.h"
#include "Tara/Math/BoundingBox.h"
#include "Tara/Input/EventListener.h"
#include "Tara/Input/Event.h"
#include "Tara/Core/Component.h"
#include <sol/sol.hpp>

#define ENTITY_EXISTS(x) if (!Exists()) {return x;}

//#define LISTTYPE std::list
//#define LISTTYPE std::list

namespace Tara {

	REFTYPE(Layer);
	NOREFTYPE(Layer);

	REFTYPE(Entity);
	NOREFTYPE(Entity);

	struct Manifold;

	/// <summary>
	/// Base class for all entities in the game. 
	/// Entities are game objects that have a location in the world.
	/// </summary>
	class Entity : public EventListener, public std::enable_shared_from_this<Entity> {
		/// <summary>
		/// So that Layer can access protected functions from Entity.
		/// </summary>
		friend class Layer;
		

	public:
		/***********************************************************************************
		*                          Construction Functions                                  *
		************************************************************************************/


		/// <summary>
		/// Basic entity constructor.
		/// Since entities cannot add themselves to their parents or the layer (due to shared pointer strangeness)
		/// they way to contstruct them is the Create static function. Thus, this is protected
		/// </summary>
		/// <param name="parent">the parent entity, may be null</param>
		/// <param name="owningLayer">the owning layer, may not be null</param>
		/// <param name="transform">the transform, defaults to origin, no rot, and a scale of 1</param>
		/// <param name="name">the name</param>
		Entity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform,  std::string name);

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
		/// A function called after this entity is added to the hirarchy
		/// </summary>
		inline virtual void OnBeginPlay() {}

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

		/// <summary>
		/// Forward events to 
		/// </summary>
		/// <param name="e"></param>
		virtual void ReceiveEvent(Event& e) override;

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
		inline void SetRelativeTransform(const Transform& transform) { m_Transform = transform; }

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
		/// Get if this entity is marked visible. Will be true even if parent is not visible
		/// </summary>
		/// <returns></returns>
		inline bool GetVisible() const { return m_Visible; }

		/// <summary>
		/// Set if this entity is visible. All children will inherit visiblility effects, but not value. (ie, a visible child of an invisible parent will not be rendered)
		/// </summary>
		/// <param name="visible"></param>
		inline void SetVisible(bool visible) { m_Visible = visible; }

		/// <summary>
		/// Get the filtering bits for what cameras can be render this entity
		/// </summary>
		/// <returns></returns>
		inline virtual uint32_t GetRenderFilterBits() const { return m_RenderFilterBits; }

		/// <summary>
		/// Set the filtering bits for what cameras can be render this entity
		/// </summary>
		/// <param name="bits"></param>
		inline virtual void SetRenderFilterBits(uint32_t bits) { m_RenderFilterBits = bits; }

		/***********************************************************************************
		*                      Relationship Utility Funcions                               *
		************************************************************************************/

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
		/// Get a weak ref to the owning layer
		/// </summary>
		/// <returns>a weak ref to the owning layer</returns>
		inline std::weak_ptr<Layer> GetOwningLayer() const { return m_OwningLayer; }

		/// <summary>
		/// Debug function, logs the name of every child to the output
		/// </summary>
		/// <param name="recursive">if it should be done recursively.</param>
		/// <param name="indentLevel">Internal Use Only</param>
		void DebugLogAllChildren(bool recursive = false, int indentLevel = 0) const;

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

		/***********************************************************************************
		*                          Component Utility Funcions                              *
		************************************************************************************/

		/// <summary>
		/// Add a component
		/// </summary>
		/// <param name="component"></param>
		/// <returns></returns>
		bool AddComponent(ComponentRef component);

		/// <summary>
		/// Check if a component belongs to this entity
		/// </summary>
		/// <param name="ref">the component to check</param>
		/// <returns>true if it belongs, false otherwise</returns>
		bool IsComponent(ComponentRef ref) const;

		/// <summary>
		/// Get the first component that has the supplied name, if any
		/// </summary>
		/// <param name="name">the name</param>
		/// <returns>the first component with that name, or nullptr if none have it</returns>
		ComponentRef GetFirstComponentOfName(const std::string& name) const;

		/// <summary>
		/// Get the first component of a specific subclass of component, if any.
		/// </summary>
		/// <typeparam name="ComponentType">the subclass of component</typeparam>
		/// <returns>the first component of that subclass if any, or nullptr if none found</returns>
		template<typename ComponentType> std::shared_ptr<ComponentType> GetFirstCompontentOfType() const;

		/// <summary>
		/// Remove a component that has a specific name
		/// </summary>
		/// <param name="name">the name to remove</param>
		/// <returns>A reference to the removed component if one was removed, nullptr otherwise</returns>
		ComponentRef RemoveComponentByName(const std::string& name);

		/// <summary>
		/// Remove a component by reference
		/// </summary>
		/// <param name="ref">the reference to that component</param>
		/// <returns>True if the component was remove, false otherwise</returns>
		bool RemoveComponentByRef(ComponentRef ref);


		/// <summary>
		/// Destroy this entity. 
		/// This by default will make all chilren root entities. If something else is desired, do it manually first.
		/// Will not remove the ref you currently have, so you have to do that too.
		/// </summary>
		void Destroy();

		/***********************************************************************************
		*                      Physics and Event Utility Funcions                          *
		************************************************************************************/

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
		void Draw(float deltaTime, const uint32_t& cameraBits);


		/// <summary>
		/// Get a const ref to the list of children
		/// </summary>
		/// <returns>the list of children</returns>
		const std::list<EntityRef>& GetChildren() const { return m_Children; }

		/// <summary>
		/// used for checking if this entity is a real entity, or if its a ghost, zombie entity.
		/// </summary>
		/// <returns></returns>
		inline bool Exists() const { if (!m_Exists) { LOG_S(ERROR) << "Attempting to access a deleted Entity!"; } return m_Exists; }

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

		

	public:
		//Lua Stuff
		inline sol::table __SCRIPT__GetRelativeTransform()	const		{ return GetRelativeTransform().ToScriptTable(); }
		inline sol::table __SCRIPT__GetWorldTransform()	const			{ return GetWorldTransform().ToScriptTable(); }
		inline void __SCRIPT__SetRelativeTransform(sol::table t)		{ SetRelativeTransform(Transform(t)); }
		inline void __SCRIPT__SetWorldTransform(sol::table t)			{ SetWorldTransform(Transform(t)); };
		inline sol::table __SCRIPT__GetRelativePosition() const			{ return GetRelativePosition().ToScriptTable(); }
		inline sol::table __SCRIPT__GetWorldPosition() const			{ return GetWorldPosition().ToScriptTable(); };
		inline void __SCRIPT__SetRelativePosition(sol::table t)			{ SetRelativePosition(Vector(t)); }
		inline void __SCRIPT__SetWorldPosition(sol::table t)			{ SetWorldPosition(Vector(t)); }
		inline sol::table __SCRIPT__GetRelativeRotation() const			{ return GetRelativeRotation().ToScriptTable(); }
		inline sol::table __SCRIPT__GetWorldRotation() const			{ return GetWorldRotation().ToScriptTable(); }
		inline void __SCRIPT__SetRelativeRotation(sol::table t)			{ SetRelativeRotation(Rotator(t)); }
		inline void __SCRIPT__SetWorldRotation(sol::table t)			{ SetWorldRotation(Rotator(t)); }
		inline sol::table __SCRIPT__GetRelativeScale() const			{ return GetRelativeScale().ToScriptTable(); }
		inline sol::table __SCRIPT__GetWorldScale() const				{ return GetWorldScale().ToScriptTable(); }
		inline void __SCRIPT__SetRelativeScale(sol::table t)			{ SetRelativeScale(Vector(t)); }
		inline void __SCRIPT__SetWorldScale(sol::table t)				{ SetWorldScale(Vector(t)); }
		EntityRef __SCRIPT__GetParent() const { return GetParent().lock(); }

		/// <summary>
		/// Register the lua type
		/// </summary>
		/// <param name="lua"></param>
		static void RegisterLuaType(sol::state& lua);

	private:
		/// <summary>
		/// Set the parent of an entity. Should not be manually called
		/// </summary>
		/// <param name="newParent"> the new parent</param>
		void SetParent(EntityNoRef newParent, bool ignoreChecks = false);


	private:
		const std::string m_Name;
		Transform m_Transform;
		const LayerNoRef m_OwningLayer;
		EntityNoRef m_Parent;
		std::list<EntityRef> m_Children;
		std::list<ComponentRef> m_Components;
		uint32_t m_RenderFilterBits;
		bool m_UpdateChildrenFirst = true;
		bool m_DrawChildrenFirst = false;
		bool m_Exists = true;
		bool m_Visible = true;
	};

	/// <summary>
	/// Create a new entity
	/// </summary>
	/// <typeparam name="EntityType">The subclass of Entity to make</typeparam>
	/// <typeparam name="...VA_ARGS">The variable args for its constructor</typeparam>
	/// <param name="...args">the arguments to the subclass's constructor</param>
	/// <returns>A reference to the created Entity</returns>
	template<class EntityType, typename... VA_ARGS>
	inline std::shared_ptr<EntityType> CreateEntity(VA_ARGS&&... args) {
		static_assert(std::is_base_of<Entity, EntityType>::value, "Error: Tara::CreateEntity:: Provided class is not a subclass of Tara::Entity");
		static_assert(std::is_constructible<EntityType, VA_ARGS...>::value, "Error: Tara::CreateEntity:: cannot compile due to paramaters passed not matching constructor of that entity type!");
		std::shared_ptr<EntityType> entity = std::make_shared<EntityType>(std::forward<VA_ARGS>(args)...);
		Entity::Register(entity);
		entity->OnBeginPlay();
		return entity;
	}

	template<typename ComponentType>
	inline std::shared_ptr<ComponentType> Entity::GetFirstCompontentOfType() const
	{
		ENTITY_EXISTS(nullptr);
		for (auto comp : m_Components) {
			auto subtype = std::dynamic_pointer_cast<ComponentType>(comp);
			if (subtype) {
				return subtype;
			}
		}
		return nullptr;
	}



}

#undef LISTTYPE
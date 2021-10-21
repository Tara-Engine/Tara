#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"

namespace Tara {

	REFTYPE(DynamicMultiChildEntity)
	NOREFTYPE(DynamicMultiChildEntity)

	/// <summary>
	/// DynamicMultiChildEntity is an entity designed to optimize collision among its children and between its children and other entities. 
	/// It does this by using an internal AABB tree data structure to handle the transforms.
	/// </summary>
	class DynamicMultiChildEntity : public Entity {
	public:

		/// <summary>
		/// Basic Constructor
		/// </summary>
		/// <param name="parent">parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="transform">the transform</param>
		/// <param name="name">the name</param>
		DynamicMultiChildEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "DynamicMultiChildEntity")
			:Entity(parent, owningLayer, transform, name), m_Tree(nullptr)
		{}

		virtual ~DynamicMultiChildEntity(){
			if (m_Tree) { delete m_Tree; m_Tree = nullptr; }
		}

	public:

		/// <summary>
		/// Run Overlap checks for oneself and one's children.
		/// This function should not be called manually, except in an overloaded version of this function
		/// override to make a custom manner of handling overlap checks for a specific type of entity
		/// </summary>
		virtual void SelfOverlapChecks() override;

		/// <summary>
		/// Run an overlap check between self+children and another, entity
		/// This function should not be called manually, except in an overloaded version of this function or an overloaded version of SelfOverlapChecks()
		/// override to make a custom manner of handling overlap checks for a specific type of entity
		/// Responsible for generating manifolds between two objects
		/// </summary>
		/// <param name="other"></param>
		virtual void OtherOverlapChecks(EntityRef other) override;


		/// <summary>
		/// Apppend to a list of all the children that overlap a bounding box
		/// </summary>
		/// <param name="box">the box to check with</param>
		/// <param name="list">the list to append to</param>
		virtual void GetAllChildrenInBox(const BoundingBox& box, std::list<EntityRef>& list) override;


		/// <summary>
		/// Append to a list of all the children that overlap a circle/sphere
		/// </summary>
		/// <param name="origin">the position</param>
		/// <param name="radius">the radius</param>
		/// <param name="list">the list to append to</param>
		virtual void GetAllChildrenInRadius(Vector origin, float radius, std::list<EntityRef>& list) override;

	private:

		struct Node {
			Node* Left;
			Node* Right;
			BoundingBox Box;

			/// <summary>
			/// The internal entity. This ?should? be const ref because 
			/// (a) it does not need to change and
			/// (B) it does not need to increment the reference count
			/// </summary>
			EntityRef Entity;

			/// <summary>
			/// Basic leaf constructor
			/// </summary>
			/// <param name="entity">the entity to store</param>
			inline Node(const EntityRef& entity)
				: Left(nullptr), Right(nullptr), Box(entity->GetFullBoundingBox()), Entity(entity)
			{}

			/// <summary>
			/// Basic non-leaf constructor
			/// </summary>
			/// <param name="left">the left node of this node</param>
			/// <param name="right">the right node of this node</param>
			inline Node(Node* left, Node* right)
				: Left(left), Right(right), Box(left->Box + right->Box), Entity(nullptr)
			{}

			/// <summary>
			/// basic recursive destructor
			/// </summary>
			inline ~Node() {
				if (Left) { delete Left; Left = nullptr; }
				if (Right) { delete Right; Right = nullptr; }
			}

			/// <summary>
			/// Add a child, and get every leaf it overlaps all in one sweep.
			/// The overlapps are added to the list
			/// </summary>
			/// <param name="newBox"> the box around the new entity. Passed here so it only has to be calculated once per add. Passed by const ref so no copying.</param>
			/// <param name="entity"> the new eneity to add</param>
			/// <param name="overlapping">list to add all overlaps to</param>
			/// <returns>the new node that may be at this position</returns>
			Node* AddChild(const BoundingBox& newBox, EntityRef entity, std::list<EntityRef>& overlapping);

			/// <summary>
			/// Trace a bounding box against the tree, and get every entity that overlaps it.
			/// The list is passed as reference so that there is not a continuous concatinating of lists
			/// </summary>
			/// <param name="box">the box</param>
			/// <param name="overlapping">the list to fill</param>
			void TraceBox(BoundingBox box, std::list<EntityRef>& overlapping);

			/// <summary>
			/// Trace a sphere against the tree, and get every entity that overlaps it
			/// The list is passed as reference so that there is not a continuous concatinating of lists
			/// </summary>
			/// <param name="center">center of sphere</param>
			/// <param name="radius">radius of sphere</param>
			/// <param name="overlapping">the list to fill</param>
			void TraceSphere(Vector center, float radius, std::list<EntityRef>& overlapping);

			/// <summary>
			/// Update bounding box
			/// </summary>
			void UpdateBox();
		};

	private:
		Node* m_Tree;

	};

}
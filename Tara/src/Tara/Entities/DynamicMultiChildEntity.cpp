#include "tarapch.h"
#include "DynamicMultiChildEntity.h"

namespace Tara{

    DynamicMultiChildEntityRef DynamicMultiChildEntity::Create(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name)
    {
        DynamicMultiChildEntityRef newEntity = std::make_shared<DynamicMultiChildEntity>(parent, owningLayer, transform, name);
        Entity::Register(newEntity);
        return newEntity;
    }


    void DynamicMultiChildEntity::SelfOverlapChecks()
    {
        //so, not only do all entities need to have their own self called,
        //they need to be added to the tree so that they can be overlapped with
        //each other efficently
        //Not that there is no need to check the children against this entity, as this eneity does
        //not have a real size. 

        std::list<std::pair<EntityRef, EntityRef>> overlapQueue;

        //clear previous tree
        if (m_Tree) { delete m_Tree; m_Tree = nullptr; }


        for (auto child : GetChildren()) {

            //update child
            child->SelfOverlapChecks();
            
            //add child to tree
            if (m_Tree == nullptr) {
                m_Tree = new Node(child);
            }
            else {
                std::list<EntityRef> overlaps;
                m_Tree = m_Tree->AddChild(child->GetFullBoundingBox(), child, overlaps);
                //for every overlap in the tree, add to queue
                for (auto child2 : overlaps) {
                    overlapQueue.push_back({ child2, child });
                }
            }
        }

        // for every overlap in the queue
        for (auto pair : overlapQueue) {
            pair.first->OtherOverlapChecks(pair.second);
        }
    }

    void DynamicMultiChildEntity::OtherOverlapChecks(EntityRef other)
    {
        //When an entity is passed, it is traced against the tree. Any overlaps are then propigated down
        //Not that it is not checked agains this entity, as this entity has no size.

        if (m_Tree == nullptr) { return; } //early out

        //fill the queue using the trace
        std::list<EntityRef> overlapQueue;
        m_Tree->TraceBox(other->GetFullBoundingBox(), overlapQueue);

        //propigate down
        for (auto child : overlapQueue) {
            child->OtherOverlapChecks(other);
        }
    }







    DynamicMultiChildEntity::Node* DynamicMultiChildEntity::Node::AddChild(const BoundingBox& newBox, EntityRef entity, std::list<EntityRef>& overlapping)
    {
        //BoundingBox newBox = entity->GetFullBoundingBox();
        if (Entity) {
            //This is a leaf node. Create sibling and new parent of both. 
            Node* sibling = new Node(entity);
            Node* newParent;
            if (newBox < Box) {
                newParent = new Node(sibling, this);
            }
            else {
                newParent = new Node(this, sibling);
            }
            //Return that new parent to replace this in tree position
            return newParent;
        }
        //this is not a leaf node (leaves return before this)
        if (Left->Box.Overlaping(newBox)) {
            //check if Left is a leaf, and, if it is, then add to overlapping list
            //must be done first, because after AddChild, Left is definately not a leaf
            if (Left->Entity) {
                overlapping.push_back(Left->Entity);
            }
            //overlapping the Left side, so, we add it to that side
            Left = Left->AddChild(newBox, entity, overlapping);
            UpdateBox();
            //also check if overlapping Right. If it is, trace against it to add any leaves that side to overlapping, if they are
            if (Right->Box.Overlaping(newBox)) {
                Right->TraceBox(newBox, overlapping);
            }
        }
        else {
            if (Right->Box.Overlaping(newBox)) {
                //check if Right is a leaf, and, if it is, then add to overlapping list
                //must be done first, because after AddChild, Right is definately not a leaf
                if (Right->Entity) {
                    overlapping.push_back(Right->Entity);
                }
                //overlapping with the Right side, so, we add it to that side.
                Right = Right->AddChild(newBox, entity, overlapping);
                UpdateBox();
                //No need to check Left, as that already failed.
            }
            else {
                //neither are overlapping. check combined boxes to get optimal non-overlap
                if (Left->Box.Overlaping(newBox + Right->Box)) {
                    Left = Left->AddChild(newBox, entity, overlapping);
                }
                else {
                    Right = Right->AddChild(newBox, entity, overlapping);
                }
                UpdateBox();
            }
        }
        //return this because we don't need to replace anything
        return this;
    }

    void DynamicMultiChildEntity::Node::TraceBox(BoundingBox box, std::list<EntityRef>& overlapping)
    {
        if (Entity != nullptr) {
            overlapping.push_back(Entity);
        }
        else {
            if (Left->Box.Overlaping(box)) {
                Left->TraceBox(box, overlapping);
            }
            if (Right->Box.Overlaping(box)) {
                Right->TraceBox(box, overlapping);
            }
        }
    }

    void DynamicMultiChildEntity::Node::UpdateBox()
    {
        if (Left && Right) {
            Left->UpdateBox();
            Right->UpdateBox();
            Box = Left->Box + Right->Box;
        }
    }

}

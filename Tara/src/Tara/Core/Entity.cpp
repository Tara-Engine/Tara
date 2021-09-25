#include "tarapch.h"
#include "Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Input/ApplicationEvents.h"
#include "Tara/Input/Manifold.h"

namespace Tara{
    Entity::Entity(EntityNoRef parent, std::weak_ptr<Layer> owningLayer, Transform transform, std::string name)
        :m_Parent(parent), m_OwningLayer(owningLayer), m_Name(name), m_Transform(transform)
    {

        CHECK_NOTNULL_F(m_OwningLayer.lock(), "the owning layer of a newly created entity should never be null!");
        
        /*
        //entities cannot add themselves to their parent or the layer, as they have ... issues with shared pointers
        if (m_Parent.lock()) {
            m_Parent.lock()->AddChild(shared_from_this());
        }
        else {
            m_OwningLayer.lock()->AddEntity(shared_from_this());
        }
        */
    }

    EntityRef Entity::Create(EntityNoRef parent, std::weak_ptr<Layer> owningLayer, Transform transform, std::string name)
    {
        EntityRef newEntity = std::make_shared<Entity>(parent, owningLayer, transform, name);
        Register(newEntity);
        return newEntity;
    }

    void Entity::Register(EntityRef ref)
    {
        if (ref->m_Parent.lock()) {
            ref->m_Parent.lock()->AddChild(ref); //auto cast to weak_ptr
        }
        else {
            bool result = ref->m_OwningLayer.lock()->AddEntity(ref);//auto cast to weak_ptr
            //LOG_S(INFO) << "Entity Registered! Name: [" << ref->GetName() << "] Sucess? " << result;
        }
    }

    Transform Entity::GetWorldTransform() const
    {
        if (GetParent().lock()) {
            auto parentTransform = GetParent().lock()->GetWorldTransform();
            return parentTransform + m_Transform;
        }
        else {
            return m_Transform;
        }
    }

    bool Entity::IsChild(EntityRef ref, bool recursive) const
    {
        if (std::find(m_Children.begin(), m_Children.end(), ref) != m_Children.end()) {
            return true;
        }
        else {
            if (recursive) {
                for (auto child : m_Children) {
                    if (child->IsChild(ref, true)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }


    EntityRef Entity::GetFirstChildOfName(const std::string& name) const
    {
        for (auto child : m_Children) {
            if (child->GetName() == name) {
                return child;
            }
        }
        return nullptr;
    }

    EntityRef Entity::RemoveChildByName(const std::string& name)
    {
        for (auto child : m_Children) {
            if (child->GetName() == name) {
                if (RemoveChildByRef(child)) {
                    return child;
                }
                else {
                    return nullptr;
                }
            }
        }
        return nullptr;
    }

    //TODO: add parenting child to level
    bool Entity::RemoveChildByRef(EntityRef ref, bool setToLayer)
    {
        if (&*(ref->GetParent().lock()) == this) {
            ref->SetParent(std::weak_ptr<Entity>());
            m_Children.remove(ref);
            if (setToLayer) {
                m_OwningLayer.lock()->AddEntity(ref);
            }
            return true;
        }
        else {
            return false;
        }
        
    }

    //TODO: remove from current parent or level
    bool Entity::AddChild(EntityRef ref)
    {
        //early out if ref is already a immedate child
        if (IsChild(ref)) {
            return false;
        }

        //early out if self is child of ref
        if (ref->IsChild(shared_from_this(), true)) {
            return false;
        }

        //if part of layer, remove
        if (m_OwningLayer.lock()->IsEntityRoot(ref)) {
            m_OwningLayer.lock()->RemoveEntity(ref);
        }

        if (ref->GetParent().lock()) {
            //if has a parent, swap
            ref->SwapParent(weak_from_this());
        }
        else {
            //else, just add to this
            ref->SetParent(weak_from_this(), true);
            m_Children.push_back(ref);
        }
        return true;
    }

    bool Entity::SwapParent(EntityNoRef newParent)
    {
        //check if newParent can be parent
        if (IsChild(newParent.lock(), true)) {
            return false;
        }
        
        //remove the entity from old parent
        m_Parent.lock()->RemoveChildByRef(shared_from_this(), false);

        //add to new parent
        SetParent(newParent, true);
        newParent.lock()->m_Children.push_back(shared_from_this());
        return false;
    }



    void Entity::Update(float deltaTime)
    {
        //update children first, then self
        for (auto child : m_Children) {
            child->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }

    void Entity::Draw(float deltaTime)
    {
        //draw self first, then children
        OnDraw( deltaTime);
        for (auto child : m_Children) {
            child->Draw(deltaTime);
        }
    }

    void Entity::DebugLogAllChildren(bool recursive, int indentLevel) const
    {
        for (auto child : m_Children) {
            LOG_S(INFO) << std::string(indentLevel, ' ') << child->GetName() << "{" << (child->GetParent().lock() == shared_from_this()) << "}";
            if (recursive) {
                child->DebugLogAllChildren(true, indentLevel + 1);
            }
            
        }
    }

    BoundingBox Entity::GetFullBoundingBox() const
    {
        BoundingBox box = GetSpecificBoundingBox();
        for (auto child : m_Children) {
            box = box + child->GetFullBoundingBox(); //not += because that is not overloaded.
        }
        return box;
    }

    void Entity::ListenForEvents(bool enable)
    {
        m_OwningLayer.lock()->EnableListener(weak_from_this(), enable);
        SetListeningForEvents(enable);
    }

    void Entity::SetParent(EntityNoRef newParent, bool ignoreChecks)
    {
        if (newParent.lock() == nullptr) {
            LOG_S(INFO) << "Entity::SetParent clearing the parent.";
            m_Parent = EntityNoRef();
            return;
        }
        //make sure the new parent is not a child
        //if ignoreChecks is true, it should not run the IsChild function
        if (!ignoreChecks || IsChild(newParent.lock(), true)) {
            LOG_S(WARNING) << "Entity::SetParent called, but parent not set.";
            return;
        }
        m_Parent = newParent;
    }

    void Entity::SelfOverlapChecks()
    {
        std::list<std::pair<EntityRef, EntityRef>> overlapQueue;

        for (auto iter1 = m_Children.begin(); iter1 != m_Children.end(); iter1++) {
            //for all entities, check their own children
            EntityRef child1 = *iter1;
            child1->SelfOverlapChecks();

            //run all children x root, 
            if (GetSpecificBoundingBox().Overlaping(child1->GetFullBoundingBox())) {
                //for any that overlap (Full AABB for children only), queue up
                overlapQueue.push_back(std::make_pair( shared_from_this(), child1 ));
            }

            //run all children x children full AABB overlap check
            auto iter2 = iter1;
            iter2++;
            for (; iter2 != m_Children.end(); iter2++) {
                EntityRef child2 = *iter2;
                if (child1->GetFullBoundingBox().Overlaping(child2->GetFullBoundingBox())) {
                    //for any that overlap (Full AABB for children only), queue up
                    overlapQueue.push_back(std::make_pair( child1, child2 ));
                }
            }
        }

        //when finished, run through that queue and call OtherOverlapChecks on them
        for (auto pair : overlapQueue) {
            pair.first->OtherOverlapChecks(pair.second);
        }
    }

    void Entity::OtherOverlapChecks(EntityRef other)
    {
        //IF the core AABB of self and other overlap, THEN generate overlap event
        if (GetSpecificBoundingBox().Overlaping(other->GetSpecificBoundingBox())) {
            Manifold m(shared_from_this(), other);
            m_OwningLayer.lock()->AddManifoldToQueue(std::move(m));

            //INENTIONAL NO RETURN
        }

        //ALSO, filter it down more, by checking children against other full AABB, and get list of children that overlap
        std::list<EntityRef> selfPotentialChildrenQueue;
        BoundingBox otherBox= other->GetFullBoundingBox(); //cache this
        for (auto child : m_Children) {
            if (child == other) { continue; } //otherwise, it starts colliding children and self.

            if (child->GetFullBoundingBox().Overlaping(otherBox)) {
                selfPotentialChildrenQueue.push_back(child);
            }
        }
        //other's children against our full aabb
        std::list<EntityRef> otherPotentialChildrenQueue;
        otherBox = GetFullBoundingBox(); //cache this
        for (auto otherChild : other->m_Children) {
            if (otherBox.Overlaping(otherChild->GetFullBoundingBox())) {
                otherPotentialChildrenQueue.push_back(otherChild);
            }
        }

        //THEN, find the self+children in that list that overlap other's core or potential children.
        std::list<std::pair<EntityRef, EntityRef>> overlapQueue;
        otherBox = other->GetSpecificBoundingBox();
        //first, do self's potential children against the other root
        for (auto child : selfPotentialChildrenQueue) {
            if (child == other) {
                continue;// skip adding the child again
            } 
            if (child->GetFullBoundingBox().Overlaping(otherBox)) {
                //for each of those, queue up
                overlapQueue.push_back(std::make_pair( child, other ));
            }
        }
        for (auto otherChild : otherPotentialChildrenQueue) {
            otherBox = otherChild->GetFullBoundingBox(); //cache this
            //second, self root against other children
            if (GetSpecificBoundingBox().Overlaping(otherBox)) {
                //for each of those, queue up
                overlapQueue.push_back(std::make_pair(shared_from_this(), otherChild ));
            }
            //third, self potential children against other's children
            for (auto child : selfPotentialChildrenQueue) {
                if (child->GetFullBoundingBox().Overlaping(otherBox)) {
                   //for each of those, queue up
                    overlapQueue.push_back(std::make_pair(child, otherChild ));
                }
            }
        }

        //when done checking all, run through queue and call this function recursively
        for (auto pair : overlapQueue) {
            //these checks no longer nesecary, the conditions that would make them true are no longer added to queue
            //if (pair.second->IsChild(pair.first, true)) {
            //    continue;
            //}
            //if (pair.first != pair.second) {
                pair.first->OtherOverlapChecks(pair.second);
            //}
            //else{
            //    LOG_S(WARNING) << "Infinite recursion blocked in overlap detection: " << GetName() << " is trying to overlap itself!";
            //}
        }
    }

    



}
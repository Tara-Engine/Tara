#include "tarapch.h"
#include "Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Input/ApplicationEvents.h"
#include "Tara/Input/Manifold.h"
#include "Tara/Core/Script.h"

#pragma warning( push )
#pragma warning( disable : 4003 ) 
//disable MSVC warning C4003. It has to do with not enough params in a function-like macro, which is being done intentionally here.

namespace Tara{
    
    Entity::Entity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, const std::string& name)
        :m_Transform(transform), m_RenderFilterBits(~0), m_Name(name),
        m_OwningLayer((parent.lock() && !owningLayer.lock()) ? parent.lock()->GetOwningLayer() : owningLayer), 
        m_Parent(parent),
        m_Exists(true), m_UpdateChildrenFirst(true), m_UpdateComponentsFirst(true), m_DrawChildrenFirst(false), m_Visible(true)
    {
        CHECK_NOTNULL_F(m_OwningLayer.lock(), "the owning layer of a newly created entity should never be null!");
    }

    void Entity::Register(EntityRef ref)
    {
        if (ref->m_Parent.lock()) {
            ref->m_Parent.lock()->AddChild(ref); //auto cast to weak_ptr
        }
        else {
            ref->m_OwningLayer.lock()->AddEntity(ref);//auto cast to weak_ptr
            //LOG_S(INFO) << "Entity Registered! Name: [" << ref->GetName() << "] Sucess? " << result;
        }
    }

    void Entity::ReceiveEvent(Event& e)
    {
        ENTITY_EXISTS();
        OnEvent(e);
        for (auto comp : m_Components) {
            if (!(comp->GetListeningForEvents() && (e.GetCategoryFlags() & EventCategoryNative))) { //if both the entity and the component are listening for native window events, don't forward
                comp->ReceiveEvent(e);
            }
        }

    }

    Transform Entity::GetWorldTransform() const
    {
        ENTITY_EXISTS(TRANSFORM_DEFAULT);
        if (GetParent().lock()) {
            auto parentTransform = GetParent().lock()->GetWorldTransform();
            return parentTransform + m_Transform;
        }
        else {
            return m_Transform;
        }
    }

    void Entity::SetWorldTransform(const Transform& transform)
    {
        ENTITY_EXISTS();
        if (GetParent().lock()) {
            auto parentTransform = GetParent().lock()->GetWorldTransform();
            m_Transform = transform - parentTransform;
        }
        else {
            m_Transform = transform;
        }
    }

    void Entity::SetWorldPosition(const Vector& pos)
    {
        ENTITY_EXISTS();
        if (GetParent().lock()) {
            auto parentTransform = GetParent().lock()->GetWorldTransform();
            m_Transform.Position = pos - parentTransform.Position;
        }
        else {
            m_Transform.Position = pos;
        }
    }

    void Entity::SetWorldRotation(const Rotator& rot)
    {
        ENTITY_EXISTS();
        if (GetParent().lock()) {
            auto parentTransform = GetParent().lock()->GetWorldTransform();
            m_Transform.Rotation = rot - parentTransform.Rotation;
        }
        else {
            m_Transform.Rotation = rot;
        }
    }

    void Entity::SetWorldScale(const Vector& scale)
    {
        ENTITY_EXISTS();
        if (GetParent().lock()) {
            auto parentTransform = GetParent().lock()->GetWorldTransform();
            m_Transform.Scale = scale - parentTransform.Scale;
        }
        else {
            m_Transform.Scale = scale;
        }
    }

    void Entity::Destroy()
    {
        ENTITY_EXISTS();
        LOG_S(INFO) << "Entity destroyed. Should be cleaned soon.";
        m_Exists = false;
        auto sthis = shared_from_this();
        //first, remove from hirarchy entirely (not root, not child)
        if (m_Parent.lock()) {
            m_Parent.lock()->RemoveChildByRef(sthis, false);
        }
        if (m_OwningLayer.lock()->IsEntityRoot(sthis)) {
            m_OwningLayer.lock()->RemoveEntity(sthis);
        }
        //take care of childrend
        while(m_Children.size() > 0){
            auto child = m_Children.front();
            RemoveChildByRef(child, true);
        }
        //mark destroyed for the layer's cleanup policies
        m_OwningLayer.lock()->MarkDestroyed(weak_from_this());
    }


    bool Entity::IsChild(EntityRef ref, bool recursive) const
    {
        ENTITY_EXISTS(false);
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
        ENTITY_EXISTS(nullptr);
        for (auto child : m_Children) {
            if (child->GetName() == name) {
                return child;
            }
        }
        return nullptr;
    }

    EntityRef Entity::RemoveChildByName(const std::string& name)
    {
        ENTITY_EXISTS(nullptr);
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
        ENTITY_EXISTS(false);
        if (&*(ref->GetParent().lock()) == this) {
            ref->SetParent(std::weak_ptr<Entity>());
            m_Children.remove(ref);
            if (setToLayer) {
                m_OwningLayer.lock()->AddEntity(ref);
                //event to child, only if setToLayer is true. Otherwhise, whatever called this will handle it.
                ParentSwapedEvent parentSwappedEvent(weak_from_this(), EntityNoRef());
                ref->ReceiveEvent(parentSwappedEvent);
            }
            //send event to parent (self)
            ChildRemovedEvent childRemoveEvent(weak_from_this(), ref);
            ReceiveEvent(childRemoveEvent);
            return true;
        }
        else {
            return false;
        }
        
    }

    //TODO: remove from current parent or level
    bool Entity::AddChild(EntityRef ref)
    {
        ENTITY_EXISTS(false);
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
            //parent swap event handled
        }
        else {
            //else, just add to this
            ref->SetParent(weak_from_this(), true);
            m_Children.push_back(ref);
            //Parent Swap event
            ParentSwapedEvent parentSwappedEvent(EntityNoRef(), weak_from_this());
            ref->ReceiveEvent(parentSwappedEvent);
        }
        //Child Added Event
        ChildAddedEvent childAddedEvent(weak_from_this(), ref);
        ReceiveEvent(childAddedEvent);
        return true;
    }

    bool Entity::SwapParent(EntityNoRef newParent)
    {
        ENTITY_EXISTS(false);
        //check if newParent can be parent
        if (IsChild(newParent.lock(), true)) {
            return false;
        }
        auto parentCopy = m_Parent;
        //remove the entity from old parent
        m_Parent.lock()->RemoveChildByRef(shared_from_this(), false); //handles ChildRemovedEvent

        //add to new parent
        SetParent(newParent, true);
        newParent.lock()->m_Children.push_back(shared_from_this());
        
        //parent swappedEvent
        ParentSwapedEvent parentSwappedEvent(parentCopy, newParent);
        ReceiveEvent(parentSwappedEvent);
        return false;
    }



    void Entity::Update(float deltaTime)
    {
        ENTITY_EXISTS();
        if (m_UpdateChildrenFirst) {
            for (auto child : m_Children) {
                child->Update(deltaTime);
            }
        }
        if (m_UpdateComponentsFirst) {
            for (auto component : m_Components) {
                component->OnUpdate(deltaTime);
            }
        }
        OnUpdate(deltaTime);
        if (!m_UpdateComponentsFirst) {
            for (auto component : m_Components) {
                component->OnUpdate(deltaTime);
            }
        }
        if (!m_UpdateChildrenFirst) {
            for (auto child : m_Children) {
                child->Update(deltaTime);
            }
        }
    }

    void Entity::Draw(float deltaTime, const uint32_t& cameraBits)
    {
        ENTITY_EXISTS();
        if (!m_DrawChildrenFirst && (cameraBits & m_RenderFilterBits)){
            OnDraw(deltaTime);
        }
        for (auto child : m_Children) {
            if (child->GetVisible()) {
                child->Draw(deltaTime, cameraBits);
            }
        }
        if (m_DrawChildrenFirst && (cameraBits & m_RenderFilterBits)) {
            OnDraw(deltaTime);
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


    bool Entity::AddComponent(ComponentRef component)
    {
        ENTITY_EXISTS(false);
        if (IsComponent(component)) {
            return false;
        }
        else {
            if (!component->GetParent().lock()) { //no parent. make this parent
                component->SetParent(weak_from_this());
            }
            else if (component->GetParent().lock() != shared_from_this()) { //this is not parent. remove from old.
                component->GetParent().lock()->RemoveComponentByRef(component);
                component->SetParent(weak_from_this());
            }
            //now add to self
            m_Components.push_back(component);
            //event
            ComponentAddedEvent e(weak_from_this(), component);
            ReceiveEvent(e);
            return true;
        }
    }

    bool Entity::IsComponent(ComponentRef ref) const
    {
        ENTITY_EXISTS(false);
        auto iter = std::find(m_Components.begin(), m_Components.end(), ref);
        return iter != m_Components.end();
    }

    ComponentRef Entity::GetFirstComponentOfName(const std::string& name) const
    {
        ENTITY_EXISTS(nullptr);
        for (auto comp : m_Components) {
            if (comp->GetName() == name) {
                return comp;
            }
        }
        return nullptr;
    }

    ComponentRef Entity::RemoveComponentByName(const std::string& name)
    {
        ENTITY_EXISTS(nullptr);
        ComponentRef component = nullptr;
        for (auto comp : m_Components) {
            if (comp->GetName() == name) {
                component = comp;
            }
        }
        if (component) {
            m_Components.remove(component);
            component->SetParent(EntityNoRef());
            ComponentRemovedEvent e(weak_from_this(), component);
            ReceiveEvent(e);
        }
        return component;
    }

    bool Entity::RemoveComponentByRef(ComponentRef ref)
    {
        ENTITY_EXISTS(false);
        if (IsComponent(ref)) {
            m_Components.remove(ref);
            ComponentRemovedEvent e(weak_from_this(), ref);
            ReceiveEvent(e);
            ref->SetParent(EntityNoRef());
            return true;
        }
        else {
            return false;
        }
    }

    bool Entity::MoveToTop()
    {
        EntityRef pp = m_Parent.lock(); 
        if (pp) { 
            return pp->MoveChildUp(shared_from_this(), true); 
        }
        else { 
            return m_OwningLayer.lock()->MoveEntityUp(shared_from_this(), true); 
        }
    }

    inline bool Entity::MoveToBottom()
    {
        EntityRef pp = m_Parent.lock();
        if (pp) {
            return pp->MoveChildDown(shared_from_this(), true);
        }
        else {
            return m_OwningLayer.lock()->MoveEntityDown(shared_from_this(), true);
        }
    }

    inline bool Entity::MoveUp()
    {
        EntityRef pp = m_Parent.lock();
        if (pp) {
            return pp->MoveChildUp(shared_from_this(), false);
        }
        else {
            return m_OwningLayer.lock()->MoveEntityUp(shared_from_this(), false);
        }
    }

    inline bool Entity::MoveDown()
    {
        EntityRef pp = m_Parent.lock();
        if (pp) {
            return pp->MoveChildDown(shared_from_this(), false);
        }
        else {
            return m_OwningLayer.lock()->MoveEntityDown(shared_from_this(), false);
        }
    }

    bool Entity::MoveChildUp(EntityRef child, bool toTop)
    {
        auto f = std::find(m_Children.begin(), m_Children.end(), child);
        if (f == m_Children.end()) {
            //not a child
            return false;
        }
        if (f == std::prev(m_Children.end())) {
            //it is already top
            return true;
        }
        if (toTop) {
            //remove and re-insert at top
            //erase is used so not to search the list again
            m_Children.erase(f);
            m_Children.push_back(child);
        }
        else {
            std::iter_swap(f, std::next(f));
        }
        return true;
    }

    bool Entity::MoveChildDown(EntityRef child, bool toBottom)
    {
        auto f = std::find(m_Children.begin(), m_Children.end(), child);
        if (f == m_Children.end()) {
            //not a child
            return false;
        }
        if (f == m_Children.begin()) {
            //it is already bottom
            return true;
        }
        if (toBottom) {
            //remove and re-insert at top
            //erase is used so not to search the list again
            m_Children.erase(f);
            m_Children.push_front(child);
        }
        else {
            std::iter_swap(std::prev(f), f);
        }
        return true;
    }

    void Entity::MoveListenersUp()
    {
        if (GetListeningForEvents()) {
            ListenForEvents(true);
        }
        for (auto& component : m_Components) {
            if (component->GetListeningForEvents()) {
                component->ListenForEvents(true);
            }
        }
        for (auto& child : m_Children) {
            child->MoveListenersUp();
        }
    }

    

    BoundingBox Entity::GetFullBoundingBox() const
    {
        ENTITY_EXISTS(BoundingBox());
        BoundingBox box = GetSpecificBoundingBox();
        for (auto child : m_Children) {
            box = box + child->GetFullBoundingBox(); //not += because that is not overloaded.
        }
        return box;
    }

    void Entity::ListenForEvents(bool enable)
    {
        ENTITY_EXISTS();
        m_OwningLayer.lock()->EnableListener(weak_from_this(), enable);
        SetListeningForEvents(enable);
    }



 

    void Entity::SetParent(EntityNoRef newParent, bool ignoreChecks)
    {
        ENTITY_EXISTS();
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
        ENTITY_EXISTS();
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
        ENTITY_EXISTS();
        //IF the core AABB of self and other overlap, THEN generate overlap event
        if (GetSpecificBoundingBox().Overlaping(other->GetSpecificBoundingBox())) {
            if (ConfirmOverlap(other) && other->ConfirmOverlap(shared_from_this())) {
                Manifold m(shared_from_this(), other);
                m_OwningLayer.lock()->AddManifoldToQueue(std::move(m));
            }
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


   

    void Entity::GetAllChildrenInBox(const BoundingBox& box, std::list<EntityRef>& list)
    {
        ENTITY_EXISTS();
        for (auto child : m_Children) {
            if (box.Overlaping(child->GetFullBoundingBox())) {
                if (box.Overlaping(child->GetSpecificBoundingBox())) {
                    list.push_back(child);
                }
                child->GetAllChildrenInBox(box, list);
            }
        }
    }

    
    void Entity::GetAllChildrenInRadius(Vector origin, float radius, std::list<EntityRef>& list)
    {
        ENTITY_EXISTS();
        for (auto child : m_Children) {
            if (child->GetFullBoundingBox().OverlappingSphere(origin, radius)) {
                if (child->GetSpecificBoundingBox().OverlappingSphere(origin, radius)) {
                    list.push_back(child);
                }
                child->GetAllChildrenInRadius(origin, radius, list);
            }
        }
    }




    void Entity::RegisterLuaType(sol::state& lua)
    {
        sol::usertype<Entity> type = lua.new_usertype<Entity>("Entity"); //no constructors. for now.
        
        //TODO: make lua-specific versions
        CONNECT_METHOD(Entity, GetSpecificBoundingBox);
        CONNECT_METHOD(Entity, GetFullBoundingBox);

        CONNECT_METHOD_OVERRIDE(Entity, GetRelativeTransform);
        CONNECT_METHOD_OVERRIDE(Entity, GetWorldTransform);
        CONNECT_METHOD_OVERRIDE(Entity, SetRelativeTransform);
        CONNECT_METHOD_OVERRIDE(Entity, SetWorldTransform);
        CONNECT_METHOD_OVERRIDE(Entity, GetRelativePosition);
        CONNECT_METHOD_OVERRIDE(Entity, SetRelativePosition);
        CONNECT_METHOD_OVERRIDE(Entity, GetWorldPosition);
        CONNECT_METHOD_OVERRIDE(Entity, SetWorldPosition);
        CONNECT_METHOD_OVERRIDE(Entity, GetRelativeRotation);
        CONNECT_METHOD_OVERRIDE(Entity, SetRelativeRotation);
        CONNECT_METHOD_OVERRIDE(Entity, GetWorldRotation);
        CONNECT_METHOD_OVERRIDE(Entity, SetWorldRotation);
        CONNECT_METHOD_OVERRIDE(Entity, GetRelativeScale);
        CONNECT_METHOD_OVERRIDE(Entity, SetRelativeScale);
        CONNECT_METHOD_OVERRIDE(Entity, GetWorldScale);
        CONNECT_METHOD_OVERRIDE(Entity, SetWorldScale);


        CONNECT_METHOD(Entity, GetName);
        CONNECT_METHOD(Entity, GetVisible);
        CONNECT_METHOD(Entity, SetVisible);
        CONNECT_METHOD(Entity, IsChild);
        CONNECT_METHOD(Entity, GetFirstChildOfName);
        CONNECT_METHOD(Entity, RemoveChildByName);
        CONNECT_METHOD(Entity, RemoveChildByRef);
        CONNECT_METHOD(Entity, AddChild);
        CONNECT_METHOD(Entity, SwapParent);
        
        CONNECT_METHOD_OVERRIDE(Entity, GetParent);
        
        CONNECT_METHOD(Entity, GetOwningLayer);
        CONNECT_METHOD(Entity, DebugLogAllChildren);
        CONNECT_METHOD(Entity, SetUpdateChildrenFirst);
        CONNECT_METHOD(Entity, GetUpdateChildrenFirst);
        CONNECT_METHOD(Entity, SetUpdateComponentsFirst);
        CONNECT_METHOD(Entity, GetUpdateComponentsFirst);
        CONNECT_METHOD(Entity, SetDrawChildrenFirst);
        CONNECT_METHOD(Entity, GetDrawChildrenFirst);
        CONNECT_METHOD(Entity, AddComponent);
        CONNECT_METHOD(Entity, IsComponent);
        CONNECT_METHOD(Entity, GetFirstComponentOfName);
        CONNECT_METHOD(Entity, RemoveComponentByName);
        CONNECT_METHOD(Entity, RemoveComponentByRef);
        CONNECT_METHOD(Entity, Destroy);
        CONNECT_METHOD(Entity, ListenForEvents);

        CONNECT_METHOD(Entity, GetRenderFilterBits);
        CONNECT_METHOD(Entity, SetRenderFilterBits);
    }

}

#pragma warning( pop )
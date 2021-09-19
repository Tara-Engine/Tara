#include "tarapch.h"
#include "Entity.h"
#include "Tara/Core/Layer.h"

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



}
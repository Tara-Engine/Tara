#include "tarapch.h"
#include "ClickableComponent.h"
#include "Tara/Core/Layer.h"
#include "Tara/Input/Input.h"
#include "Tara/Input/ApplicationEvents.h"

namespace Tara{
	ClickableComponent::ClickableComponent(EntityNoRef parent, const std::string& name)
		: Component(parent, name), m_IsDownOverMe(false), m_IsHovering(false), m_IsDragging(false),
		m_DragStartDist(5), m_DragOriginCache(0,0), m_Camera(CameraEntityNoRef())
	{
	}

	CameraEntityNoRef ClickableComponent::GetCamera() const
	{
		CameraEntityRef camEntity = m_Camera.lock();
		if (camEntity) {
			return camEntity;
		}
		else {
			auto parent = GetParent().lock();
			if (parent) {
				camEntity = parent->GetOwningLayer().lock()->GetLayerCamera().lock();
				if (camEntity) {
					return camEntity;
				}
			}
		}
		return CameraEntityNoRef();
	}

	void ClickableComponent::OnBeginPlay()
	{
		//this entity should listen for events
		ListenForEvents(true);
	}

	void ClickableComponent::OnUpdate(float deltaTime)
	{
		auto screenPos = Input::Get()->GetMousePos();
		auto worldPos = GetMousePos(screenPos);
		if (!m_IsDownOverMe && !m_IsDragging) {
			if (IsInOwner(worldPos)) {
				m_IsHovering = true;
				HoverEvent e(screenPos.x, screenPos.y, true);
				GetParent().lock()->ReceiveEvent(e);
			}
			else {
				if (m_IsHovering) {
					HoverEvent e(screenPos.x, screenPos.y, false);
					GetParent().lock()->ReceiveEvent(e);
					m_IsHovering = false;
				}
			}
		}
		else {
			if (!m_IsDragging) {
				auto tween = screenPos - m_DragOriginCache;
				auto dist = sqrtf(tween.x * tween.x + tween.y * tween.y);
				
				if (dist >= m_DragStartDist) {
					m_IsDragging = true;
					//LOG_S(INFO) << "Dragging started!";
					//send start event
					DragEvent e(m_DragOriginCache.x, m_DragOriginCache.y, DragEvent::DragType::BEGIN);
					GetParent().lock()->ReceiveEvent(e);
					//send update event
					DragEvent c(screenPos.x, screenPos.y, DragEvent::DragType::CONTINUE);
					GetParent().lock()->ReceiveEvent(c);
				}
				
				if (!IsInOwner(worldPos)) {
					//unclick nicely
					ClickEvent e(screenPos.x, screenPos.y, 0, false, true);
					GetParent().lock()->ReceiveEvent(e);
					m_IsDownOverMe = false;
				}
			}
			else {
				//send a drag contiue event
				//LOG_S(INFO) << "Dragging Continued";
				DragEvent e(screenPos.x, screenPos.y, DragEvent::DragType::CONTINUE);
				GetParent().lock()->ReceiveEvent(e);
			}

			
		}
	}

	void ClickableComponent::OnEvent(Event& e)
	{
		EventFilter filter(e);
		filter.Call<MouseButtonPressEvent>(TARA_BIND_FN(ClickableComponent::OnMouseButtonPressEvent));
		filter.Call<MouseButtonReleaseEvent>(TARA_BIND_FN(ClickableComponent::OnMouseButtonReleaseEvent));
		//filter.Call<MouseMoveEvent>(TARA_BIND_FN(ClickableComponent::OnMouseMoveEvent));
	}


	bool ClickableComponent::OnMouseButtonPressEvent(MouseButtonPressEvent& e)
	{
		auto screenPos = Input::Get()->GetMousePos();
		auto worldPos = GetMousePos(screenPos);
		if (IsInOwner(worldPos)) {
			m_IsDownOverMe = true;
			m_DragOriginCache = screenPos;
			ClickEvent event(screenPos.x, screenPos.y, e.GetButton(), false, false);
			GetParent().lock()->ReceiveEvent(event);
			return event.Handled();
		}
		return false;
	}


	bool ClickableComponent::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e)
	{
		auto screenPos = Input::Get()->GetMousePos();
		bool rval = false;
		if (m_IsDownOverMe) {
			ClickEvent event(screenPos.x, screenPos.y, e.GetButton(), true, false);
			GetParent().lock()->ReceiveEvent(event);
			m_IsDownOverMe = false;
			rval = rval | event.Handled();
		}
		if (m_IsDragging) {
			//LOG_S(INFO) << "Dragging Ended";
			m_IsDragging = false;
			//send drag end event
			DragEvent event(screenPos.x, screenPos.y, DragEvent::DragType::END);
			GetParent().lock()->ReceiveEvent(event);
			rval = rval | event.Handled();
		}
		return rval;
	}



	Vector ClickableComponent::GetMousePos(glm::vec2 mousePos)
	{
		CameraRef cam;
		CameraEntityRef camEntity = m_Camera.lock();
		if (camEntity) {
			cam = camEntity->GetCamera();
		}
		else {
			auto parent = GetParent().lock();
			if (parent) {
				camEntity = parent->GetOwningLayer().lock()->GetLayerCamera().lock();
				if (camEntity) {
					cam = camEntity->GetCamera();
				}
			}
		}
		if (cam) {
			auto ray = cam->GetRayFromScreenCoordinate(mousePos.x, mousePos.y);
			return ray.first;
		}
		else {
			return Vector(0,0,0);
		}
	}

	bool ClickableComponent::IsInOwner(Vector pos)
	{
		auto bb = GetParent().lock()->GetSpecificBoundingBox();
		return (
			pos.x >= bb.x && pos.x < bb.x + bb.Width&&
			pos.y >= bb.y && pos.y < bb.y + bb.Height
			//pos.z >= bb.z && pos.z < bb.z + bb.Depth
			);
	}
}
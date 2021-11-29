#include "tarapch.h"
#include "ClickableComponent.h"
#include "Tara/Core/Layer.h"
#include "Tara/Input/Input.h"
#include "Tara/Input/ApplicationEvents.h"

namespace Tara{
	ClickableComponent::ClickableComponent(EntityNoRef parent, const std::string& name)
		: Component(parent, name)
	{
	}

	void ClickableComponent::OnBeginPlay()
	{
		//this entity should listen for events
		ListenForEvents(true);
	}

	void ClickableComponent::OnEvent(Event& e)
	{
		EventFilter filter(e);
		filter.Call<MouseButtonPressEvent>(TARA_BIND_FN(ClickableComponent::OnMouseButtonPressEvent));
		filter.Call<MouseButtonReleaseEvent>(TARA_BIND_FN(ClickableComponent::OnMouseButtonReleaseEvent));
		filter.Call<MouseMoveEvent>(TARA_BIND_FN(ClickableComponent::OnMouseMoveEvent));
	}


	bool ClickableComponent::OnMouseButtonPressEvent(MouseButtonPressEvent& e)
	{
		auto screenPos = Input::Get()->GetMousePos();
		auto worldPos = GetMousePos(screenPos);
		if (IsInOwner(worldPos)) {
			m_IsDownOverMe = true;
			ClickEvent e(screenPos.x, screenPos.y, e.GetButton());
			GetParent().lock()->ReceiveEvent(e);
		}
		return false;
	}


	bool ClickableComponent::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e)
	{
		return false;
	}


	bool ClickableComponent::OnMouseMoveEvent(MouseMoveEvent& e)
	{
		
		return false;
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
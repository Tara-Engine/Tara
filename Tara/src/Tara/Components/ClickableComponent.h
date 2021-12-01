#pragma once
#include "Tara/Core/Component.h"
#include "Tara/Entities/CameraEntity.h"

namespace Tara {

	REFTYPE(ClickableComponent)
	NOREFTYPE(ClickableComponent)

	/// <summary>
	/// ClickableComponent is a component that allows for detection if the parent entity was clicked on. Its basically Create-And-Forget.
	/// The parent entity will receive OnClick events, even if not Listening for Window events.
	/// </summary>
	class ClickableComponent : public Component {
	public:
		ClickableComponent(EntityNoRef parent, const std::string& name = "ClickableComponent");

		virtual ~ClickableComponent() {}

		/// <summary>
		/// Set the camera to use for mouse position calculations
		/// </summary>
		/// <param name="camera"></param>
		inline void SetCamera(CameraEntityNoRef camera) { m_Camera = camera; }

	public:
		//overrides
		virtual void OnBeginPlay() override;

		virtual void OnUpdate(float deltaTime) override;
		
		virtual void OnEvent(Event& e) override;

	private:
		
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& e);
		
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e);
		
		bool OnMouseMoveEvent(MouseMoveEvent& e);

		Vector GetMousePos(glm::vec2 mousePos);

		bool IsInOwner(Vector pos);

	private:
		bool m_IsDownOverMe;
		bool m_IsHovering;
		CameraEntityNoRef m_Camera;
	};

}
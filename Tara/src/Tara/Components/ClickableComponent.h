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

		/// <summary>
		/// Get the camera being used. If none set, returns layer camera. If that is not set, returns nullptr
		/// </summary>
		/// <returns></returns>
		CameraEntityNoRef GetCamera() const;

		/// <summary>
		/// Set the distance (in screen units) that the mouse has to travel while down to start drag and drop events
		/// </summary>
		/// <param name="dist"></param>
		inline void SetDragStartDistance(float dist) { m_DragStartDist = dist; }

		/// <summary>
		/// Get the distance (in screen units) that the mouse has to travel while down to start drag and drop events
		/// </summary>
		/// <returns></returns>
		inline float GetDragStartDistance() const { return m_DragStartDist; }

	public:
		//overrides
		virtual void OnBeginPlay() override;

		virtual void OnUpdate(float deltaTime) override;
		
		virtual void OnEvent(Event& e) override;

	private:
		
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& e);
		
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e);
		
		//bool OnMouseMoveEvent(MouseMoveEvent& e);

		Vector GetMousePos(glm::vec2 mousePos);

		bool IsInOwner(Vector pos);

	private:
		bool m_IsDownOverMe;
		bool m_IsHovering;
		bool m_IsDragging;
		float m_DragStartDist;
		glm::vec2 m_DragOriginCache;
		CameraEntityNoRef m_Camera;
	};

}
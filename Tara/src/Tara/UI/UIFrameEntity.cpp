#include "tarapch.h"
#include "UIFrameEntity.h"
#include "Tara/Components/ClickableComponent.h"
#include "Tara/Input/ApplicationEvents.h"

namespace Tara {
	UIFrameEntity::UIFrameEntity(EntityNoRef parent, LayerNoRef owningLayer, PatchRef patch, float headerPixels, const std::string& name)
		: UIVisualEntity(parent, owningLayer, patch, name), m_Size(0,0), m_HeaderSize(headerPixels), m_Dragging(false), m_DragOrigin(0,0)
	{
		SetBorderFromPatch();
		SetSnapRules(UISnapRule::TOP | UISnapRule::LEFT);
	}

	void UIFrameEntity::CalculateDesiredSize()
	{
		UIVisualEntity::CalculateDesiredSize();
		auto desired = GetDesiredSize();
		desired.x = std::max(desired.x, m_Size.x);
		desired.y = std::max(desired.y, m_Size.y);
		SetDesiredSize(desired);
		m_Size = desired; //update our size to match desired size too
	}

	void UIFrameEntity::OnBeginPlay()
	{
		//make a clickable component to handle click detection for the button
		CreateComponent<ClickableComponent>(weak_from_this(), "ButtonClickableComponent");
		SetUpdateChildrenFirst(false);
	}

	void UIFrameEntity::OnEvent(Event& e)
	{
		EventFilter filter(e);
		filter.Call<DragEvent>(TARA_BIND_FN(UIFrameEntity::OnDragEvent));
	}

	void UIFrameEntity::OnDraw(float deltaTime)
	{
		UIVisualEntity::OnDraw(deltaTime);

		//if extra drawing stuff needed, do it here

	}

	bool UIFrameEntity::OnDragEvent(DragEvent& e)
	{
		auto cmp = GetFirstCompontentOfType<ClickableComponent>();
		if (!cmp) { return false; }
		CameraEntityRef camEntity = cmp->GetCamera().lock();
		if (!camEntity) {return false;}
		CameraRef cam = camEntity->GetCamera();
		if (!cam) { return false; }

		auto worldCoord = cam->GetRayFromScreenCoordinate(e.GetMouseX(), e.GetMouseY()).first;


		switch (e.GetDragType()) {
		case DragEvent::DragType::BEGIN: {
			auto drawArea = GetRenderArea();
			float off = drawArea.y2 - worldCoord.y;
			if (off <= m_HeaderSize) {
				//start dragging
				m_Dragging = true;
				auto offset = GetOffsets();
				m_DragOrigin = glm::vec2(worldCoord.x, worldCoord.y) - glm::vec2(drawArea.x1, drawArea.y2);
				m_DragOrigin.x *= -1;
				return true;
			}

			break;
		}
		case DragEvent::DragType::CONTINUE: {
			if (m_Dragging){
				//auto drawArea = GetRenderArea();
				glm::vec2 pos = e.GetMousePos() + m_DragOrigin;
				auto offset = GetOffsets();
				SetOffsets(pos.x, offset.y, pos.y, offset.w);
				return true;
			}
			break;
		}
		case DragEvent::DragType::END: {
			if (m_Dragging) {
				m_Dragging = false;
				return true;
			}
			break;
		}
		}
		return false;
	}

}


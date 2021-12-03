#include "tarapch.h"
#include "UIFrameEntity.h"
#include "Tara/Components/ClickableComponent.h"
#include "Tara/Input/ApplicationEvents.h"
#include "Tara/Renderer/Renderer.h"

#define RESIZE_SIDE_LEFT	0
#define RESIZE_SIDE_RIGHT	1
#define RESIZE_SIDE_TOP		2
#define RESIZE_SIDE_BOTTOM	3

namespace Tara {
	UIFrameEntity::UIFrameEntity(EntityNoRef parent, LayerNoRef owningLayer, PatchRef patch, float headerPixels, const std::string& name)
		: UIVisualEntity(parent, owningLayer, patch, name), m_Size(0,0), m_HeaderSize(headerPixels),
		m_ResizeHandleSize(10), m_ResizingSide(0), m_Dragging(false), m_Resizing(false), m_DragData(nullptr), m_DrawResizeBar(false)
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
		
	}

	void UIFrameEntity::OnEvent(Event& e)
	{
		EventFilter filter(e);
		filter.Call<DragEvent>(TARA_BIND_FN(UIFrameEntity::OnDragEvent));
		filter.Call<HoverEvent>(TARA_BIND_FN(UIFrameEntity::OnHoverEvent));
	}

	void UIFrameEntity::OnDraw(float deltaTime)
	{
		UIVisualEntity::OnDraw(deltaTime);
		
		if (m_Resizing || m_DrawResizeBar) {
			auto d = GetRenderArea();
			switch (m_ResizingSide) {
			case RESIZE_SIDE_LEFT: {
				Renderer::Quad(TRANSFORM_2D(d.x1, d.y1, 0, m_ResizeHandleSize, d.Height()), glm::vec4{ 0,0,0,0.5 });
				break;
			}
			case RESIZE_SIDE_RIGHT: {
				Renderer::Quad(TRANSFORM_2D(d.x2 - m_ResizeHandleSize, d.y1, 0, m_ResizeHandleSize, d.Height()), glm::vec4{ 0,0,0,0.5 });
				break;
			}
			case RESIZE_SIDE_TOP: {
				Renderer::Quad(TRANSFORM_2D(d.x1, d.y1, 0, d.Width(), m_ResizeHandleSize), glm::vec4{ 0,0,0,0.5 });
				break;
			}
			case RESIZE_SIDE_BOTTOM: {
				Renderer::Quad(TRANSFORM_2D(d.x1, d.y2 - m_ResizeHandleSize, 0, d.Width(), m_ResizeHandleSize), glm::vec4{ 0,0,0,0.5 });
				break;
			}
			default:{}
			}
			m_DrawResizeBar = false;
		}
		//
		//LOG_S(INFO) << "POSITION: " << d.y2;
		//if extra drawing stuff needed, do it here

	}

	bool UIFrameEntity::OnDragEvent(DragEvent& e)
	{
		auto worldCoord = e.GetMousePos();

		switch (e.GetDragType()) {
		case DragEvent::DragType::BEGIN: {
			bool setDragOrigin = false;
			auto drawArea = GetRenderArea();
			float offLeft = -(drawArea.x1 - worldCoord.x);
			float offRight = drawArea.x2 - worldCoord.x;
			float offTop = -(drawArea.y1 - worldCoord.y);
			float offBottom = drawArea.y2 - worldCoord.y;
			
			//check for resizing graps
			if (offLeft <= m_ResizeHandleSize) {
				m_Resizing = true;
				m_ResizingSide = RESIZE_SIDE_LEFT;
				setDragOrigin = true;
			}
			if (offRight <= m_ResizeHandleSize) {
				m_Resizing = true;
				m_ResizingSide = RESIZE_SIDE_RIGHT;
				setDragOrigin = true;
			}
			if (offTop <= m_ResizeHandleSize) {
				m_Resizing = true;
				m_ResizingSide = RESIZE_SIDE_TOP;
				setDragOrigin = true;
			}
			if (offBottom <= m_ResizeHandleSize) {
				m_Resizing = true;
				m_ResizingSide = RESIZE_SIDE_BOTTOM;
				setDragOrigin = true;
			}

			//check for moving grab
			if (offTop <= m_HeaderSize && offTop > m_ResizeHandleSize) {
				//start dragging
				m_Dragging = true;
				setDragOrigin = true;
			}

			if (setDragOrigin) {
				auto offset = GetOffsets();
				auto mouseOffsetCorner = glm::vec2(worldCoord.x, worldCoord.y) - glm::vec2(drawArea.x1, drawArea.y1);
				mouseOffsetCorner *= -1;
				
				m_DragData = new DragData{
					worldCoord,			//mouse pos
					GetSize(),			//size
					GetOffsets()		//offset
				};
				//LOG_S(INFO) << "MousePosition: " << m_DragData->MousePositionWorld;
				//LOG_S(INFO) << "FrameSize: " << m_DragData->FrameSize;
				//LOG_S(INFO) << "FrameOffsets: " << m_DragData->FrameOffsets;

				return true;
			}

			break;
		}
		case DragEvent::DragType::CONTINUE: {
			if (m_DragData == nullptr) {
				return false;
			}

			if (m_Resizing) {
				switch (m_ResizingSide) {
				case RESIZE_SIDE_LEFT: {
					//here we need to set both the width and the offset X1. 
					
					//change in X
					auto delta = e.GetMouseX() - m_DragData->MousePosition.x;

					auto size = GetSize();
					//size is change + original size
					size.x = (-delta) + m_DragData->FrameSize.x;
					SetSize(size);

					//set the offset
					auto offset = GetOffsets();
					SetOffsets(delta + m_DragData->FrameOffsets.x, offset.y, offset.z, offset.w);
					
					return true;
				}
				case RESIZE_SIDE_RIGHT: {
					//here, we need to set the width to be whatever it was
					//get the mouse location (window coords because we don't care about Y)
					
					//change in X
					auto delta = e.GetMouseX() - m_DragData->MousePosition.x;
					
					auto size = GetSize();
					//size is change + original size
					size.x = delta + m_DragData->FrameSize.x;
					SetSize(size);
					return true;
				}

				case RESIZE_SIDE_TOP: {
					//here we need to set both the width and the offset X1. 
					//get the mouse location (window coords because we don't care about Y)
					auto delta = e.GetMouseY() - m_DragData->MousePosition.y;

					auto size = GetSize();
					//size is change + original size
					size.y = (-delta) + m_DragData->FrameSize.y;
					SetSize(size);

					//set the offset
					auto offset = GetOffsets();
					SetOffsets(offset.x, offset.y, delta + m_DragData->FrameOffsets.z, offset.w);

					return true;
				}

				case RESIZE_SIDE_BOTTOM: {
					//here, we need to set the width to be whatever it was
					//get the mouse location (window coords because we don't care about Y
					auto delta = e.GetMouseY() - m_DragData->MousePosition.y;
					
					//mouse is now relative to upper left corner of frame
					auto size = GetSize();
					size.y = delta + m_DragData->FrameSize.y;
					SetSize(size);
					return true;
				}

				default: {
					LOG_S(ERROR) << "ERROR: m_ResizingSide is not one of the defined sides, something strage is happening!"; 
					return false; 
				}
				}


			}

			
			if (m_Dragging){
				
				auto pos = e.GetMousePos() - m_DragData->MousePosition + glm::vec2(m_DragData->FrameOffsets.x, m_DragData->FrameOffsets.z);
				//auto allowed = GetAllowedArea();
				//glm::vec2 pos = e.GetMousePos() + m_DragData->MouseOffsetCorner - glm::vec2(allowed.x1, allowed.y1);;
				//pos.y += m_DragData->FrameSize.y + m_DragData->MouseOffsetCorner.y;
				
				//LOG_S(INFO) << "off2:" << (m_DragData->FrameLocationUpperLeft.y + m_DragData->FrameSize.y) - m_DragData->MousePositionScreen.y;
				auto offset = GetOffsets();
				SetOffsets(pos.x, offset.y, pos.y, offset.w);
				return true;
			}
			break;
		}
		case DragEvent::DragType::END: {
			if (m_Dragging) {
				m_Dragging = false;
				delete m_DragData;
				m_DragData = nullptr;
				return true;
			}
			if (m_Resizing) {
				m_Resizing = false;
				delete m_DragData;
				m_DragData = nullptr;
				return true;
			}
			break;
		}
		}
		return false;
	}

	bool UIFrameEntity::OnHoverEvent(HoverEvent& e)
	{
		if (!m_Dragging){
			auto worldCoord = e.GetMousePos();

			auto drawArea = GetRenderArea();
			float offLeft = -(drawArea.x1 - worldCoord.x);
			float offRight = drawArea.x2 - worldCoord.x;
			float offTop = -(drawArea.y1 - worldCoord.y);
			float offBottom = drawArea.y2 - worldCoord.y;
			
			//check for resizing graps
			if (offBottom <= m_ResizeHandleSize) {
				m_DrawResizeBar = true;
				m_ResizingSide = RESIZE_SIDE_BOTTOM;
			}
			else if (offTop <= m_ResizeHandleSize) {
				m_DrawResizeBar = true;
				m_ResizingSide = RESIZE_SIDE_TOP;
			}
			else if (offRight <= m_ResizeHandleSize) {
				m_DrawResizeBar = true;
				m_ResizingSide = RESIZE_SIDE_RIGHT;
			}
			else if (offLeft <= m_ResizeHandleSize) {
				m_DrawResizeBar = true;
				m_ResizingSide = RESIZE_SIDE_LEFT;
			}
			else {
				m_DrawResizeBar = false;
			}
			
			
		}
		return false;
	}


	glm::vec2 UIFrameEntity::GetFrameCornerUpperLeft()
	{
		auto ra = GetRenderArea();
		return glm::vec2(ra.x1, ra.y1);
	}

}


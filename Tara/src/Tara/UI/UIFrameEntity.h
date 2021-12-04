#pragma once
#include "Tara/UI/UIVisualEntity.h"

namespace Tara {
	REFTYPE(UIFrameEntity);
	NOREFTYPE(UIFrameEntity);

	class DragEvent;
	class HoverEvent;
	class ClickEvent;

	class UIFrameEntity : public UIVisualEntity {
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="parent"></param>
		/// <param name="owningLayer"></param>
		/// <param name="patch"></param>
		/// <param name="name"></param>
		UIFrameEntity(EntityNoRef parent, LayerNoRef owningLayer, PatchRef patch, float headerPixels, const std::string& name = "UIFrameEntity");
		
		virtual ~UIFrameEntity() = default;

		/// <summary>
		/// Set the size of the frame
		/// </summary>
		/// <param name="size"></param>
		inline void SetSize(const glm::vec2& size) { m_Size = size; m_DesiredSizeDirty = true;}

		/// <summary>
		/// Set the size of the frame
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		inline void SetSize(float x, float y) { SetSize({ x,y }); }

		/// <summary>
		/// Get the size of the frame
		/// </summary>
		/// <returns></returns>
		inline const glm::vec2& GetSize() const { return m_Size; }

	public:
		//overrides
		virtual void CalculateDesiredSize() override;

		virtual void OnBeginPlay() override;
		
		virtual void OnEvent(Event& e) override;

		virtual void OnDraw(float deltaTime) override;

	private:
		//Events

		bool OnDragEvent(DragEvent& e);

		bool OnHoverEvent(HoverEvent& e);

		bool OnClickEvent(ClickEvent& e);

	private:
		//helper methods

		glm::vec2 GetFrameCornerUpperLeft();

	private:
		struct DragData {
			//inverted distance from upper-left corner of widget
			glm::vec2 MousePosition;
			glm::vec2 FrameSize;
			glm::vec4 FrameOffsets;
		};

	private:
		glm::vec2 m_Size;
		float m_HeaderSize;
		float m_ResizeHandleSize;

		DragData* m_DragData;
		//glm::vec2 m_DragOrigin;
		
		uint8_t m_ResizingSide;
		bool m_Dragging;
		bool m_Resizing; 
		bool m_DrawResizeBar;
	};
}
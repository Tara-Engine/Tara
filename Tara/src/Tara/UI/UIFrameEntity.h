#pragma once
#include "Tara/UI/UIVisualEntity.h"

namespace Tara {
	REFTYPE(UIFrameEntity);
	NOREFTYPE(UIFrameEntity);

	class DragEvent;

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
		inline void SetSize(const glm::vec2& size) { m_Size = size; }

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

	private:
		glm::vec2 m_Size;
		float m_HeaderSize;
		glm::vec2 m_DragOrigin;
		bool m_Dragging;
	};
}
#include "tarapch.h"
#include "UIVisualEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{

	UIVisualEntity::UIVisualEntity(EntityNoRef parent, LayerNoRef owningLayer, PatchRef patch, const std::string& name)
		: UIBaseEntity(parent, owningLayer, name), m_Patch(patch), m_Tint(1)
	{}

	void UIVisualEntity::SetBorderFromPatch()
	{
		auto pixels = m_Patch->GetBorderPixels();
		SetBorder((float)std::get<0>(pixels), (float)std::get<1>(pixels), (float)std::get<2>(pixels), (float)std::get<3>(pixels));
	}
	/*
	void UIVisualEntity::CalculateDesiredSize()
	{

		//for the base entity, the desired size is that of the largest 
		//child for x and y independantly
		auto& children = GetChildren();
		glm::vec2 size{ 0,0 };
		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				auto childDesiredSize = asUI->GetDesiredSize();
				size.x = std::max(size.x, childDesiredSize.x);
				size.y = std::max(size.y, childDesiredSize.y);
			}
		}
		//add the border on, as that will take space too
		//also add the frame of the patch.
		auto pixels = m_Patch->GetBorderPixels();
		auto border = GetBorder();
		size.x += border.x + border.y + std::get<0>(pixels) + std::get<1>(pixels);
		size.y += border.z + border.w + std::get<2>(pixels) + std::get<3>(pixels);
		//set the size
		SetDesiredSize(size);
	}
	*/

	/*
	void UIVisualEntity::OnUpdate(float deltaTime)
	{
		//the idea here is that the Allowed area of all the children should be updated every frame

		if (m_DesiredSizeDirty) {
			CalculateDesiredSize();
			m_DesiredSizeDirty = false;
		}

		//get the allowed area for this entity, and remove the border from it
		UIBox allowed = GetRenderArea();
		glm::vec4 border = GetBorder();
		auto pixels = m_Patch->GetBorderPixels();
		allowed.x1 += border.x + std::get<0>(pixels); //left
		allowed.x2 -= border.y + std::get<1>(pixels); //right
		allowed.y2 -= border.z + std::get<2>(pixels); //top
		allowed.y1 += border.w + std::get<3>(pixels); //bottom

		auto& children = GetChildren();
		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				//normally, this would be needed, but we are ignoring it.
				//Other sublcasses of UIBaseEntity will probably override OnUpdate
				//themselves, for the same reason, as they want to do it differently. EX, a UIVerticalListEntity

				//auto childDesiredSize = asUI->GetDesiredSize();

				//just set the allowed area. Children of UIBase Entity are allowed to overlap
				asUI->SetAllowedArea(allowed);
			}
		}
		
	}
	*/

	void UIVisualEntity::OnDraw(float deltaTime)
	{

		auto target = GetRenderArea();

		//TEMP just fill allowed area
		Renderer::Patch(target.ToTransform(), m_Patch, m_Tint);
	}


}

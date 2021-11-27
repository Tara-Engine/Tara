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

	void UIVisualEntity::OnDraw(float deltaTime)
	{

		auto target = GetRenderArea();

		//TEMP just fill allowed area
		Renderer::Patch(target.ToTransform(), m_Patch, m_Tint);
	}


}

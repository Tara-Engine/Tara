#include "tarapch.h"
#include "UITextEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{

	UITextEntity::UITextEntity(EntityNoRef parent, LayerNoRef owningLayer, FontRef font, const std::string& name)
		:UIBaseEntity(parent, owningLayer, name), m_Font(font), m_Color(1), 
		m_Text(name), m_TextMeshSize(0), m_TextSize(16), m_VerticalOffset(0), m_TextDirty(true)
	{
		UpdateDisplayString();
	}

	void UITextEntity::CalculateDesiredSize()
	{
		UIBaseEntity::CalculateDesiredSize();
		auto desired = GetDesiredSize();
		desired.x = std::max(desired.x, m_TextMeshSize.x);
		desired.y = std::max(desired.y, m_TextMeshSize.y);
		SetDesiredSize(desired);
	}

	void UITextEntity::OnDraw(float deltaTime)
	{
		auto target = GetRenderArea();

		Transform t{ Vector(target.x1, target.y2 - m_VerticalOffset, 0), Rotator(0,0,0), Vector(m_TextSize, -m_TextSize, 1) };

		auto& tex = m_Font->GetTexture(); //cache of texture. audo does deduce const, but not &
		//for each character, draw its font. Don't use m_Text.size() because there may be more transforms than in the original text. using \t is an example of this
		for (int i = 0; i < m_CachedTrasforms.size(); i++) {
			if (m_CachedMinUVs[i] == glm::vec2(0, 0) && m_CachedMaxUVs[i] == glm::vec2(0, 0)) {
				//there is more allocated cells than displayed characters.
				break;
			}
			Renderer::Quad(t + m_CachedTrasforms[i], m_Color, tex, m_CachedMinUVs[i], m_CachedMaxUVs[i]);
		}
	}

	void UITextEntity::OnUpdate(float deltaTime)
	{
		if (m_TextDirty) {
			UpdateDisplayString();
		}
		UIBaseEntity::OnUpdate(deltaTime);
	}


	void UITextEntity::UpdateDisplayString()
	{
		m_Font->GetTextQuads(m_Text, m_CachedTrasforms, m_CachedMinUVs, m_CachedMaxUVs);
		auto target = GetRenderArea();
		Transform t{ Vector(target.x1, target.y1, 0), Rotator(0,0,0), Vector(m_TextSize, m_TextSize, 1) };
		if (m_CachedTrasforms.size() > 0){
			glm::vec2 minPos{ (t+m_CachedTrasforms[0]).Position.x, (t + m_CachedTrasforms[0]).Position.y };
			glm::vec2 maxPos{ minPos.x, minPos.y };
			m_VerticalOffset = 0;
			for (auto& cacheT : m_CachedTrasforms) {
				Transform f = t + cacheT;
				//the transforms given from the font system can have negative Scale.y values
				minPos.x = std::min(f.Position.x, minPos.x);
				minPos.y = std::min(f.Position.y + f.Scale.y, minPos.y);
				minPos.y = std::min(f.Position.y, minPos.y);
				
				m_VerticalOffset = std::min(m_VerticalOffset, (cacheT.Position.y + cacheT.Scale.y) * (m_TextSize));

				maxPos.x = std::max(f.Position.x + f.Scale.x, maxPos.x);
				maxPos.y = std::max(f.Position.y + f.Scale.y, maxPos.y);
				maxPos.y = std::max(f.Position.y, maxPos.y);
			}
			m_TextMeshSize = glm::vec2{
				maxPos.x - minPos.x,
				(maxPos.y - minPos.y) + m_VerticalOffset
			};
			m_VerticalOffset = -m_VerticalOffset;
			//LOG_S(INFO) << m_VerticalOffset;
		}
		else {
			m_TextMeshSize = glm::vec2{0,0};
			m_VerticalOffset = 0;
		}
		m_TextDirty = false;
		m_DesiredSizeDirty = true;
	}


}

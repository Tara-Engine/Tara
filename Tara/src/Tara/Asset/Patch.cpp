#include "tarapch.h"
#include "Patch.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara{

	Patch::Patch(const Texture2DRef& texture, const std::string& name)
		: Asset(name), m_Texture(texture),
		m_BorderLeft(0), m_BorderRight(0),
		m_BorderTop(0), m_BorderBottom(0)
	{}

	PatchRef Patch::Create(const Texture2DRef & texture, const std::string & name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<Patch>(name);
		if (ref == nullptr) {
			ref = std::make_shared<Patch>(texture, name);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}

	void Patch::SetBorderPercent(float left, float right, float top, float bottom)
	{
		if (left >= 0.0f && right >= 0.0f && left + right <= 1.0f) {
			m_BorderLeft = left;
			m_BorderRight = right;
		}
		else {
			LOG_S(ERROR) << "Attempted to set Patch border (left-right) in such a way the border took more than the full image";
		}
		if (top >= 0.0f && bottom >= 0.0f && top + bottom <= 1.0f) {
			m_BorderTop = top;
			m_BorderBottom = bottom;
		}
		else {
			LOG_S(ERROR) << "Attempted to set Patch border (top-bottom) in such a way the border took more than the full image";
		}
	}

	void Patch::SetBorderPixels(int32_t left, int32_t right, int32_t top, int32_t bottom)
	{
		int32_t textureWidth = m_Texture->GetWidth();
		int32_t textureHeight = m_Texture->GetHeight();
		if (left >= 0 && right >= 0 && left + right <= textureWidth) {
			m_BorderLeft = (float)left / (float)textureWidth;
			m_BorderRight = (float)right / (float)textureWidth;
		}
		else {
			LOG_S(ERROR) << "Attempted to set Patch border (left-right) in such a way the border took more than the full image";
		}

		if (top >= 0 && bottom >= 0 && top + bottom <= textureHeight) {
			m_BorderTop = (float)top / (float)textureHeight;
			m_BorderBottom = (float)bottom / (float)textureHeight;
		}
		else {
			LOG_S(ERROR) << "Attempted to set Patch border (top-bottom) in such a way the border took more than the full image";
		}
	}

	std::tuple<float, float, float, float> Patch::GetBorderPercent() const
	{
		return std::make_tuple(m_BorderLeft, m_BorderRight, m_BorderTop, m_BorderBottom);
	}

	std::tuple<int32_t, int32_t, int32_t, int32_t> Patch::GetBorderPixels() const
	{
		int32_t textureWidth = m_Texture->GetWidth();
		int32_t textureHeight = m_Texture->GetHeight();
		return std::make_tuple(
			(int32_t)floorf(m_BorderLeft * textureWidth),
			(int32_t)floorf(m_BorderRight * textureWidth),
			(int32_t)floorf(m_BorderTop * textureHeight),
			(int32_t)floorf(m_BorderBottom * textureHeight)
		);
	}

	std::pair<glm::vec2, glm::vec2> Patch::GetMiddleUVs() const
	{
		return std::make_pair(
			glm::vec2(m_BorderLeft, m_BorderBottom), 
			glm::vec2(1.0f-m_BorderRight, 1.0f-m_BorderTop)
		);
	}

	std::pair<glm::vec2, glm::vec2> Patch::GetMiddleOffsets(glm::vec2 scale) const
	{
		glm::vec2 texSize(m_Texture->GetWidth(), m_Texture->GetHeight());

		//scale normalized to the size of the texture being 0 to 1
		glm::vec2 nScale = scale / texSize;


		auto ret = std::make_pair(
			glm::vec2(m_BorderLeft, m_BorderBottom) * texSize,
			glm::vec2(nScale.x - m_BorderRight, nScale.y - m_BorderTop) * texSize
		);
		//if (scale.x < 0) {
		//	ret.first.x = (scale.x) + ret.first.x;
		//	ret.second.x = (scale.x) + ret.second.x;
		//}
		//if (scale.y < 0) {
		//	ret.first.y = (scale.y) + ret.first.y;
		//	ret.second.y = (scale.y) + ret.second.y;
		//}


		return ret;
	}




}
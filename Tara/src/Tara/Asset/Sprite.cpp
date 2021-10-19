#include "tarapch.h"
#include "Sprite.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara {
	
	Sprite::Sprite(const Texture2DRef& texture, uint32_t xFrameCount, uint32_t yFrameCount, const std::string& name)
		: Asset(name), m_Texture(texture), m_XFrameCount(xFrameCount), m_YFrameCount(yFrameCount)
	{
	}

	SpriteRef Sprite::Create(const Texture2DRef& texture, uint32_t xFrameCount, uint32_t yFrameCount, const std::string& name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<Sprite>(name);
		if (ref == nullptr) {
			ref = std::make_shared<Sprite>(texture, xFrameCount, yFrameCount, name);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}

	bool Sprite::CreateAnimationSequence(const std::string& name, uint32_t start, uint32_t end, float frameRate)
	{
		auto loc = m_Sequences.find(name);
		if (loc != m_Sequences.end()) {
			LOG_S(WARNING) << "Animation sequence: \"" << name << "\" already exists for this sprite!";
			return false;
		}
		AnimationSequence seq(start, end, frameRate);
		if (ValidSequence(seq)) {
			m_Sequences[name] = seq;
			return true;
		}
		LOG_S(WARNING) << "Animation sequence: \"" << name << "\" not a valid sequence for this sprite!";
		return false;
	}

	bool Sprite::CreateAnimationSequence(const std::string& name, const AnimationSequence& sequence)
	{
		auto loc = m_Sequences.find(name);
		if (loc != m_Sequences.end()) {
			return false;
		}
		if (ValidSequence(sequence)) {
			m_Sequences[name] = sequence;
			return true;
		}
		return false;
	}

	const Sprite::AnimationSequence* Sprite::GetAnimationSequence(const std::string& name) const
	{
		// TODO: insert return statement here
		auto loc = m_Sequences.find(name);
		if (loc != m_Sequences.end()) {
			return &loc->second;
		}
		return nullptr;
	}

	std::pair<glm::vec2, glm::vec2> Sprite::GetUVsForFrame(uint32_t frame) const
	{
		auto uvSize = GetFrameSizeUV();
		//auto invFrame = GetLastFrame() - frame;
		uint32_t frameX = frame % m_XFrameCount;
		uint32_t frameY = (m_YFrameCount - 1) - (frame / m_XFrameCount);
		glm::vec2 uvMin = glm::vec2(uvSize.x * frameX, uvSize.y * frameY);
		glm::vec2 uvMax = uvMin + uvSize;
		return std::make_pair(uvMin, uvMax);
	}


}

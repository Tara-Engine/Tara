#include "tarapch.h"
#include "SpriteEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/Texture.h"

namespace Tara {

	SpriteEntity::SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, SpriteRef sprite)
		: Entity(parent, owningLayer, transform, name), m_Sprite(sprite), m_CurrentFrame(0), m_CurrentSequence(0,0,0.0f), m_CurrentFrameTimer(0.0f)
	{}

	std::shared_ptr<SpriteEntity> SpriteEntity::Create(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, SpriteRef sprite)
	{
		std::shared_ptr<SpriteEntity> newEntity = std::make_shared<SpriteEntity>(parent, owningLayer, transform, name, sprite);
		Entity::Register(newEntity);
		return newEntity;
	}

	void SpriteEntity::OnUpdate(float deltaTime)
	{
		if (m_CurrentSequence.FrameRate > 0.0f) {
			m_CurrentFrameTimer += deltaTime;
			if (m_CurrentFrameTimer > m_CurrentSequence.FrameRate) {
				m_CurrentFrameTimer = 0.0f;
				m_CurrentFrame++;
				if (m_CurrentFrame > m_CurrentSequence.End) {
					m_CurrentFrame = m_CurrentSequence.Start;
				}
			}
		}
	}

	void SpriteEntity::OnDraw(float deltaTime)
	{
		if (m_Sprite){
			auto UVs = m_Sprite->GetUVsForFrame(m_CurrentFrame);
			Renderer::Quad(GetWorldTransform(), glm::vec4{ 1.0f,1.0f,1.0f,1.0f }, m_Sprite->GetTexture(), UVs.first, UVs.second);
		}
	}

	void SpriteEntity::PlayAnimation(const std::string& name)
	{
		auto anim = m_Sprite->GetAnimationSequence(name);
		if (anim) {
			SetCurrentSequence(*anim);
		}
	}

}
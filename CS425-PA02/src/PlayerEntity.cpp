#include "tarapch.h"
#include "PlayerEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Input/Input.h"
#include "Tara/Input/Mappings.h"

namespace Tara {

	PlayerEntity::PlayerEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name)
		: SpriteEntity(parent, owningLayer, transform, name, nullptr), m_Velocity(0.0f, 0.0f)
	{}

	PlayerEntity::PlayerEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, Tara::Texture2DRef texture)
		: SpriteEntity(parent, owningLayer, transform, name, texture), m_Velocity(0.0f, 0.0f)
	{}


	std::shared_ptr<PlayerEntity> PlayerEntity::Create(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, Tara::Texture2DRef texture)
	{
		std::shared_ptr<PlayerEntity> newEntity = std::make_shared<PlayerEntity>(parent, owningLayer, transform, name, texture);
		Entity::Register(newEntity);
		return newEntity;
	}

	void PlayerEntity::OnUpdate(float deltaTime)
	{
		Tara::Input *input = Tara::Input::Get();
		if (input->IsKeyPressed(TARA_KEY_W) || input->IsKeyPressed(TARA_KEY_UP))
		{
			m_Velocity.y = 100;
		}
		if (input->IsKeyPressed(TARA_KEY_S) || input->IsKeyPressed(TARA_KEY_DOWN))
		{
			m_Velocity.y = -100;
		}
		if (input->IsKeyPressed(TARA_KEY_A) || input->IsKeyPressed(TARA_KEY_LEFT))
		{
			m_Velocity.x = -100;
		}
		if (input->IsKeyPressed(TARA_KEY_D) || input->IsKeyPressed(TARA_KEY_RIGHT))
		{
			m_Velocity.y = 100;
		}
	}
}
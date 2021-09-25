#pragma once
#include "Tara/Core/Layer.h"
#include "Tara/Core/Entity.h"
#include "Tara/Renderer/Texture.h"

namespace Tara {
	class SpriteEntity : public Entity	{

	public:
		SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name);
		SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture);

		static std::shared_ptr<SpriteEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::Texture2DRef texture = nullptr);

	public:
		virtual ~SpriteEntity() {};

		//virtual void OnUpdate(float deltaTime) override; //No need to override

		virtual void OnDraw(float deltaTime) override;

		//virtual void OnEvent(Tara::Event& e) override; //No need to override

		inline virtual Tara::BoundingBox GetSpecificBoundingBox() const override { return Tara::BoundingBox::FromTransform(GetWorldTransform()); }

		inline void SetTexture(Tara::Texture2DRef texture) { m_Texture = texture; }
		inline const Tara::Texture2DRef& GetTexture() const { return m_Texture; }

	private:
		Tara::Texture2DRef m_Texture;
	};

	using SpriteEntityRef = std::shared_ptr<SpriteEntity>;
}
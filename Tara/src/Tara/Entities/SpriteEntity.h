#pragma once
#include <Tara.h>

class SpriteEntity : public Tara::Entity
{

public:
	SpriteEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name);

	static std::shared_ptr<SpriteEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity");

public:
	virtual ~SpriteEntity();

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnDraw(float deltaTime) override;

	virtual void OnEvent(Tara::Event& e) override;

	virtual bool OnOverlapEvent(Tara::OverlapEvent& e);

	inline virtual Tara::BoundingBox GetSpecificBoundingBox() const override { return Tara::BoundingBox::FromTransform(GetWorldTransform()); }

	inline void SetTexture(Tara::Texture2DRef texture) { m_Texture = texture; }
	inline const Tara::Texture2DRef& GetTexture() const { return m_Texture; }

private:
	Tara::Texture2DRef m_Texture;
};
#pragma once
#include <Tara.h>

//As a random note, the "T" at the beginning of this name just denonates that this is for Testing

class TColorRectEntity : public Tara::Entity {

public:
	TColorRectEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name);

	static std::shared_ptr<TColorRectEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "TColorRectEntity");

public:
	virtual ~TColorRectEntity();

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnDraw(float deltaTime) override;

	inline virtual Tara::BoundingBox GetSpecificBoundingBox() const override { return Tara::BoundingBox::FromTransform(GetWorldTransform()); }

	//color stuff
	inline void SetColor(glm::vec4 color) { m_Color = color; }
	inline const glm::vec4& GetColor() const { return m_Color; }

private:
	glm::vec4 m_Color;
};

class TControlableEntity : public TColorRectEntity {
public:
	TControlableEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name);
	static std::shared_ptr<TControlableEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "TControlableEntity");
	virtual ~TControlableEntity();

	virtual void OnUpdate(float deltaTime) override;

	inline void SetSpeed(float s) { m_Speed = s; }
	inline float GetSpeed() const { return m_Speed; }

private:
	float m_Speed;
};

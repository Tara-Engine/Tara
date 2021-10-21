#pragma once
#include <Tara.h>

//As a random note, the "T" at the beginning of this name just denonates that this is for Testing

class TColorRectEntity : public Tara::Entity {

public:
	TColorRectEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "TColorRectEntity");

public:
	virtual ~TColorRectEntity();

	virtual void OnBeginPlay() override;

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnDraw(float deltaTime) override;

	virtual void OnEvent(Tara::Event& e) override;

	virtual bool OnOverlapEvent(Tara::OverlapEvent& e);

	inline virtual Tara::BoundingBox GetSpecificBoundingBox() const override { return Tara::BoundingBox::FromTransform(GetWorldTransform()); }

	//color stuff
	inline void SetColor(glm::vec4 color) { m_Color = color; }
	inline const glm::vec4& GetColor() const { return m_Color; }

private:
	glm::vec4 m_Color;
};


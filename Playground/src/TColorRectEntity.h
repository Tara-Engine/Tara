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

	//color stuff
	inline void SetColor(glm::vec4 color) { m_Color = color; }
	inline const glm::vec4& GetColor() const { return m_Color; }

private:
	glm::vec4 m_Color;
};

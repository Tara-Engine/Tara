#pragma once
#include <Tara.h>
#include "TColorRectEntity.h"

struct QuadData {
	float x, y;
	float width, height;
	float Umin, Vmin, Umax, Vmax;
	float r, g, b, a;
	float textureIndex;
	QuadData(glm::vec2 pos, glm::vec2 scale, glm::vec2 UVmin, glm::vec2 UVmax, glm::vec4 color, float textureIndex)
		: x(pos.x), y(pos.y), width(scale.x), height(scale.y), Umin(UVmin.x), Vmin(UVmin.y), Umax(UVmax.x), Vmax(UVmax.y),
		r(color.r), g(color.g), b(color.b), a(color.a), textureIndex(textureIndex)
	{}
};


class BatchTestLayer : public Tara::Layer {
public:
	BatchTestLayer();

	~BatchTestLayer() {
		Deactivate();
	}

	virtual void Activate() override;

	virtual void Draw(float deltaTime) override;


private:
	Tara::Texture2DRef m_Texture;
	Tara::Texture2DRef m_Texture2;
	Tara::Texture2DRef m_Texture3;



	float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	std::shared_ptr<TControlableEntity> m_Player;
	

	Tara::ShaderRef m_GShader;
	Tara::VertexArrayRef m_QuadPoints;
	
};

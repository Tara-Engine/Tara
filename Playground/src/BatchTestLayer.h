#pragma once
#include <Tara.h>
#include "TColorRectEntity.h"

struct OldQuadData {
	float x, y;
	float width, height;
	float Umin, Vmin, Umax, Vmax;
	float r, g, b, a;
	float textureIndex;
	OldQuadData(glm::vec2 pos, glm::vec2 scale, glm::vec2 UVmin, glm::vec2 UVmax, glm::vec4 color, float textureIndex)
		: x(pos.x), y(pos.y), width(scale.x), height(scale.y), Umin(UVmin.x), Vmin(UVmin.y), Umax(UVmax.x), Vmax(UVmax.y),
		r(color.r), g(color.g), b(color.b), a(color.a), textureIndex(textureIndex)
	{}
};



struct QuadData {
	Tara::Transform Transform;  //+9 [ 9] floats
	glm::vec2 UVmin;			//+2 [11] floats
	glm::vec2 UVmax;			//+2 [13] floats
	glm::vec4 Color;			//+4 [17] floats
	float TextureIndex;			//+1 [18] floats
	QuadData(const Tara::Transform& transform, const glm::vec2& uvmin, const glm::vec2& uvmax, const glm::vec4& color, float textureIndex)
		: Transform(transform), UVmin(uvmin), UVmax(uvmax), Color(color), TextureIndex(textureIndex)
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

	virtual void OnEvent(Tara::Event& e) override;

	bool BatchTestLayer::OnWindowResizeEvent(Tara::WindowResizeEvent& e);
private:
	Tara::Texture2DRef m_Texture;
	Tara::Texture2DRef m_Texture2;
	Tara::Texture2DRef m_Texture3;



	float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	std::shared_ptr<TControlableEntity> m_Player;
	

	Tara::ShaderRef m_GShader;
	Tara::VertexArrayRef m_QuadPoints;
	
	std::vector<QuadData> m_Quads;
};

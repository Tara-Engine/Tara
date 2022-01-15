#pragma once
#include <Tara.h>

class ModelBuildLayer : public Tara::Layer {
public:
	ModelBuildLayer();

	virtual ~ModelBuildLayer();

	virtual void Activate() override;

	virtual void Deactivate() override;

	//virtual void Update(float deltaTime) override;

	virtual void Draw(float deltaTime) override;

	virtual void OnEvent(Tara::Event& e) override;

	Tara::MaterialBaseRef m_Material;
private:
	Tara::CameraEntityRef m_Camera;
	Tara::CameraEntityRef m_ScreenCamera;

	Tara::ShaderRef m_Shader;

	Tara::StaticMeshRef m_Mesh;

	Tara::PointLightEntityRef m_Light;
	Tara::DirectionalLightEntityRef m_DirectionalLight;

	Tara::RenderTargetRef m_DepthTarget;
};
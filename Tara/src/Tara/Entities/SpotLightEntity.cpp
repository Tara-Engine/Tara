#include "tarapch.h"
#include "SpotLightEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara{
	Texture2DRef SpotLightEntity::s_EditorLogo = nullptr;

	SpotLightEntity::SpotLightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, Vector lightColor, float lightIntensity, float lightAngle, const std::string& name)
		:Entity(parent, owningLayer, transform, name),
		LightBase(lightColor, lightIntensity, 1, LightType::SPOT),
		m_SpotlightAngle(lightAngle),
		m_SpotlightInnerAngle(lightAngle),
		m_DrawingEditorLogo(false),
		m_DepthTarget(nullptr)
	{
		SetLightRadiusFromIntensity();
		srand((uint32_t)(this));
		std::stringstream ss;
		ss << "__SpotlightDepthMap__:" << rand();
		m_DepthTarget = RenderTarget::Create(1024, 1024, 0, RenderTarget::InternalType::FLOAT, true, ss.str());
		AssetLibrary::Get()->ForgetAsset(m_DepthTarget); //should not be in asset lib anyway
		m_DepthTarget->SetDefaultTextureWrapping(Texture::Wrapping::Border);
		m_DepthTarget->SetBorderColor({ 1,1,1,1 });
	}


	void SpotLightEntity::OnUpdate(float deltaTime)
	{
		GetOwningLayer().lock()->EnqueLight(std::dynamic_pointer_cast<LightBase>(shared_from_this()));
	}

	void SpotLightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(std::dynamic_pointer_cast<SpotLightEntity>(shared_from_this()));
		if (s_EditorLogo && m_DrawingEditorLogo && Renderer::GetCurrentSceneData().renderPass == RenderPass::Standard) {
			auto camPos = Renderer::GetCurrentSceneData().camera->GetPosition();
			Vector forward = (GetWorldPosition() - camPos).Normalize();
			Transform transform = Transform(GetWorldPosition() - Vector{ 0.5, 0.5, 0.5 }, Rotator::FromForwardVector(forward), { 1,1,1 });
			Renderer::Quad(transform, { 1,1,1,1 }, s_EditorLogo);
		}
	}

	LightData SpotLightEntity::GetLightData()
	{
		float theta = glm::cos(glm::radians(GetSpotlightAngle()));//the cos is so that when we have this value in the shader, we are working with the same units as vector dot products.
		return LightData(
			GetWorldPosition(), GetLightColor(), GetLightIntensity(), GetLightRadius(), GetLightType(),
			-(GetWorldRotation().GetForwardVector()), 
			theta, 
			glm::cos(glm::radians(GetSpotlightInnerAngle())) - theta //the difference between inner and outer angle
		);
	}

	glm::mat4 SpotLightEntity::GetLightProjectionMatrix()
	{
		glm::mat4 proj = glm::perspective(glm::radians(2* m_SpotlightAngle), 1.0f, 0.1f, m_LightRadius);
		glm::mat4 view = glm::inverse(Transform(GetWorldPosition(), GetWorldRotation(), { 1 }).GetTransformMatrix());
		return proj * view;
	}
}

#include "tarapch.h"
#include "DirectionalLightEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara {
	Texture2DRef DirectionalLightEntity::s_EditorLogo = nullptr;

	DirectionalLightEntity::DirectionalLightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, Vector lightColor, float lightIntensity, const std::string& name)
		: Entity(parent, owningLayer, transform, name),
		LightBase(lightColor, lightIntensity, 0, LightType::DIRECTIOAL),
		m_DrawingEditorLogo(false),
		m_DepthTarget(nullptr),
		m_BackDistance(100),
		m_ShadowScale(50)
	{
		srand((uint32_t)(this));
		std::stringstream ss;
		ss << "__DirectionalLightDepthMap__:" << rand();
		m_DepthTarget = RenderTarget2D::Create(4096, 4096, 0, RenderTarget::InternalType::FLOAT, true, ss.str());
		AssetLibrary::Get()->ForgetAsset(m_DepthTarget); //should not be in asset lib anyway
		m_DepthTarget->SetWrap(Texture::Wrapping::Border);
		m_DepthTarget->SetFiltering(Texture::Filtering::Linear);
		m_DepthTarget->SetBorderColor({ 1,1,1,1 });
	}

	void DirectionalLightEntity::OnUpdate(float deltaTime)
	{
		GetOwningLayer().lock()->EnqueLight(std::dynamic_pointer_cast<LightBase>(shared_from_this()));
	}

	void DirectionalLightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(std::dynamic_pointer_cast<DirectionalLightEntity>(shared_from_this()));
		if (s_EditorLogo && m_DrawingEditorLogo && Renderer::GetCurrentSceneData().renderPass == RenderPass::Standard) {
			auto camPos = Renderer::GetCurrentSceneData().camera->GetPosition();
			Vector forward = (GetWorldPosition() - camPos).Normalize();
			Transform transform = Transform(GetWorldPosition() - Vector{ 0.5, 0.5, 0.5 }, Rotator::FromForwardVector(forward), { 1,1,1 });
			Renderer::Quad(transform, { 1,1,1,1 }, s_EditorLogo);
		}
	}

	LightData DirectionalLightEntity::GetLightData()
	{
		//directional lights are rendered as if from backwards along the main layer camera position
		auto pos = GetWorldPosition();
		auto cam = GetOwningLayer().lock()->GetLayerCamera();
		if (cam.lock()) {
			pos = cam.lock()->GetWorldPosition();
		}
		pos += GetWorldRotation().GetForwardVector() * (m_BackDistance/2);
		if (Renderer::GetCurrentSceneData().light) {
			return LightData(
				pos, GetLightColor(), GetLightIntensity(),
				m_BackDistance * 2, GetLightType(),
				GetWorldRotation().GetForwardVector()
			);
		}
		else {
			return LightData(
				pos, GetLightColor(), GetLightIntensity(), 
				-(m_BackDistance * 2), GetLightType(),
				GetWorldRotation().GetForwardVector()
			);
		}
	}

	glm::mat4 DirectionalLightEntity::GetLightProjectionMatrix()
	{
		auto pos = GetWorldPosition();
		auto cam = GetOwningLayer().lock()->GetLayerCamera();
		if (cam.lock()) {
			pos = cam.lock()->GetWorldPosition();
		}
		pos += GetWorldRotation().GetForwardVector() * (m_BackDistance/2);
		
		glm::mat4 proj = glm::ortho(
			-m_ShadowScale, m_ShadowScale,
			-m_ShadowScale, m_ShadowScale,
			0.1f, m_BackDistance*2
		);
		
		//glm::mat4 proj = glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, m_BackDistance);
		glm::mat4 view = glm::inverse(Transform(pos, GetWorldRotation(), { 1 }).GetTransformMatrix());
		return proj * view;
	}


}
#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Light.h"

namespace Tara {
	REFTYPE(DirectionalLightEntity);
	NOREFTYPE(DirectionalLightEntity);

	class DirectionalLightEntity : public Entity, public LightBase {
	public:
		DirectionalLightEntity(
			EntityNoRef parent, LayerNoRef owningLayer,
			Transform transform = TRANSFORM_DEFAULT,
			Vector lightColor = { 1,1,1 },
			float lightIntensity = 1.0f,
			const std::string& name = "AmbientLightEntity"
		);

		virtual ~DirectionalLightEntity() = default;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnDraw(float deltaTime) override;

		virtual LightData GetLightData() override;

		inline virtual RenderTargetRef GetDepthTarget() override { return m_DepthTarget; };

		virtual glm::mat4 GetLightProjectionMatrix() override;

		inline virtual bool ShouldDrawDepth() override { return true; }

		/// <summary>
		/// Get the distance behing the current layer camera that this light draws its shadows from (backwards along its rotation)
		/// </summary>
		/// <returns></returns>
		inline float GetBackDistance() { return m_BackDistance; }

		/// <summary>
		/// Set the distance behing the current layer camera that this light draws its shadows from (backwards along its rotation)
		/// </summary>
		/// <returns></returns>
		inline void SetBackDistance(float distance) { m_BackDistance = distance; }

		/// <summary>
		/// Get the radius of the area shadows are drawn around the main layer camera
		/// </summary>
		/// <returns></returns>
		inline float GetShadowScale() { return m_ShadowScale; }

		/// <summary>
		/// Set the radius of the area shadows are drawn around the main layer camera
		/// </summary>
		/// <returns></returns>
		inline void SetShadowScale(float scale) { m_ShadowScale = scale; }

		/// <summary>
		/// Set the class-wide editor logo for this type of light
		/// </summary>
		/// <param name="logo"></param>
		inline static void SetEditorLogo(const Texture2DRef& logo) { s_EditorLogo = logo; }

		/// <summary>
		/// Get if this specific light is drawing the editor logo
		/// </summary>
		/// <returns></returns>
		inline bool GetDrawingEditorLogo() { return m_DrawingEditorLogo; }

		/// <summary>
		/// Set if this specific light is drawing the editor logo
		/// </summary>
		/// <param name="drawing"></param>
		inline void SetDrawingEditorLogo(bool drawing) { m_DrawingEditorLogo = drawing; }
	private:
		bool m_DrawingEditorLogo;
		RenderTarget2DRef m_DepthTarget;
		float m_BackDistance;
		float m_ShadowScale;
	private:
		static Texture2DRef s_EditorLogo;
	};

}
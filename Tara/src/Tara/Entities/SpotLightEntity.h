#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Light.h"

namespace Tara {
	REFTYPE(SpotLightEntity);
	NOREFTYPE(SpotLightEntity);

	class SpotLightEntity : public Entity, public LightBase {
	public:
		SpotLightEntity(
			EntityNoRef parent, LayerNoRef owningLayer,
			Transform transform = TRANSFORM_DEFAULT,
			Vector lightColor = { 1,1,1 },
			float lightIntensity = 1.0f,
			float lightAngle = 25.0f,
			const std::string& name = "AmbientLightEntity"
		);

		virtual ~SpotLightEntity() = default;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnDraw(float deltaTime) override;

		virtual LightData GetLightData() override;

		inline virtual RenderTargetRef GetDepthTarget() override { return m_DepthTarget; };

		virtual glm::mat4 GetLightProjectionMatrix() override;

		inline virtual bool ShouldDrawDepth() override { return true; }


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

		/// <summary>
		/// Get the spotlight angle
		/// </summary>
		/// <returns></returns>
		inline float GetSpotlightAngle() { return m_SpotlightAngle; }

		/// <summary>
		/// Set the spotlight angle
		/// If shrinking would make this angle less than the inner angle, the inner angle is shrunk to be the same as this one
		/// </summary>
		/// <param name="angle"></param>
		inline void SetSpotlightAngle(float angle) { m_SpotlightAngle = angle; m_SpotlightInnerAngle = fmin(m_SpotlightAngle, m_SpotlightInnerAngle); }

		/// <summary>
		/// Get the innder angle of the spotlight
		/// </summary>
		/// <returns></returns>
		inline float GetSpotlightInnerAngle() { return m_SpotlightInnerAngle; }

		/// <summary>
		/// Set the inner angle of the spotlight
		/// Silently fails if the angle provided is larger than the outer angle of the spotlight
		/// </summary>
		/// <param name="angle"></param>
		inline void SetSpotlightInnerAngle(float angle) { if (angle < m_SpotlightAngle) { m_SpotlightInnerAngle = angle; } }

	private:
		float m_SpotlightAngle;
		float m_SpotlightInnerAngle;
		bool m_DrawingEditorLogo;
		RenderTargetRef m_DepthTarget;
	private:
		static Texture2DRef s_EditorLogo;
	};

}
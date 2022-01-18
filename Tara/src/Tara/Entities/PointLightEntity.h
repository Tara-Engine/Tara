#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Light.h"

namespace Tara {

	REFTYPE(PointLightEntity);
	NOREFTYPE(PointLightEntity);

	class PointLightEntity : public Entity, public LightBase {
	public:

		PointLightEntity(
			EntityNoRef parent, LayerNoRef owningLayer,
			Transform transform = TRANSFORM_DEFAULT,
			Vector lightColor = { 1,1,1 },
			float lightIntensity = 1.0f,
			const std::string& name = "PointLightEntity"
		);

		virtual ~PointLightEntity() = default;

		virtual void OnDraw(float deltaTime) override;

		virtual void OnUpdate(float deltaTime) override;

		virtual LightData GetLightData() override;


		inline virtual RenderTargetRef GetDepthTarget() override { return m_DepthTarget; };

		virtual glm::mat4 GetLightProjectionMatrix() override;

		inline virtual bool ShouldDrawDepth() override { return true; }

		inline virtual bool DepthIsPanoramic() override { return true; }

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
		RenderTargetCubemapRef m_DepthTarget;
	private:
		static Texture2DRef s_EditorLogo;
	};
}
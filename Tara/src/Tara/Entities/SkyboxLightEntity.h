#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Light.h"
#include "Tara/Asset/StaticMesh.h"
#include "Tara/Renderer/Shader.h"


namespace Tara {

	REFTYPE(SkyboxLightEntity);
	NOREFTYPE(SkyboxLightEntity);

	class SkyboxLightEntity : public Entity, public LightBase {
	public:

		SkyboxLightEntity(
			EntityNoRef parent, LayerNoRef owningLayer,
			Transform transform = TRANSFORM_DEFAULT,
			TextureCubemapRef lightTexture = nullptr,
			float lightIntensity = 1.0f,
			const std::string& name = "PointLightEntity"
		);

		virtual ~SkyboxLightEntity() = default;

		virtual void OnDraw(float deltaTime) override;

		virtual void OnUpdate(float deltaTime) override;

		virtual LightData GetLightData() override;

		virtual TextureCubemapRef GetDiffuseMap() override { return m_DiffuseTexture; };

		virtual TextureCubemapRef GetSpecularMap() override { return m_SpecularTexture; };

		inline virtual bool ShouldDrawDepth() override { return false; }

		inline virtual bool DepthIsPanoramic() override { return false; }

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

	public:
		static TextureCubemapRef CreateConvolutionFromCubemap(const TextureCubemapRef& map);

	private:
		static void CreateSkyboxAssets();

	private:
		bool m_DrawingEditorLogo;
		TextureCubemapRef m_DiffuseTexture;
		TextureCubemapRef m_SpecularTexture;
	private:
		static Texture2DRef s_EditorLogo;
		static ShaderRef s_SkyboxShader;
		static StaticMeshRef s_SkyboxMesh;
	};
}
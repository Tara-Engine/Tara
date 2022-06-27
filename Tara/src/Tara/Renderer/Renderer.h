#pragma once
#include "tarapch.h"
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Camera.h"
#include "Tara/Math/Types.h"
#include "Tara/Renderer/RenderTarget.h"
#include "Tara/Asset/Font.h"
#include "Tara/Asset/Patch.h"
#include "Tara/Renderer/Light.h"
#include "Tara/Asset/StaticMesh.h"
#include "Tara/Renderer/MaterialBase.h"

namespace Tara {
	
	/// <summary>
	/// A enum of the types of rendering backends available
	/// </summary>
	enum class RenderBackend {
		None = 0,
		OpenGl
	};
	class RenderCommand;
	
	enum class RenderPass {
		Standard = 0,
		ModelDepth
	};

	struct RenderSceneData {
		CameraRef camera;
		LightBaseRef light;
		RenderTargetRef target;
		RenderPass renderPass;
		std::vector<LightBaseRef> lights;
	};

	/// <summary>
	/// Renderer
	/// Mostly static class that is used for rendering things in worldspace. 
	/// This includes any 2d or 3d world components, but not UI
	/// UI rendering should be done with Renderer2D <NOT MADE YET> that renders in screenspace
	/// </summary>
	class Renderer {
		friend class RenderCommand;
	public:
		/// <summary>
		/// Get the rendering backend
		/// </summary>
		/// <returns>the rendering backend</returns>
		static RenderBackend GetRenderBackend() { return s_RenderBackend; }

		/// <summary>
		/// Begin a scene to render with a given camera
		/// </summary>
		/// <param name="camera">the camera to render with</param>
		static void BeginScene(const CameraRef camera);

		/// <summary>
		/// Begin a scene, rendering the model depth, for shading purposes.
		/// </summary>
		/// <param name="camera"></param>
		static void BeginModelDepthScene(const LightBaseRef light);//TODO: change to light

		/// <summary>
		/// End the current scene
		/// </summary>
		static void EndScene();

		/// <summary>
		/// draw a arbitrary vertex array
		/// </summary>
		/// <param name="vertexArray">the vertex array to draw</param>
		/// <param name="shader">the shader to draw with</param>
		/// <param name="transform">where the vertex array is</param>
		static void Draw(VertexArrayRef vertexArray, ShaderRef shader, Transform transform);

		/// <summary>
		/// draw a texture on a 1x1 quad
		/// </summary>
		/// <param name="texture">the texture to draw</param>
		/// <param name="transform">the transform of the quad</param>
		static void Quad(const Transform& transform, glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f }, const Texture2DRef& texture = nullptr, glm::vec2 minUV = { 0,0 }, glm::vec2 maxUV = {1,1});

		/// <summary>
		/// Render text. If this is being done every frame with unchanging text, it may be more efficent to instead get the rect data from the font directly, and cache it.
		/// IE, this is a quick and lazy function, or for text that changes every frame.
		/// </summary>
		/// <param name="transform">the transform to draw the text at</param>
		/// <param name="text">the text to draw</param>
		/// <param name="font">the font to draw with</param>
		/// <param name="color">the color of the text. defaults to white</param>
		static void Text(const Transform& transform, const std::string& text, FontRef font, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		
		/// <summary>
		/// Render a 9-patch. If this is being done every frame with an unchanging patch, 
		/// it may be more efficent to do it manually with some caching.
		/// </summary>
		/// <param name="transform">The transform to render at</param>
		/// <param name="patch">the patch to render</param>
		/// <param name="color">the tint color</param>
		static void Patch(const Transform& transform, const PatchRef& patch, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

		/// <summary>
		/// Draw a light into the world
		/// </summary>
		/// <param name="transform">the light's transform</param>
		/// <param name="color">the light's color</param>
		/// <param name="intensity">the light's intensity</param>
		/// <param name="angle">the spotlight angle, if a spotlight. ranges from 0 to 1</param>
		static void Light(const LightBaseRef& light);

		/// <summary>
		/// Render a Static Mesh
		/// </summary>
		/// <param name="mesh"></param>
		/// <param name="materials"></param>
		/// <param name="transform"></param>
		static void StaticMesh(const Transform& transform, const StaticMeshRef& mesh, const std::vector<MaterialBaseRef>& materials);

		/// <summary>
		/// Get the current scene data. Non-modifiable reference. Used inside of OnDraw functions to query info about the current scene that may change how they render.
		/// </summary>
		/// <returns></returns>
		inline static const RenderSceneData& GetCurrentSceneData() { return s_SceneData; };

	private:
		/// <summary>
		/// Loads the quad shader
		/// </summary>
		static void LoadQuadShader();

		/// <summary>
		/// Loads the Model Depth Shader
		/// </summary>
		static void LoadMeshDepthShader();

		/// <summary>
		/// Renders the batched quads
		/// </summary>
		static void RenderQuads();

		/// <summary>
		/// Render the scene, executing all queues, deferred rendering, lighting, etc.
		/// </summary>
		static void SceneRender();

		/// <summary>
		/// load the assets nesecary to render a scene
		/// </summary>
		static void LoadSceneParts();

	private:

		/// <summary>
		/// Structure that holds the information for a single batched quad
		/// </summary>
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

		struct QuadGroup {
			std::vector<QuadData> Quads;
			std::vector<Texture2DRef> TextureNames;
		};

	
		/// <summary>
		/// Structure that holds data about the current scene.
		/// </summary>
		

	private:

		/// <summary>
		/// Stored rendering backend
		/// </summary>
		static RenderBackend s_RenderBackend;
		
		/// <summary>
		/// Stored scene data.
		/// </summary>
		static RenderSceneData s_SceneData;
		
		/// <summary>
		/// The Gbuffer for the current render
		/// </summary>
		static RenderTarget2DRef s_GBuffer;
		static RenderTarget2DRef s_PostProcessBufferA;
		static RenderTarget2DRef s_PostProcessBufferB;
		static RenderTarget2DRef s_FakeDepth2D;
		static RenderTargetCubemapRef s_FakeDepthCubemap;
		static TextureCubemapRef s_FakeDiffuseCubemap;
		static TextureCubemapRef s_FakeSpecularCubemap;
		
		/// <summary>
		/// full screen quad for rendering the GBuffer
		/// </summary>
		static StaticMeshRef s_ScreenQuadMesh;
		static StaticMeshRef s_LightSphereMesh;

		//max textures number
		static uint32_t s_MaxTextures;

		//batched quad stuff
		static VertexArrayRef s_QuadArray;
		static ShaderRef s_QuadShader;
		static std::vector<QuadGroup> s_QuadGroups;

		//depth draw stuff
		static ShaderRef s_MeshDepthShader;
		static ShaderRef s_MeshDepthPanoramicShader;
	};

}
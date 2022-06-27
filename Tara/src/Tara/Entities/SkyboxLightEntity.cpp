#include "tarapch.h"
#include "SkyboxLightEntity.h"
#include "Tara/Asset/AssetLibrary.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{

	Texture2DRef SkyboxLightEntity::s_EditorLogo = nullptr;
	ShaderRef SkyboxLightEntity::s_SkyboxShader = nullptr;
	StaticMeshRef SkyboxLightEntity::s_SkyboxMesh = nullptr;

	SkyboxLightEntity::SkyboxLightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, TextureCubemapRef lightTexture, float lightIntensity, const std::string& name)
		:Entity(parent, owningLayer, transform, name),
		LightBase({ 1,1,1 }, lightIntensity, 0, LightType::SKYBOX),
		m_DrawingEditorLogo(false),
		m_DiffuseTexture(SkyboxLightEntity::CreateConvolutionFromCubemap(lightTexture)),
		m_SpecularTexture(lightTexture)
	{
		if (s_SkyboxMesh == nullptr || s_SkyboxShader == nullptr) {
			SkyboxLightEntity::CreateSkyboxAssets();
		}
	}

	void SkyboxLightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(std::dynamic_pointer_cast<SkyboxLightEntity>(shared_from_this()));
		if (s_EditorLogo && m_DrawingEditorLogo && Renderer::GetCurrentSceneData().renderPass == RenderPass::Standard) {
			auto camPos = Renderer::GetCurrentSceneData().camera->GetPosition();
			Vector forward = (GetWorldPosition() - camPos).Normalize();
			Transform transform = Transform(GetWorldPosition() - Vector{ 0.5, 0.5, 0.5 }, Rotator::FromForwardVector(forward), { 1,1,1 });
			Renderer::Quad(transform, { 1,1,1,1 }, s_EditorLogo);
		}
		RenderCommand::EnableDeferred(false);
		RenderCommand::EnableDepthTesting(true);
		if (s_SkyboxShader->ValidUniform("u_EnvironmentMap")){
			m_SpecularTexture->Bind(0,0);
			//for some crazy reason, this works without actually sending the texture.
			// HOWEVER, sending gives a pile of errors in the console about sending the wrong type of data.
			// So, its currently commented out.
			
			//s_SkyboxShader->Send("u_EnvironmentMap", 0);
		}
		Renderer::Draw(s_SkyboxMesh->GetVertexArrays()[0], s_SkyboxShader, GetWorldTransform());
		m_SpecularTexture->Unbind();
	}

	void SkyboxLightEntity::OnUpdate(float deltaTime)
	{
		GetOwningLayer().lock()->EnqueLight(std::dynamic_pointer_cast<LightBase>(shared_from_this()));
		//m_DiffuseTexture = SkyboxLightEntity::CreateConvolutionFromCubemap(m_SpecularTexture); //stupid to have this here, but its for testing so I can see what is going on
	}

	LightData SkyboxLightEntity::GetLightData()
	{
		return LightData(GetWorldPosition(), GetLightColor(), GetLightIntensity(), GetLightRadius(), GetLightType());
	}

	static std::string convolutionVertexSource = R"VOGON(
#version 450 core
layout (location = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_MatrixProjection;
uniform mat4 u_MatrixView;

void main()
{
    v_Position = a_Position;  
    gl_Position =  u_MatrixProjection * u_MatrixView * vec4(a_Position, 1.0);
}
)VOGON";

	static std::string convolutionFragmentSource = R"VOGON(
#version 450 core
out vec4 FragColor;
in vec3 v_Position;

uniform samplerCube u_EnvironmentMap;

const float PI = 3.14159265359;

void main()
{		
    // the sample direction equals the hemisphere's orientation 
    vec3 normal = normalize(v_Position);
  
    vec3 irradiance = vec3(0.0);
  
    vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, normal));
	up         = normalize(cross(normal, right));
	
	float sampleDelta = 0.025;
	float nrSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
	    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
	    {
	        // spherical to cartesian (in tangent space)
	        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
	        // tangent space to world
	        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 
	
	        irradiance += texture(u_EnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
	        nrSamples++;
	    }
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
  
    FragColor = vec4(irradiance, 1.0);
}
)VOGON";

	TextureCubemapRef SkyboxLightEntity::CreateConvolutionFromCubemap(const TextureCubemapRef& map)
	{
		DCHECK_F(!RenderCommand::CheckQueue(), "Cannot create a convolution when in a queue");
		if (map) {

			//load the data needed for convolution
			auto cubeMesh = AssetLibrary::Get()->GetAssetIf<StaticMesh>("__unit_cube__"); //should exist, from loading panoramic image.
			if (cubeMesh == nullptr) {
				cubeMesh = StaticMesh::Create(
					{ MeshPart::UnitCube().Transform(
						//the skybox is scaled to max size in the shader.
						Tara::Transform({-0.5, -0.5,-0.5},{0,0,0},{2,2,2}).GetTransformMatrix()
					) },
					"__unit_cube__"
				);
			}
			
			std::unordered_map<Shader::TargetStage, std::string> shaderStages;
			shaderStages[Shader::TargetStage::Vertex] = convolutionVertexSource;
			shaderStages[Shader::TargetStage::Pixel] = convolutionFragmentSource;
			auto convolutionShader = Shader::Create(
				"__Convolution_Shader__",
				Shader::SourceType::Strings,
				shaderStages
			);

			uint32_t size = map->GetWidth() / 16; //one sixteenth the size seems to work nicely. Taken from LearnOpenGL.com
			auto target = Tara::RenderTargetCubemap::Create(size, size, 1, RenderTarget::InternalType::FLOAT, map->GetAssetName() + "_CONVOLUTED");
			
			glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			std::array<glm::mat4, 6> captureViews{
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			};
			
			convolutionShader->Bind();
			convolutionShader->Send("u_MatrixProjection", captureProjection);
			map->Bind(0);
			convolutionShader->Send("u_EnvironmentMap", 0);
			target->RenderTo(true);
			auto cubeVertexArray = cubeMesh->GetVertexArrays()[0];
			cubeVertexArray->Bind();
			for (uint32_t i = 0; i < 6; i++) {
				//this will cause it to render to the target
				target->SetTargetedFace((RenderTargetCubemap::Face)i);
				convolutionShader->Send("u_MatrixView", captureViews[i]);

				RenderCommand::Clear();
				RenderCommand::Draw(cubeVertexArray);
			}
			target->SetTargetFaceAll();
			target->RenderTo(false);
			return target;
		}
		return nullptr;
	}

	static std::string skyboxShaderVertexSource = R"VOGON(
#version 450 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_MatrixProjection;
uniform mat4 u_MatrixView;

out vec3 v_Position;

void main()
{
    v_Position = a_Position;

    mat4 rotView = mat4(mat3(u_MatrixView)); // remove translation from the view matrix
    vec4 clipPos = u_MatrixProjection * rotView * vec4(a_Position, 1.0);

    gl_Position = clipPos.xyww; //max out the depth value
}
)VOGON";

	static std::string skyboxShaderFragmentSource = R"VOGON(
#version 450 core
out vec4 FragColor;

in vec3 v_Position;
  
uniform samplerCube u_EnvironmentMap;
  
void main()
{
    vec3 envColor = texture(u_EnvironmentMap, normalize(v_Position)).rgb;
    
	//gamma correction
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
	//FragColor = vec4(1,1,1,1);
}
)VOGON";
	
	void SkyboxLightEntity::CreateSkyboxAssets()
	{
		if (s_SkyboxMesh == nullptr) {
			//this SHOULD exist cause it is created to make a cubemap
			s_SkyboxMesh = AssetLibrary::Get()->GetAssetIf<StaticMesh>("__panoramic_cube__");
			if (s_SkyboxMesh == nullptr){
				s_SkyboxMesh = StaticMesh::Create(
					{ MeshPart::UnitCube().Transform(
						//the skybox is scaled to max size in the shader.
						Tara::Transform({-0.5, -0.5,-0.5},{0,0,0},{2,2,2}).GetTransformMatrix()
					).FlipTriangles() },
					"__panoramic_cube__"
				);
			}
		}
		if (s_SkyboxShader == nullptr) {
			std::unordered_map<Shader::TargetStage, std::string> shaderStages;
			shaderStages[Shader::TargetStage::Vertex] = skyboxShaderVertexSource;
			shaderStages[Shader::TargetStage::Pixel] = skyboxShaderFragmentSource;
			s_SkyboxShader = Shader::Create(
				"__Skybox_Shader__",
				Shader::SourceType::Strings,
				shaderStages
			);
		}
	}

}

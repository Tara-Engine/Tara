#include "tarapch.h"
#include "OpenGLTextureCubemap.h"
#include "Tara/Asset/AssetLibrary.h"

#include "Tara/Renderer/Shader.h"
#include "Tara/Asset/StaticMesh.h"
#include "Tara/Renderer/RenderCommand.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace Tara {

	OpenGLTextureCubemap::OpenGLTextureCubemap(std::string path, uint32_t size, const std::string& name)
		: Texture (name), TextureCubemap(name), m_Path(path), m_Width(size), m_Height(size)
	{
		LoadHDRIFile();
	}

	OpenGLTextureCubemap::~OpenGLTextureCubemap()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCubemap::ImplBind(int slot, int index) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTextureCubemap::ImplUnbind() const
	{
	}

	void OpenGLTextureCubemap::SetFiltering(Filtering filter)
	{
		if (filter == Texture::Filtering::Nearest) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void OpenGLTextureCubemap::SetWrap(Wrapping wrap)
	{
		switch (wrap) {
		case Texture::Wrapping::Clamp: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		case Texture::Wrapping::Border: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		case Texture::Wrapping::Repeat: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		case Texture::Wrapping::Mirror: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		}
	}

	void OpenGLTextureCubemap::SetBorderColor(const glm::vec4& color)
	{
		glTextureParameterfv(m_RendererID, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&color);
	}

	/*
	* This variable holds the vertex shader source code for 
	* turning a HDRI panoramic into a cubemap. 
	*/
	static std::string panoramicToCubemapVertexSource = R"VOGON(
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

	static std::string panoramicToCubemapFragmentSource = R"VOGON(
#version 450 core
out vec4 FragColor;
in vec3 v_Position;

uniform sampler2D u_EquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(v_Position)); // make sure to normalize localPos
    vec3 color = texture(u_EquirectangularMap, uv).rgb;
    
    FragColor = vec4(color, 1.0);
}

)VOGON";

	/*
	* HDRI Panoramic conversion system created from
	* https://learnopengl.com/PBR/IBL/Diffuse-irradiance
	* This was very helpful
	*/
	void OpenGLTextureCubemap::LoadHDRIFile()
	{
		DCHECK_F(!RenderCommand::CheckQueue(), "Cannot create a HDRI Cubemap when currently in a render queue. Create it outside any render operations");

		//first, load the texture into a glTexture2D object.
		int32_t width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		float* imageData = stbi_loadf(m_Path.c_str(), &width, &height, &channels, 0);
		DCHECK_NOTNULL_F(imageData, "Failed to load image! Path: %s", m_Path.c_str());
		
		uint32_t hdriTexture = 0;
		glGenTextures(1, &hdriTexture);
		glBindTexture(GL_TEXTURE_2D, hdriTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(imageData);

		//then, render to cubemap
		//this requires making a rendertarget
		uint32_t captureFBO, captureRBO;
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		//and then actually creating our texture
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		
		//create each face
		for (uint32_t i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
				m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		
		//deal with filtering
		if (Texture::s_DefaultTextureFiltering == Texture::Filtering::Nearest) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		//deal with wrapping
		switch (Texture::s_DefaultTextureWrapping) {
		case Texture::Wrapping::Clamp: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		case Texture::Wrapping::Border: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		case Texture::Wrapping::Repeat: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		case Texture::Wrapping::Mirror: {
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		}

		//set up the data needed for rendering
		//shader, projections, cube mesh, etc.
		std::unordered_map<Shader::TargetStage, std::string> shaderStages;
		shaderStages[Shader::TargetStage::Vertex] = panoramicToCubemapVertexSource;
		shaderStages[Shader::TargetStage::Pixel] = panoramicToCubemapFragmentSource;
		ShaderRef panoramicToCubemapShader = Shader::Create(
			"__panoramicToCubemapShader__",
			Shader::SourceType::Strings,
			shaderStages
		);

		//get a cube from -1 to 1 in x,y,z
		//UnitCube makes one from 0 to 1, so, move it so its centered on 0,0,0 and scale up by 2
		StaticMeshRef cubeMesh = StaticMesh::Create(
			{ MeshPart::UnitCube().Transform(
				Tara::Transform({-0.5, -0.5,-0.5},{0,0,0},{2,2,2}).GetTransformMatrix()
			) }, 
			"__unit_cube__"
		);

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		std::array<glm::mat4, 6> captureViews{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		//THEN performing the render.
		//since we know this is OpenGL renderer, we can skip using the Renderer and render much of this manually.
		panoramicToCubemapShader->Bind();
		panoramicToCubemapShader->Send("u_MatrixProjection", captureProjection);
		panoramicToCubemapShader->Send("u_EquirectangularMap", 0);
		glBindTextureUnit(0, hdriTexture);
		auto& cubeVertexArray = cubeMesh->GetVertexArrays()[0];
		cubeVertexArray->Bind();

		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glViewport(0, 0, m_Width, m_Height);
		for (uint32_t i = 0; i < 6; i++) {
			//we have to render the cube six times, one for each face of the cube map
			panoramicToCubemapShader->Send("u_MatrixView", captureViews[i]);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, 
				GL_COLOR_ATTACHMENT0, 
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				m_RendererID, 
				0
			);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//render the cube
			RenderCommand::Draw(cubeVertexArray);
		}

		//and now clean up the old, useless GL objects
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteRenderbuffers(1, &captureRBO);
		glDeleteFramebuffers(1, &captureFBO);
		glDeleteTextures(1, &hdriTexture);
	}

}

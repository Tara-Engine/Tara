#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"

/*This file is responsible for loading the shader for batch rendering quads. 
* It is seperate from Renderer.cpp due to the long, long inline glsl strings
* The reason the glsl code is inside of the cpp code, and not loaded from a file
* is due to the fact that this shader should exist, even if no shader files are loaded by the engine
* 
* As far as the whole thing with a stringstream in the fragment (pixel) shader goes, some AMD gpus cannot accept a array of samplers (at least, in OpenGL) and thus, there is some ... fussing to avoid that
* Its made to be dynamic to the max number of shaders that the specific GPU currently being used can handle.
*/

namespace Tara {
	void Renderer::LoadQuadShader()
	{
		std::unordered_map<Shader::TargetStage, std::string> sources;
		switch (Renderer::GetRenderBackend()) {
		case RenderBackend::None : { break; }
		case RenderBackend::OpenGl: {
			//fill the sources map with a bunch of strings
			sources[Shader::TargetStage::Vertex] = R"V0G0N(
#version 450 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Rotation;
layout(location=2) in vec3 a_Scale;
layout(location=3) in vec2 a_UVmin;
layout(location=4) in vec2 a_UVmax;
layout(location=5) in vec4 a_Color;
layout(location=6) in float a_TextureIndex;

out VS_OUT {
	mat4  Transform;
	vec2  UVmin;
	vec2  UVmax;
	vec4  Color;
	float TextureIndex;
} v_QuadData;

uniform mat4 u_MatrixViewProjection;

mat4 Scale(vec3 scalar){
	return mat4(
		scalar.x,0,0,0,
		0,scalar.y,0,0,
		0,0,scalar.z,0,
		0,0,0,1
	);
}
mat4 Translate(vec3 position){
	return mat4(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		position.x,position.y,position.z,1
	);
}

mat4 Rotate(vec3 axis, float angle){
	axis = normalize(axis);
    float s = sin(radians(angle));
    float c = cos(radians(angle));
    float oc = 1.0 - c;
    return mat4(
		oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
        0.0,                                0.0,                                0.0,                                1.0
	);
}

mat4 Transform(vec3 position, vec3 rotation, vec3 scale){
	//roll = x
	//pitch = y
	//yaw = z
    //the negatives in the axis make it match the glm c++ side functions.
	mat4 m_scale = Scale(scale);
	mat4 m_pos = Translate(position);
	mat4 m_rot = 
		Rotate(vec3(0.0,-1.0,0.0), rotation.z)*
		Rotate(vec3(-1.0, 0.0, 0.0), rotation.y) * 
		Rotate(vec3(0.0, 0.0, -1.0), rotation.x); 
	return m_pos * m_rot * m_scale;
}

void main(){
	v_QuadData.Transform = u_MatrixViewProjection * Transform(a_Position, a_Rotation, a_Scale);
	v_QuadData.UVmin = a_UVmin;
	v_QuadData.UVmax = a_UVmax;
	v_QuadData.Color = a_Color;
	v_QuadData.TextureIndex = a_TextureIndex;
	gl_Position = vec4(0,0,0,1);
}
)V0G0N";

			sources[Shader::TargetStage::Geometry] = R"V0G0N(
#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

in VS_OUT {
	mat4  Transform;
	vec2  UVmin;
	vec2  UVmax;
	vec4  Color;
	float TextureIndex;
} v_QuadData[];

out GEOM_OUT {
	vec2 UV;
	vec4 Color;
	float TextureIndex;
} v_VertexData;

void main(){
	v_VertexData.Color = v_QuadData[0].Color;
	v_VertexData.TextureIndex = v_QuadData[0].TextureIndex;
	gl_Position = v_QuadData[0].Transform * vec4(0,0,0,1);
	v_VertexData.UV = v_QuadData[0].UVmin;
	EmitVertex();
	gl_Position = v_QuadData[0].Transform * vec4(1,0,0,1);
	v_VertexData.UV = vec2(v_QuadData[0].UVmax.x, v_QuadData[0].UVmin.y);
	EmitVertex();
	gl_Position = v_QuadData[0].Transform * vec4(0,1,0,1);
	v_VertexData.UV = vec2(v_QuadData[0].UVmin.x, v_QuadData[0].UVmax.y);
	EmitVertex();
	gl_Position = v_QuadData[0].Transform * vec4(1,1,0,1);
	v_VertexData.UV = v_QuadData[0].UVmax;
	EmitVertex();
	EndPrimitive();
}
)V0G0N";

			std::stringstream ss;
			ss << R"V0G0N(
#version 450 core
#extension GL_ARB_bindless_texture : require

layout(location=0)out vec4 color;
#define TEXTURE_COUNT )V0G0N";
			ss << s_MaxTextures << "\n";
			for (uint32_t i = 0; i < s_MaxTextures; i++) {
				ss << "uniform sampler2D u_Texture" << i << ";\n";
			}

			ss << R"V0G0N(
in GEOM_OUT {
	vec2 UV;
	vec4 Color;
	float TextureIndex;
} v_VertexData;

void main(){
	
	if(v_VertexData.TextureIndex < 0){
		color = v_VertexData.Color;
	}
	else{
		sampler2D textures[TEXTURE_COUNT] = {
)V0G0N";

			for (uint32_t i = 0; i < s_MaxTextures; i++) {
				ss << "u_Texture" << i;
				if (i < s_MaxTextures - 1) {
					ss << ", ";
				}
			}

			ss << R"V0G0N(};
		int index = int(v_VertexData.TextureIndex);
		color = v_VertexData.Color * texture(textures[index], v_VertexData.UV);
	}
}
)V0G0N";
			sources[Shader::TargetStage::Pixel] = ss.str();
			break;
		}
		}
		s_QuadShader = Shader::Create("D_QuadShader", Shader::SourceType::Strings, sources);
	}
}
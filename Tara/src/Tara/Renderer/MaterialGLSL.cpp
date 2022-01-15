#include "tarapch.h"
#include "Tara/Asset/Material.h"

/*
The purpose of this file is to contain all the inline GLSL code for the material system, as it is growing fast
and the Material.cpp file was getting bloated.
*/

namespace Tara {

	std::unordered_map<MaterialType, std::string> Material::SourcePartsVertex = {
		{
			MaterialType::UNLIT,
			R"V0G0N(
//UNLIT
#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec4 a_Color;
layout(location=3) in vec2 a_UV;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

out vec3 v_WorldNorm;
out vec3 v_WorldPos;
out vec4 v_Color;
out vec2 v_UV;

void main(){
	v_WorldNorm = vec3(inverse(transpose(u_MatrixModel)) * vec4(a_Normal, 1));
	v_WorldPos = vec3(u_MatrixModel * vec4(a_Position, 1));
	v_Color = a_Color;
	
	v_UV = a_UV;
	
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
}

)V0G0N"
		},
		{
			MaterialType::LIT,
			R"V0G0N(
//LIT
#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec4 a_Color;
layout(location=3) in vec2 a_UV;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

out vec3 v_WorldNorm;
out vec3 v_WorldPos;
out vec4 v_Color;
out vec2 v_UV;

void main(){
	v_WorldNorm = vec3(inverse(transpose(u_MatrixModel)) * vec4(a_Normal, 1));
	v_WorldPos = vec3(u_MatrixModel * vec4(a_Position, 1));
	v_Color = a_Color;
	
	v_UV = a_UV;
	
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
}

)V0G0N"
		},
		{
			MaterialType::LIGHTING,
			R"V0G0N(
//LIGHTING
#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec4 a_Color;
layout(location=3) in vec2 a_UV;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

void main(){
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
}

)V0G0N"
		},
	};

	std::unordered_map<MaterialType, std::string> Material::SourcePartsFragmentBegin = {
		{
			MaterialType::UNLIT,
			R"V0G0N(
//UNLIT
#version 450 core
layout(location=0)out vec4 outColor;

uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

in vec3 v_WorldNorm;
in vec3 v_WorldPos;
in vec4 v_Color;
in vec2 v_UV;
)V0G0N"
		},
		{
			MaterialType::LIT,
			R"V0G0N(
//LIT
#version 450 core
layout(location=0)out vec4 ColorMetallic;
layout(location=1)out vec4 SpecularRoughness;
layout(location=2)out vec4 EmissiveAO;
layout(location=3)out vec4 WorldSpaceNormal;
layout(location=4)out vec4 WorldSpacePosition;

uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

in vec3 v_WorldNorm;
in vec3 v_WorldPos;
in vec4 v_Color;
in vec2 v_UV;

vec3 PixelNormal = vec3(0);
)V0G0N"
		},
		{
			MaterialType::LIGHTING,
			R"V0G0N(
//LIGHTING
#version 450 core
layout(location=0)out vec4 outColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D u_ColorMetallicSampler;
uniform sampler2D u_SpecularRoughnessSampler;
uniform sampler2D u_EmissiveAOSampler;
uniform sampler2D u_WorldSpaceNormalSampler;
uniform sampler2D u_WorldSpacePositionSampler;

uniform vec2 u_TargetSize;			
uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

uniform vec3 u_LightPosition;
uniform vec3 u_LightForwardVector;
uniform vec3 u_LightColor;
uniform int u_LightType;
uniform float u_LightIntensity;
uniform float u_LightParam1;
uniform float u_LightParam2;
uniform float u_LightRadius;
uniform float u_CameraNearClipPlane;
uniform float u_CameraFarClipPlane;
uniform mat4 u_LightProjectionMatrix;
uniform sampler2D u_LightDepthMapPlanar;
uniform float u_LightDepthMapSize;

float Metallic         = 0;
float Roughness        = 0;
float AmbientOcclusion = 0;
vec3 Diffuse            = vec3(0);
vec3 Specular           = vec3(0);
vec3 Emissive           = vec3(0);
vec3 WorldSpaceNormal   = vec3(0);
vec3 WorldSpacePosition = vec3(0);
float Depth				= 0;
vec2 UVs				= vec2(0);

const int LightType_Point		= 0;			
const int LightType_Spot		= 1;			
const int LightType_Directional	= 2;			
const int LightType_Ambient		= 3;			
const int LightType_Rect		= 4;			

)V0G0N"
		},
	};

	std::unordered_map<MaterialType, std::string> Material::SourcePartsFragmentEnd = {
		{
			MaterialType::UNLIT,
			R"V0G0N(
//UNLIT
void main(){
	outColor = diffuse();
}
)V0G0N"
		},
		{
			MaterialType::LIT,
			R"V0G0N(
//LIT
void main(){
	PixelNormal = normalize(v_WorldNorm + normal().xyz);

	ColorMetallic = vec4(diffuse().xyz, metallic() + 1);
	SpecularRoughness = vec4(specular().xyz, roughness() + 1);
	EmissiveAO = vec4(emissive().xyz, ambientOcclusion() + 1);
	WorldSpaceNormal = vec4(PixelNormal,1);
	WorldSpacePosition = vec4(v_WorldPos,1);
}
)V0G0N"
		},
		{
			MaterialType::LIGHTING,
			R"V0G0N(
//LIGHTING
void main(){
	UVs = vec2(gl_FragCoord.x / u_TargetSize.x, gl_FragCoord.y / u_TargetSize.y );
	
	Diffuse = texture(u_ColorMetallicSampler, UVs).xyz;
	Metallic = texture(u_ColorMetallicSampler, UVs).w -1;
	Specular = texture(u_SpecularRoughnessSampler, UVs).xyz;
	Roughness = texture(u_SpecularRoughnessSampler, UVs).w -1;
	Emissive = texture(u_EmissiveAOSampler, UVs).xyz;
	AmbientOcclusion = texture(u_EmissiveAOSampler, UVs).w -1;
	WorldSpaceNormal = texture(u_WorldSpaceNormalSampler, UVs).xyz;
	WorldSpacePosition = texture(u_WorldSpacePositionSampler, UVs).xyz;
	Depth = length(WorldSpacePosition - u_CameraPositionWS) / u_CameraFarClipPlane;

	outColor = color();
}
)V0G0N"
		},
	};

	std::unordered_map<std::string, std::string> Material::SourceIncludes = {
		{"phong", 
		R"V0G0N(
vec2 Phong(float attenuationLinear, float attenuationQuadratic){
	if (u_LightType == LightType_Point){
		float lightDist = length(u_LightPosition - WorldSpacePosition);	
		float attenuation = min((u_LightIntensity / (1 +  attenuationLinear + lightDist + attenuationQuadratic * (lightDist*lightDist))), 1);
		vec3 lightDir = normalize(u_LightPosition - WorldSpacePosition);
		float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
		float specularFactor = pow(max(dot(normalize(u_CameraPositionWS - WorldSpacePosition), reflect(-lightDir, WorldSpaceNormal)), 0.0), pow(2, (1-Roughness)*8));
		return vec2(diffuseFactor*attenuation, specularFactor*attenuation);
	}
	else if (u_LightType == LightType_Spot){
		vec3 lightDir = normalize(u_LightPosition - WorldSpacePosition);
		float theta = dot(lightDir, normalize(-u_LightForwardVector));
		if (theta > u_LightParam1){
			float intensity = clamp((theta - u_LightParam1) / u_LightParam2, 0.0, 1.0);
			float lightDist = length(u_LightPosition - WorldSpacePosition);	
			float attenuation = min(((u_LightIntensity *intensity) / (1 +  attenuationLinear + lightDist + attenuationQuadratic * (lightDist*lightDist))), 1);
			float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
			float specularFactor = pow(max(dot(normalize(u_CameraPositionWS - WorldSpacePosition), reflect(-lightDir, WorldSpaceNormal)), 0.0), pow(2, (1-Roughness)*8));
			return vec2(diffuseFactor*attenuation, specularFactor*attenuation);
		}else{
			discard; //if outside spotlight effect, ignore pixel entirely!
		}
	}
	else if (u_LightType == LightType_Directional){
		vec3 lightDir = normalize(-u_LightForwardVector);
		float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
		float specularFactor = pow(max(dot(normalize(u_CameraPositionWS - WorldSpacePosition), reflect(-lightDir, WorldSpaceNormal)), 0.0), pow(2, (1-Roughness)*8));
		
		return vec2(u_LightIntensity * diffuseFactor, u_LightIntensity * specularFactor);
	}
	else if (u_LightType == LightType_Ambient){
		return vec2((u_LightIntensity / 100), 0.0);
	}
}
)V0G0N"
		},
	};
}
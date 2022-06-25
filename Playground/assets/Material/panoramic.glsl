#include <normals>

uniform samplerCube cubemap;

vec3 diffuse(){
	//return normalize(texture(normalTexture, v_UV).xyz * 2.0 - 1.0);
	return texture(cubemap, normalize(v_WorldNorm)).xyz;
}

vec3 normal(){
	//return NormalStrength(NormalTexture(normalTexture, v_UV), 4.0);
	return vec3(0,0,1);
}

vec3 specular(){
	return vec3(1);
}

vec3 emissive(){
	return texture(cubemap, normalize(v_WorldNorm)).xyz;
}

float metallic(){
	return 0;
}

float roughness(){
	return 0.5;
}

float ambientOcclusion(){
	return 1;
}
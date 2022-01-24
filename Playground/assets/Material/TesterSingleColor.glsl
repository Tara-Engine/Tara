#include <normals>
uniform float metallicValue;
uniform float roughnessValue;

vec3 diffuse(){
	return vec3(1,0,0);
}

vec3 normal(){
	return vec3(0,0,1);
}

vec3 specular(){
	return vec3(1);
}

vec3 emissive(){
	return vec3(0);
}

float metallic(){
	return clamp(metallicValue, 0.0, 0.999999);
}

float roughness(){
	return clamp(roughnessValue, 0.001, 1);
}

float ambientOcclusion(){
	return 1.0;
}
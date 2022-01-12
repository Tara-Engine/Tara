uniform vec4 tintColor;
uniform sampler2D diffuseTexture;
uniform float roughnessValue;

vec3 diffuse(){
	return  vec3(tintColor) * vec3(texture(diffuseTexture, v_UV));
}

vec3 normal(){
	return vec3(0,0,0);
}

vec3 specular(){
	return vec3(1);
}

vec3 emissive(){
	return vec3(0);
}

float metallic(){
	return 0;
}

float roughness(){
	return roughnessValue;
}

float ambientOcclusion(){
	return 1;
}
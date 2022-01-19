uniform sampler2D diffuseTexture;
uniform sampler2D emissiveTexture;
uniform sampler2D normalTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D metallicTexture;
uniform sampler2D ambientOcclusionTexture;

vec3 diffuse(){
	//return normalize(texture(normalTexture, v_UV).xyz * 2.0 - 1.0);
	return texture(diffuseTexture, v_UV).xyz;
}

vec3 normal(){
	return normalize((texture(normalTexture, v_UV).xyz * 2.0 - 1.0));
	//return vec3(0,0,1)
}

vec3 specular(){
	return vec3(1);
}

vec3 emissive(){
	return texture(emissiveTexture, v_UV).xyz;
}

float metallic(){
	return texture(metallicTexture, v_UV).x;
}

float roughness(){
	return texture(roughnessTexture, v_UV).x;
}

float ambientOcclusion(){
	return texture(ambientOcclusionTexture, v_UV).x;
}
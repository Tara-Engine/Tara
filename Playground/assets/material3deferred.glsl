uniform vec4 tintColor;

vec3 diffuse(){
	return  vec3(tintColor);
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
	return 0.1;
}

float ambientOcclusion(){
	return 1;
}

vec4 color(){
	float da = clamp(dot(WorldSpaceNormal, normalize(u_CameraPositionWS - WorldSpacePosition)), 0, 1);
	float db = clamp(dot(WorldSpaceNormal, normalize(u_CameraForwardVector)), 0, 1);
	
	return vec4(vec3((da + db + 1)/3) * Diffuse, 1);
	
	//return vec4(abs(WorldSpaceNormal), 1);
}
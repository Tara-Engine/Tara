
vec4 color(){
	//color accumulator
	vec3 accColor = vec3(0,0,0);
	
	for(int i=0;i<u_LightCount;i++){
		//for each light
		
		//diffuse (no intensity yet)
		vec3 lightDir = normalize(u_LightPositions[i] - WorldSpacePosition);
		float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
		
		//specular (no intensity yet)
		vec3 viewDir = normalize(u_CameraPositionWS - WorldSpacePosition);
		vec3 reflectDir = reflect(-lightDir, WorldSpaceNormal);
		float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		
		accColor = accColor + (diffuseFactor * (Diffuse * u_LightColors[i])) + (specularFactor * (Specular * u_LightColors[i]));
	}
	accColor = accColor / (u_LightCount * 2);
	//accColor = (accColor + Diffuse * 0.1);
	
	return vec4(accColor, 1);
}
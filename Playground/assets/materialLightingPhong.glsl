
vec4 color(){
	//color accumulator
	vec3 accColor = vec3(0,0,0);
	
	for(int i=0;i<u_LightCount;i++){
		//for each light
		
		if (u_LightTypeIntensitieCustoms[i].x == LightType_Point){
			float intensity = u_LightTypeIntensitieCustoms[i].y;
			float lightDist = length(u_LightPositions[i] - WorldSpacePosition);	
			//attenuation factors chosen somewhat arbitrarily as they make it look good.
			float attenuation = (intensity / (1 +  0.022 + lightDist + 0.0019 * (lightDist*lightDist)));
		
			//diffuse (no intensity yet)
			vec3 lightDir = normalize(u_LightPositions[i] - WorldSpacePosition);
			float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
			
			//specular (no intensity yet)
			float specPower = pow(2, (1-Roughness)*8);
			vec3 viewDir = normalize(u_CameraPositionWS - WorldSpacePosition);
			vec3 reflectDir = reflect(-lightDir, WorldSpaceNormal);
			float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
			
			//accColor = accColor + vec3(attenuation);
			accColor = accColor + attenuation * ((diffuseFactor * (Diffuse * u_LightColors[i])) + (specularFactor * (Specular * u_LightColors[i])));
		
		}
	}
	//accColor = accColor / (u_LightCount * 2);
	//accColor = (accColor + Diffuse * 0.1);
	
	return vec4(accColor, 1);
}
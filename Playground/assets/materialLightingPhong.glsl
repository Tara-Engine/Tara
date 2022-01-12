
vec4 color(){
	//color accumulator
	vec3 resultColor = vec3(0,0,0);
		
	if (u_LightType == LightType_Point){
		float lightDist = length(u_LightPosition - WorldSpacePosition);	
		
		//attenuation factors chosen somewhat arbitrarily as they make it look good.
		//intensity comes from here
		float attenuation = min((u_LightIntensity / (1 +  0.07 + lightDist + 0.017 * (lightDist*lightDist))), 1);
	
		//diffuse
		vec3 lightDir = normalize(u_LightPosition - WorldSpacePosition);
		float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
		
		//specular
		float specPower = pow(2, (1-Roughness)*8);
		vec3 viewDir = normalize(u_CameraPositionWS - WorldSpacePosition);
		vec3 reflectDir = reflect(-lightDir, WorldSpaceNormal);
		float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
		
		/*
		float frac = 1;
		if (lightDist > (u_LightRadius * 0.75)){
			float alphaDist = lightDist - (u_LightRadius * 0.75);
			float maxDist = u_LightRadius * 0.25;
			frac = alphaDist * maxDist;
		}
		*/
		
		resultColor = attenuation * ((diffuseFactor * (Diffuse * u_LightColor)) + (specularFactor * (Specular * u_LightColor)));
	}
	else if (u_LightType == LightType_Spot){
		vec3 lightDir = normalize(u_LightPosition - WorldSpacePosition);
	
		float theta = dot(lightDir, normalize(-u_LightForwardVector));
		if (theta > u_LightParam1){ //u_LightParam1 holds the outer angle for spotlights
			//spotlight feathered edge
			float intensity = clamp((theta - u_LightParam1) / u_LightParam2, 0.0, 1.0);
			//attenuation
			float lightDist = length(u_LightPosition - WorldSpacePosition);	
			float attenuation = min(((u_LightIntensity *intensity) / (1 +  0.07 + lightDist + 0.017 * (lightDist*lightDist))), 1);
			
			//diffuse
			float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
			
			//specular
			float specPower = pow(2, (1-Roughness)*8);
			vec3 viewDir = normalize(u_CameraPositionWS - WorldSpacePosition);
			vec3 reflectDir = reflect(-lightDir, WorldSpaceNormal);
			float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
			
			resultColor = attenuation * ((diffuseFactor * (Diffuse * u_LightColor)) + (specularFactor * (Specular * u_LightColor)));
			
		}else{
			discard; //if outside spotlight effect, ignore pixel entirely!
		}
	}
	else if (u_LightType == LightType_Directional){
		//diffuse
		vec3 lightDir = normalize(-u_LightForwardVector);
		float diffuseFactor = max(dot(WorldSpaceNormal, lightDir), 0.0);
		
		//specular
		float specPower = pow(2, (1-Roughness)*8);
		vec3 viewDir = normalize(u_CameraPositionWS - WorldSpacePosition);
		vec3 reflectDir = reflect(-lightDir, WorldSpaceNormal);
		float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
		
		resultColor = u_LightIntensity * ((diffuseFactor * (Diffuse * u_LightColor)) + (specularFactor * (Specular * u_LightColor)));
	}
	else if (u_LightType == LightType_Ambient){
		resultColor = (u_LightIntensity / 100) * u_LightColor * Diffuse;
	}
	
	
	
	//return vec4(Diffuse, 1);
	return vec4(resultColor, 1);
}
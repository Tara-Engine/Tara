#include <shadow>

//lighting calculations from
//https://learnopengl.com/PBR/Lighting

const float PI = 3.14159265359;

float DistributionGGX(vec3 normal, vec3 halfDir, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(normal, halfDir), 0.0); //might need to be non-zero
	float NdotH2 = NdotH * NdotH;
	
	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness){
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0; //Direct
	//float k = (roughness * roughness) / 2.0; //Image-Based Lighting
	
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return num / denom;
}

float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness){
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 FrensnelSchlick(float cosTheta, vec3 F0){
	//the top implementation is from Epic's paper on shading.
	float ct = clamp(cosTheta, 0.0, 1.0);
	float p = (-5.55473 * cosTheta - 6.98316) * cosTheta;
	return F0 + (1.0 - F0) * pow(2.0, p);
	//this implementation is form learnOpenGL.com
	//return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

const float attenuationLinear = 0.07;
const float attenuationQuadratic = 0.017;


vec4 color(){
	if (u_LightType == LightType_Point || u_LightType == LightType_Spot || u_LightType == LightType_Directional){
		vec3 normal = WorldSpaceNormal;
		vec3 viewDir = normalize(u_CameraPositionWS - WorldSpacePosition);
		
		//reflection (base)
		vec3 F0 = vec3(0.04);
		F0 = mix(F0, Diffuse, Metallic);
		
		//we only have one light, so there is no loop (IBL will come later)
		
		//calculate the per-light radiance
		
		//light Dir
		vec3 lightDir = vec3(0);
		float attenuation = 1.0;
		if (u_LightType == LightType_Directional){
			lightDir = -normalize(-u_LightForwardVector);
		}else{
			lightDir = normalize(u_LightPosition - WorldSpacePosition);	
		}
		
		//attenuation
		//float attenuation = 1.0 / lightDist * lightDist; //IMPROVE THIS
		float lightDist = length(u_LightPosition - WorldSpacePosition);
		if (u_LightType == LightType_Point){
			attenuation = min((1 / (1 +  attenuationLinear + lightDist + attenuationQuadratic * (lightDist*lightDist))), 1);
		}
		if (u_LightType == LightType_Spot){
			float theta = dot(lightDir, normalize(-u_LightForwardVector));
			float intensity = clamp((theta - u_LightParam1) / u_LightParam2, 0.0, 1.0);
			attenuation = min((1 / (intensity +  attenuationLinear + lightDist + attenuationQuadratic * (lightDist*lightDist))), 1);
		}
		//half-dir and radiance
		vec3 halfDir = normalize(viewDir + lightDir);
		vec3 radiance = u_LightColor * (u_LightIntensity) * attenuation;
		
		//cook-torrance brdf
		float NDF = DistributionGGX(normal, halfDir, Roughness);
		float G = GeometrySmith(normal, viewDir, lightDir, Roughness);
		vec3 F = FrensnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);
		
		vec3 kS = F; //specular constant
		vec3 kD = vec3(1.0) - kS; //diffuse constant, energy conservation
		kD *= 1.0 - Metallic; //metallic darkening
		
		float NdotL = max(dot(normal, lightDir), 0.0);
		
		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * NdotL + 0.0001; //the last term is so it never reaches 0.
		vec3 specular = numerator / denominator;
		
		//reflectance equation
		vec3 finalColor = ((kD * Diffuse / PI) + specular) * radiance * NdotL;
		
		//color combination
		finalColor = (finalColor * Shadow(0.015) * AmbientOcclusion) + Emissive;
		return vec4(finalColor, 1.0); //error here. Emissive is becoming stronger and stronger each light added.
	}	
	else{ //assume ambient light. Eventually replace with IBL
		return vec4(Diffuse * u_LightColor * (u_LightIntensity / 100), 1.0);
	}
}
#include <phong>

vec4 color(){
	vec2 phongResults = Phong(0.07, 0.017);	
	
	vec3 resultColor = (phongResults.x * (Diffuse * u_LightColor)) + (phongResults.y * (Diffuse * Specular * u_LightColor));
	
	float shadowValue = 1;
	
	if (u_LightType == LightType_Spot || u_LightType == LightType_Directional){
		float lightDepth = length(WorldSpacePosition - u_LightPosition) / u_LightRadius;
		
		vec4 fragPosLight = u_LightProjectionMatrix * vec4(WorldSpacePosition, 1);
		
		vec3 fragProjCoords = fragPosLight.xyz / fragPosLight.w;
		
		fragProjCoords = (fragProjCoords +1) / 2.0;
		
		float uvModifer = 1 / u_LightDepthMapSize;
		float depthAry [5];
		depthAry[0] = texture(u_LightDepthMapPlanar, fragProjCoords.xy).r;
		
		depthAry[1] = texture(u_LightDepthMapPlanar, fragProjCoords.xy + vec2(uvModifer,0) ).r;
		depthAry[2] = texture(u_LightDepthMapPlanar, fragProjCoords.xy - vec2(uvModifer,0) ).r;
		
		depthAry[3] = texture(u_LightDepthMapPlanar, fragProjCoords.xy + vec2(0,uvModifer) ).r;
		depthAry[4] = texture(u_LightDepthMapPlanar, fragProjCoords.xy - vec2(0,uvModifer) ).r;
		
		float closestDepth = depthAry[0];
		for (int i=0;i<5;i++){
			closestDepth = closestDepth + depthAry[i];
		}
		closestDepth = closestDepth / 6.0;
		
		
		shadowValue = (closestDepth > lightDepth || lightDepth > 1.0 || fragProjCoords.x > 1 || fragProjCoords.y > 1 || fragProjCoords.x < 0 || fragProjCoords.y < 0) ? 1.0 : 0.0;
	}
	
	
	resultColor = resultColor * shadowValue; //fog*shadow
	return vec4(resultColor, 1);
}
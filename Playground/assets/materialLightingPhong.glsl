#include <phong>
#include <shadow>


vec4 color(){
	vec2 phongResults = Phong(0.07, 0.017);	
	
	vec3 resultColor = (phongResults.x * (Diffuse * u_LightColor)) + (phongResults.y * (Diffuse * Specular * u_LightColor));

	resultColor = resultColor * Shadow(0.015); 
	resultColor += Emissive;
	resultColor = resultColor * AmbientOcclusion;
	
	return vec4(resultColor, 1);
}
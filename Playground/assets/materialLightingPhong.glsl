#include <phong>

vec4 color(){
	//color accumulator
	vec3 resultColor = vec3(0,0,0);
	vec2 phongResults = Phong(0.07, 0.017);	
	
	resultColor = (phongResults.x * (Diffuse * u_LightColor)) + (phongResults.y * (Diffuse * Specular * u_LightColor));
	
	//return vec4(Diffuse, 1);
	return vec4(resultColor, 1);
}
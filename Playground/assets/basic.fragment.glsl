#version 450 core
layout(location=0)out vec4 color;

uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

in vec3 v_WorldNorm;
in vec3 v_WorldPos;
in vec4 v_Color;
in vec2 v_UV;

float near = 0.1; 
float far  = 1; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main(){
	/*
	color = v_WorldNorm;
	if (v_WorldNorm.x == -1){
		color = vec4(0,1,1,1);
	}
	if (v_WorldNorm.y == -1){
		color = vec4(1,0,1,1);
	}
	if (v_WorldNorm.z == -1){
		color = vec4(1,1,0,1);
	}
	*/
	float da = dot(normalize(v_WorldNorm), normalize(u_CameraPositionWS-v_WorldPos));
	float db = dot(normalize(v_WorldNorm), normalize(u_CameraForwardVector));
	da = clamp(da, 0, 1);
	db = clamp(db, 0, 1);
	color = vec4(vec3((da + db + 1)/3), 1) * v_Color;
	
	//float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	//color = vec4(vec3(depth), 1.0);
	
	//color = vec4(v_UV, 0, 1);
	
	//color = v_Color;
	
	//color = vec4(1.0,1.0,1.0,1.0);
}
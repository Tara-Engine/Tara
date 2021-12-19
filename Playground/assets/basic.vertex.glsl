#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec4 a_Color;
layout(location=3) in vec2 a_UV;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;


out vec3 v_WorldNorm;
out vec3 v_WorldPos;
out vec4 v_Color;
out vec2 v_UV;

void main(){
	v_WorldNorm = vec3(inverse(transpose(u_MatrixModel)) * vec4(a_Normal, 1));
	v_WorldPos = vec3(u_MatrixModel * vec4(a_Position, 1));
	v_Color = a_Color;
	
	v_UV = a_UV;
	
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
}
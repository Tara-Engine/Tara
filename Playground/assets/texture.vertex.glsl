#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_UV;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

out vec2 v_UV;
void main(){
	v_UV = a_UV;
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
}
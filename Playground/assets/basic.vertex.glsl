#version 450 core
layout(location=0) in vec3 a_Position;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

void main(){
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
}
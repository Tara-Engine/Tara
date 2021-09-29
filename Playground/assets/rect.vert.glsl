#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_UVs;
layout(location=2) in float a_TextureIndex;

out vec2 v_UVs;
out float v_TextureIndex;

uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

void main(){
	v_UVs = a_UVs;
	v_TextureIndex = a_TextureIndex;
	gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1.0);
}
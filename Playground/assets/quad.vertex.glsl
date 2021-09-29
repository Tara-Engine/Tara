#version 450 core

layout(location=0) in vec2 a_Position;
layout(location=1) in vec2 a_Scale;
layout(location=2) in vec2 a_UVmin;
layout(location=3) in vec2 a_UVmax;
layout(location=4) in vec4 a_Color;
layout(location=5) in float a_TextureIndex;

out VS_OUT {
	vec2  Scale;
	vec2  UVmin;
	vec2  UVmax;
	vec4  Color;
	float TextureIndex;
} v_QuadData;


uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

void main(){
	vec4 scalePos = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position + a_Scale, 0.0, 1.0);
	vec4 origPos = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 0.0, 1.0);

	v_QuadData.Scale = vec2(scalePos - origPos);
	v_QuadData.UVmin = a_UVmin;
	v_QuadData.UVmax = a_UVmax;
	v_QuadData.Color = a_Color;
	v_QuadData.TextureIndex = a_TextureIndex;
	//gl_Position = vec4(a_Position, 0.0, 1.0);
	gl_Position = origPos;
}
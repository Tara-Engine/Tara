#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

in VS_OUT {
	mat4  Transform;
	mat4  MatrixMVP;
	vec2  UVmin;
	vec2  UVmax;
	vec4  Color;
	float TextureIndex;
} v_QuadData[];


out GEOM_OUT {
	vec2 UV;
	vec4 Color;
	float TextureIndex;
} v_VertexData;


void main(){
	//make a quad here
	v_VertexData.Color = v_QuadData[0].Color;
	v_VertexData.TextureIndex = v_QuadData[0].TextureIndex;
	
	//0
	gl_Position = v_QuadData[0].MatrixMVP * v_QuadData[0].Transform * vec4(0,0,0,1);
	//gl_Position = v_QuadData[0].Transform * vec4(0,0,0,1);
	v_VertexData.UV = v_QuadData[0].UVmin;
	EmitVertex();
	
	//1
	gl_Position = v_QuadData[0].MatrixMVP * v_QuadData[0].Transform * vec4(1,0,0,1);
	//gl_Position = v_QuadData[0].Transform * vec4(1,0,0,1);
	v_VertexData.UV = vec2(v_QuadData[0].UVmax.x, v_QuadData[0].UVmin.y);
	EmitVertex();
	
	//2
	gl_Position = v_QuadData[0].MatrixMVP * v_QuadData[0].Transform * vec4(0,1,0,1);
	//gl_Position = v_QuadData[0].Transform * vec4(0,1,0,1);
	v_VertexData.UV = vec2(v_QuadData[0].UVmin.x, v_QuadData[0].UVmax.y);
	EmitVertex();
	
	//3
	gl_Position = v_QuadData[0].MatrixMVP * v_QuadData[0].Transform * vec4(1,1,0,1);
	//gl_Position = v_QuadData[0].Transform * vec4(1,1,0,1);
	v_VertexData.UV = v_QuadData[0].UVmax;
	EmitVertex();
	
	EndPrimitive();
	
	
	//vec4 pos = gl_in[0].gl_Position;
	//gl_Position = pos;
	//gl_Position = vec4(pos.x + v_QuadData[0].Scale.x, pos.y, pos.z, pos.w);
	//gl_Position = vec4(pos.x , pos.y+ v_QuadData[0].Scale.y, pos.z, pos.w);
	//gl_Position = vec4(pos.x + v_QuadData[0].Scale.x, pos.y+ v_QuadData[0].Scale.y, pos.z, pos.w);
}
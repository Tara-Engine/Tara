#version 450 core
#extension GL_ARB_bindless_texture : require

layout(location=0)out vec4 color;
#define TEXTURE_COUNT 3



uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

in GEOM_OUT {
	vec2 UV;
	vec4 Color;
	float TextureIndex;
} v_VertexData;

void main(){
	
	if(v_VertexData.TextureIndex < 0){
		color = v_VertexData.Color;
	}
	else{
		sampler2D textures[3] = {u_Texture0, u_Texture1, u_Texture2};
		int index = int(v_VertexData.TextureIndex);
		color = v_VertexData.Color * texture(textures[index], v_VertexData.UV);
	}
}
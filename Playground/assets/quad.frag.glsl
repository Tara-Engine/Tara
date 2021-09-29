#version 450 core
layout(location=0)out vec4 color;
#define TEXTURE_COUNT 3


uniform sampler2D u_Textures[TEXTURE_COUNT];

in GEOM_OUT {
	vec2 UV;
	vec4 Color;
	float TextureIndex;
} v_VertexData;

void main(){
	if(v_VertexData.TextureIndex < 0){
		color = v_VertexData.Color;
	}
	//else if(v_VertexData.TextureIndex > 2) {
	//	color = vec4(1,0,1,1);
	//}
	else{
		//int index = min(max(v_VertexData.TextureIndex, 0),(TEXTURE_COUNT-1));
		int idx = int(v_VertexData.TextureIndex);
		color = v_VertexData.Color * texture(u_Textures[idx], v_VertexData.UV);
		//color = vec4(idx,idx,idx,1.0);
	}
}
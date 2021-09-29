#version 450 core
layout(location=0)out vec4 color;
uniform sampler2D u_Textures[2];
in vec2 v_UVs;
in float v_TextureIndex;
void main(){
	int index = int(v_TextureIndex + 0.1);
	if (index == 1){
		color = texture(u_Textures[index], v_UVs);
	}else{
		color = vec4(1.0,0.0,1.0,1.0);
	}
	//color = vec4(index, index, index, 1.0);
}
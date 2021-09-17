#version 450 core
layout(location=0)out vec4 color;
uniform sampler2D u_Texture;
in vec2 v_UV;
void main(){
	color = texture(u_Texture, v_UV);
}
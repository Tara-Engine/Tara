uniform vec4 tint;

vec4 color(){	
	return vec4(FinalColor,1) * tint;
}
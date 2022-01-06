uniform vec4 tintColor;

vec4 diffuse(){
	return v_Color * tintColor;
}
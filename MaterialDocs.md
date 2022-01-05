# Materials

Materials are special wrappers around a shader, giving parameters to the shader.

A material is created by a string or a file, containing glsl code, that must define a diffuse function, with the following signature:
```glsl
vec4 diffuse(){
	return vec4(0,0,0,1);
}
```
This code can also define uniforms, which become the Material parameters.
This function will be automatically called by the shader's `main()` function (which should not be defined)

Several per-pixel values are defined beyond the GLSL defaults:
```
vec3 v_WorldNorm;
vec3 v_WorldPos;
vec4 v_Color;
vec2 v_UV;
```
These are immutable.
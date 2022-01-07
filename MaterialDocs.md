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

## Lit Materials:
```
vec3 normal()
vec3 diffuse()
vec3 specular()
vec3 emissive()
float metallic()
float roughness()
float ambientOcclusion()

vec3 PixelNormal //predefined

```

# Lighting Materials
```
vec4 color()

uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;
float Metallic         = 0;
float Roughness        = 0;
float AmbientOcclusion = 0;
vec3 Diffuse            = vec3(0);
vec3 Specular           = vec3(0);
vec3 Emissive           = vec3(0);
vec3 WorldSpaceNormal   = vec3(0);
vec3 WorldSpacePosition = vec3(0);
```
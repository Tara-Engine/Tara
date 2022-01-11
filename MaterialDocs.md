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

uniform vec2 u_TargetSize;
uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

uniform int u_LightCount;
uniform vec3 u_LightPositions[128];
uniform vec3 u_LightForwardVectors[128]; //forward vector
uniform vec3 u_LightColors[128];
uniform vec4 u_LightTypeIntensitieCustoms[128]; {type, intensity, custom1, custom2}

float Metallic
float Roughness
float AmbientOcclusion
vec3 Diffuse
vec3 Specular
vec3 Emissive
vec3 WorldSpaceNormal
vec3 WorldSpacePosition

//light types
```

## Light Types
Base [7 floats]
	vec3 Pos
	vec3 Color
	float intensity

PointLight [+5 floats]
	...
	[
		vec3 ForwardDirection
		float SourceRadius
		float SourceLength
	]
	
SpotLight [+4 floats / +5 floats, 1 sampler]
	...
	vec3 ForwardDirection
	float angle
	[
		float falloff
		sampler1d /2d LightTexture
	]
	
DirectionalLight [7 floats]
	vec3 Color
	vec3 ForwardDirection
	float intensity

AmbientLight [4 floats]
	vec3 Color
	float intensity
	
RectLight [+6 floats]
	...
	vec3 ForwardDirection
	float angle
	vec2 size



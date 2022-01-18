# Materials

Materials are special wrappers around a shader, giving parameters to the shader.

A material is created by a string or a file, containing glsl code, that must define at least one function, as shown in the material type sections.

Some pre-defined variables are also supplied, some as uniforms, some as variants, some as manually calculated values.

## Unlit Materials
Unlit materials output their final pixel color, and do not take part in the deferred rendering process. Any lighting you want must be calculated inside of them. They can be transparent or translucent.
```
//You define:
vec4 diffuse();

//predefined
uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

in vec3 v_WorldNorm;
in vec3 v_WorldPos;
in vec4 v_Color;
in vec2 v_UV;
```

## Lit Materials:
Lit materials will have lighting applied as part of the deferred rendering process. They must be opaque

```
//You define:
vec3 normal()
vec3 diffuse()
vec3 specular()
vec3 emissive()
float metallic()
float roughness()
float ambientOcclusion()

//Predefined:
uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

in vec3 v_WorldNorm;
in vec3 v_WorldPos;
in vec4 v_Color;
in vec2 v_UV;

vec3 PixelNormal; //(this is set by the normal() function's results, and holds a zero vector when that function is running.)

```

# Lighting Materials
Lighting materials are used by cameras to light a scene, as part of the deferred rendering process.

```
//You define:
vec4 color() 

//predefined:
uniform vec2 u_TargetSize;
uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

uniform vec3 u_LightPosition;
uniform vec3 u_LightForwardVector; //forward vector
uniform vec3 u_LightColor;
uniform int u_LightType;
uniform float u_LightIntensity;
uniform float u_LightParam1;
uniform float u_LightParam2;
uniform float u_LightRadius;
uniform mat4 u_LightProjectionMatrix;
uniform sampler2D u_LightDepthMapPlanar;
uniform samplerCube u_LightDepthMapPanoramic;
uniform float u_LightDepthMapSize;

vec2 UVs; //this is calculated from the fragment location on the screen, not passed from vertex shader.

float Metallic
float Roughness
float AmbientOcclusion
vec3 Diffuse
vec3 Specular
vec3 Emissive
vec3 WorldSpaceNormal
vec3 WorldSpacePosition
float Depth

//light types
const int LightType_Point		= 0;			
const int LightType_Spot		= 1;			
const int LightType_Directional	= 2;			
const int LightType_Ambient		= 3;			
const int LightType_Rect		= 4;		
```

# Material Includes
There are several things that can be `#include`d into a material, like in C++. These are generally function collections.

## phong
`#include <phong>`
gives access to the `Phong(...)` function.

Functions:
```
vec2 Phong(float attenuationLinear, float attenuationQuadratic);
```
The Phong function calculates Phong lighting for the light, with the provided attentuation factors.
Good default values are `(0.07, 0.017)`. As for the return value it is a vec2 of either `{diffuseFactor, specularFactor}` or `{ambientFactor, 0}`, depending on the light type. (ie, the first for all lights but AmbientLight, and the second for AmbientLight). Since both diffuse and ambient factors are usually treated in the same way (multiplied by the light color, and the diffuse color of the material) this is fairly safe to ignore.




## Light Types
Base [7 floats]
	vec3 Pos
	vec3 Color
	float intensity

PointLight [+0 floats / +5 floats]
	...
	[
		vec3 ForwardDirection
		float SourceRadius
		float SourceLength
	]
	
SpotLight [+5 floats / +5 floats, 1 sampler]
	...
	vec3 ForwardDirection
	float angle
	float innterAngle
	[
		sampler1d /2d LightTexture
	]
	
DirectionalLight [7 floats]
	vec3 Color
	vec3 ForwardDirection
	float intensity

AmbientLight [4 floats]
	vec3 Color
	float intensity
	
RectLight [+6 floats] *Not Implemented*
	...
	vec3 ForwardDirection
	float angle
	vec2 size



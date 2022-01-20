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

One thing to be careful about is single-channel textures store their value in the `w` component (ie, the alpha channel). This is likely to apply to metallic, roughness, and AO maps.

# Lighting Materials
Lighting materials are used by cameras to light a scene, as part of the deferred rendering process.

```
//You define:
vec4 color() 

//texture inputs (usually, you don't need to touch these):
uniform sampler2D u_ColorMetallicSampler;
uniform sampler2D u_SpecularRoughnessSampler;
uniform sampler2D u_EmissiveAOSampler;
uniform sampler2D u_WorldSpaceNormalSampler;
uniform sampler2D u_WorldSpacePositionSampler;

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

# PostProcess Materials
PostProcess materials run after the lighting is calculated, and the output of one is the input to the next. (thus the order is important)
They take many of the same scene inputs as lighting materials, but no light data, as well as the final scene (which is what is transferred from one to the next)

```
//You define:
vec4 color() 

//texture inputs (usually, you don't need to touch these):
uniform sampler2D u_ColorMetallicSampler;
uniform sampler2D u_SpecularRoughnessSampler;
uniform sampler2D u_EmissiveAOSampler;
uniform sampler2D u_WorldSpaceNormalSampler;
uniform sampler2D u_WorldSpacePositionSampler;
uniform sampler2D u_FinalColorSampler;

//predefined:
uniform vec2 u_TargetSize;
uniform vec3 u_CameraPositionWS;
uniform vec3 u_CameraForwardVector;

vec2 UVs;

float Metallic
float Roughness
float AmbientOcclusion
vec3 Diffuse
vec3 Specular
vec3 Emissive
vec3 WorldSpaceNormal
vec3 WorldSpacePosition
float Depth
vec3 FinalColor
```

# Material Includes
There are several things that can be `#include`d into a material, like in C++. These are generally function collections.

## phong
`#include <phong>`
gives access to the `Phong(...)` function. ONLY works for Lighting Materials.

Functions:
```
vec2 Phong(float attenuationLinear, float attenuationQuadratic);
```
The Phong function calculates Phong lighting for the light, with the provided attentuation factors.
Good default values are `(0.07, 0.017)`. As for the return value it is a vec2 of either `{diffuseFactor, specularFactor}` or `{ambientFactor, 0}`, depending on the light type. (ie, the first for all lights but AmbientLight, and the second for AmbientLight). Since both diffuse and ambient factors are usually treated in the same way (multiplied by the light color, and the diffuse color of the material) this is fairly safe to ignore.

## shadow
`#include <shadow>`
Gives access to the `Shadow(...)` function, for calculating light shadows. ONLY works for Lighting Materials.

Functions:
```
float Shadow(float diskRadius);
float GetLightDepth(vec3 position);
```
Shadow calculates the shadow multiplier for the current pixel. It can be directly multiplied with the result of calculating the lighting. It is not a boolean, but does contain values between 0.0 and 1.0, depending on the disk radius. (contains optimizations for when the radius is 0.0, in which case it is boolean 0.0 or 1.0). If the current light is not a PointLight, SpotLight, or DirectionalLight, then 1.0 is returned. If the distance to the light is greater than its radius, then 1.0 is returned.

GetLightDepth returns the stored depth for a specific position for the current light. This is normalized to be between 0.0 and 1.0, with 1.0 being equivilent to the light radius. If the current light is not a PointLight, SpotLight, or DirectionalLight, then 1.0 is returned.

## normals
`#include <normals>`
Gives access to several utilities related to normal mapping. Not Material-specific, but it is intended for Lit materials. Normals are represented as vectors with Z being up, and {0,0,1} being the proverbial face normal. In a lit material, they are then converted into the space of the Face normal and tangent internally, once returned from the `normal()` function.

Functions:
```
NormalTexture(sampler2D tex, vec2 uv)
NormalStrength(vec3 normal, float strength)
```
NormalTexture samples a texture as if it were a normalmap. (ie, it performs the convserions from a color to a normalized vector). It should be prefered when sampling a texture that is a normalmap.

NormalStrength is used to strengthen or weaken a normal. When strength = 1, the normal is unchanged. When strength < 1, it is weakened, and when strength > 1, it is strengthened.



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



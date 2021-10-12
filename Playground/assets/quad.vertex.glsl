#version 450 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Rotation;
layout(location=2) in vec3 a_Scale;
layout(location=3) in vec2 a_UVmin;
layout(location=4) in vec2 a_UVmax;
layout(location=5) in vec4 a_Color;
layout(location=6) in float a_TextureIndex;

out VS_OUT {
	mat4  Transform;
	mat4  MatrixMVP;
	vec2  UVmin;
	vec2  UVmax;
	vec4  Color;
	float TextureIndex;
} v_QuadData;


uniform mat4 u_MatrixViewProjection;
uniform mat4 u_MatrixModel;

mat4 Scale(vec3 scalar){
	return mat4(
		scalar.x,0,0,0,
		0,scalar.y,0,0,
		0,0,scalar.z,0,
		0,0,0,1
	);
}
mat4 Translate(vec3 position){
	return mat4(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		position.x,position.y,position.z,1
	);
}

mat4 Rotate(vec3 axis, float angle){
	axis = normalize(axis);
    float s = sin(radians(angle));
    float c = cos(radians(angle));
    float oc = 1.0 - c;
    
    return mat4(
		oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
        0.0,                                0.0,                                0.0,                                1.0
	);
}

mat4 Transform(vec3 position, vec3 rotation, vec3 scale){
	/*Explanation:
	 * SO, this works in order of operation: Scale first, then rotate, then move.
	 * Rotation happens in the extrinsic order or Roll, then Pitch, then Yaw. Or, around Z, then around X, then around Y.
	 * This is parallel to the C++ function Tara::Transform::GetTransformMatrix()
	 */
	mat4 m_scale = Scale(scale);
	mat4 m_pos = Translate(position);
	mat4 m_rot = Rotate(vec3(0.0, 0.0, 1.0), rotation.x) * Rotate(vec3(1.0, 0.0, 0.0), rotation.y) * Rotate(vec3(0.0,1.0,0.0), rotation.z);
	return m_pos * m_rot * m_scale;
}

void main(){
	//mat4 transform = Transform(a_Position, a_Rotation, a_Scale);
	
	v_QuadData.Transform = Transform(a_Position, a_Rotation, a_Scale);
	//v_QuadData.Transform = Translate(a_Position);
	
	v_QuadData.MatrixMVP = u_MatrixViewProjection * u_MatrixModel;
	v_QuadData.UVmin = a_UVmin;
	v_QuadData.UVmax = a_UVmax;
	v_QuadData.Color = a_Color;
	v_QuadData.TextureIndex = a_TextureIndex;
	//gl_Position = vec4(a_Position, 0.0, 1.0);
	gl_Position = vec4(0,0,0,1);
}
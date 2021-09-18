#pragma once
/*This file is intended to define basic math types needed by Tara.
* 
*/

#include "glm/glm.hpp"
#include <glm/ext/quaternion_float.hpp>
#include "glm/ext/quaternion_common.hpp"


#define TRANSFORM_DEFAULT {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f}}

namespace Tara {

	
	/// <summary>
	/// Vector type: 3 floats
	/// </summary>
	using Vector = glm::vec3;
	
	/// <summary>
	/// Rotator type: roll, pitch, yaw.
	/// Roll is spinning around the "Forward" direction (z)
	/// Pitch is spinning around the "Right" direction (x)
	/// Yaw is spinning around the "Up" direction (y)
	/// angles are specified in degrees
	/// </summary>
	struct Rotator {
		float Roll, Pitch, Yaw;

		/// <summary>
		/// create a rotator normally
		/// </summary>
		/// <param name="roll">degree in roll (around z)</param>
		/// <param name="pitch">degree in pitch (around x)</param>
		/// <param name="yaw">degreee is yaw (around y)</param>
		Rotator(float roll, float pitch, float yaw) : Roll(roll), Pitch(pitch), Yaw(yaw) {
			Clamp();
		}
		/// <summary>
		/// create a rotator from angles around axis.
		/// Note that the order is different from normal creation!
		/// </summary>
		/// <param name="vec">the Vector specifiying the angles around the axis</param>
		Rotator(glm::vec3 vec) : Roll(vec.z), Pitch(vec.x), Yaw(vec.y) {
			Clamp();
		}

		/// <summary>
		/// Construct from quaternions
		/// </summary>
		/// <param name="q">the quatenion</param>
		Rotator(glm::quat q);
		

		/// <summary>
		/// clamp the angles in the rotator to be -180 to 180
		/// </summary>
		void Clamp();

		/// <summary>
		/// add two rotators together. Noramlization occurs
		/// </summary>
		/// <param name="other"> the other rotator</param>
		/// <returns>the new rotator</returns>
		Rotator operator+ (const Rotator& other);

		/// <summary>
		/// get the difference of two rotators. Normalization occurs
		/// </summary>
		/// <param name="other">the other rotator</param>
		/// <returns>the new rotator</returns>
		Rotator operator- (const Rotator& other);

		/// <summary>
		/// multiply a rotator by a float. Linear scaling
		/// </summary>
		/// <param name="other">the float</param>
		/// <returns>the new rotator</returns>
		Rotator operator*(const float& other);

		/// <summary>
		/// divide a rotator by a float. Linear scalaing
		/// </summary>
		/// <param name="other">the float</param>
		/// <returns>the new rotator</returns>
		Rotator operator/(const float& other);

		/// <summary>
		/// convert the euler angles to quaternion
		/// </summary>
		/// <returns></returns>
		glm::quat ToQuaternion() const;
	};

	/// <summary>
	/// Struct to represent the transform of a location in 3d space.
	/// Contains Position, Rotation, and Scale data.
	/// Able to be combined (WIP) and have a transform matrix obtained from.
	/// </summary>
	struct Transform {
		Vector Position;
		Rotator Rotation;
		Vector Scale;

		/// <summary>
		/// construct a transform
		/// </summary>
		/// <param name="position">Vecter position, defaults to {0,0,0}</param>
		/// <param name="rotation">Rotator rotation, default to {0,0,0}</param>
		/// <param name="scale">Vector scale, defaults to {1,1,1}</param>
		Transform(Vector position = {0.0f,0.0f,0.0f}, Rotator rotation = { 0.0f,0.0f,0.0f }, Vector scale = { 1.0f,1.0f,1.0f })
			: Position(position), Rotation(rotation), Scale(scale)
		{}

		/// <summary>
		/// Get the transform matrix of the transform
		/// </summary>
		/// <returns>a 4x4 matrix (glm style)</returns>
		glm::mat4 GetTransformMatrix() const;

		/// <summary>
		/// Combine two transforms
		/// </summary>
		/// <param name="other">the other transform</param>
		/// <returns>resulting new transform</returns>
		Transform operator+ (const Transform& other);

		/// <summary>
		/// inverse combine two transforms
		/// </summary>
		/// <param name="other">the transform to remove from the current one</param>
		/// <returns>tje resulting new transform</returns>
		Transform operator- (const Transform& other);
	};


	
}

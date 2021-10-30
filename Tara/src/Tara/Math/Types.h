#pragma once
/*This file is intended to define basic math types needed by Tara.
* 
*/

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <sol/sol.hpp>

//Create the default Transform
//At origin, unrotatied, unscaled.
#define TRANSFORM_DEFAULT Tara::Transform{Tara::Vector{0.0f,0.0f,0.0f},Tara::Rotator{0.0f,0.0f,0.0f},Tara::Vector{1.0f,1.0f,1.0f}}

//Create a Transform, in 2D space
//Params: <x>, <y>, <rot>, <scale x>, <scale y>
#define TRANSFORM_2D(x,y,rot,sx,sy) Tara::Transform{Tara::Vector{x,y,0.0f},Tara::Rotator{rot,0.0f,0.0f},Tara::Vector{sx,sy,1.0f}}


namespace Tara {
	///using Vector = glm::vec3;

	
	/// <summary>
	/// Vector type: 3 floats
	/// </summary>
	struct Vector {
		float x, y, z;

		//small size constructors

		constexpr Vector() : x(0.0f), y(0.0f), z(0.0f) {}
		constexpr Vector(float x) : x(x), y(x), z(x) {}
		constexpr Vector(double x) : x((float)x), y((float)x), z((float)x) {}
		constexpr Vector(int32_t x) : x((float)x), y((float)x), z((float)x) {}

		//Constructors for literally every combination of float, double, and int. Not sure if there is a better way to do this

		constexpr Vector(float x, float y, float z) : x(x), y(y), z(z) {}
		constexpr Vector(float x, float y, int32_t z) : x(x), y(y), z((float)z) {}
		constexpr Vector(float x, float y, double z) : x(x), y(y), z((float)z) {}
		constexpr Vector(float x, double y, float z) : x(x), y((float)y), z(z) {}
		constexpr Vector(float x, double y, double z) : x(x), y((float)y), z((float)z) {}
		constexpr Vector(float x, double y, int32_t z) : x(x), y((float)y), z((float)z) {}
		constexpr Vector(float x, int32_t y, float z) : x(x), y((float)y), z(z) {}
		constexpr Vector(float x, int32_t y, double z) : x(x), y((float)y), z((float)z) {}
		constexpr Vector(float x, int32_t y, int32_t z) : x(x), y((float)y), z((float)z) {}
		constexpr Vector(double x, float y, float z) : x((float)x), y(y), z(z) {}
		constexpr Vector(double x, float y, double z) : x((float)x), y(y), z((float)z) {}
		constexpr Vector(double x, float y, int32_t z) : x((float)x), y(y), z((float)z) {}
		constexpr Vector(double x, double y, float z) : x((float)x), y((float)y), z(z) {}
		constexpr Vector(double x, double y, double z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(double x, double y, int z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(double x, int32_t y, float z) : x((float)x), y((float)y), z(z) {}
		constexpr Vector(double x, int32_t y, double z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(double x, int32_t y, int z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(int32_t x, float y, float z) : x((float)x), y(y), z(z) {}
		constexpr Vector(int32_t x, float y, double z) : x((float)x), y(y), z((float)z) {}
		constexpr Vector(int32_t x, float y, int32_t z) : x((float)x), y(y), z((float)z) {}
		constexpr Vector(int32_t x, double y, float z) : x((float)x), y((float)y), z(z) {}
		constexpr Vector(int32_t x, double y, double z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(int32_t x, double y, int z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(int32_t x, int32_t y, float z) : x((float)x), y((float)y), z(z) {}
		constexpr Vector(int32_t x, int32_t y, double z) : x((float)x), y((float)y), z((float)z) {}
		constexpr Vector(int32_t x, int32_t y, int z) : x((float)x), y((float)y), z((float)z) {}

		//from vectors

		constexpr Vector(glm::vec2 v, float z) :x(v.x), y(v.y), z(z) {}
		constexpr Vector(glm::vec2 v, double z) :x(v.x), y(v.y), z((float)z) {}
		constexpr Vector(glm::vec2 v, int32_t z) :x(v.x), y(v.y), z((float)z) {}
		constexpr Vector(glm::vec3 v) :x(v.x), y(v.y), z(v.z) {}
		constexpr Vector(glm::vec4 v) :x(v.x), y(v.y), z(v.z) {}
		
		//from lua table

		Vector(sol::table);

		//math operators

		Vector operator+(const Vector& other) const;
		Vector operator-(const Vector& other) const;
		Vector operator*(const Vector& other) const;
		Vector operator/(const Vector& other) const;

		Vector operator+(const glm::vec3& other) const;
		Vector operator-(const glm::vec3& other) const;
		Vector operator*(const glm::vec3& other) const;
		Vector operator/(const glm::vec3& other) const;

		Vector operator+(const float& other) const;
		Vector operator-(const float& other) const;
		Vector operator*(const float& other) const;
		Vector operator/(const float& other) const;

		Vector operator+(const double& other) const;
		Vector operator-(const double& other) const;
		Vector operator*(const double& other) const;
		Vector operator/(const double& other) const;

		Vector operator+(const int32_t& other) const;
		Vector operator-(const int32_t& other) const;
		Vector operator*(const int32_t& other) const;
		Vector operator/(const int32_t& other) const;

		Vector& operator+=(const Vector& other);
		Vector& operator-=(const Vector& other);
		Vector& operator*=(const Vector& other);
		Vector& operator/=(const Vector& other);

		Vector& operator+=(const glm::vec3& other);
		Vector& operator-=(const glm::vec3& other);
		Vector& operator*=(const glm::vec3& other);
		Vector& operator/=(const glm::vec3& other);

		Vector& operator+=(const float& other);
		Vector& operator-=(const float& other);
		Vector& operator*=(const float& other);
		Vector& operator/=(const float& other);

		Vector& operator+=(const double& other);
		Vector& operator-=(const double& other);
		Vector& operator*=(const double& other);
		Vector& operator/=(const double& other);

		Vector& operator+=(const int32_t& other);
		Vector& operator-=(const int32_t& other);
		Vector& operator*=(const int32_t& other);
		Vector& operator/=(const int32_t& other);

		//comparison operators

		bool operator==(const Vector& other) const;
		bool operator!=(const Vector& other) const;
		
		//unary operators
		
		Vector operator-() const;

		//casting operators

		operator glm::vec1() const;
		operator glm::vec2() const;
		operator glm::vec3() const;
		operator glm::vec4() const;
		operator glm::ivec1() const;
		operator glm::ivec2() const;
		operator glm::ivec3() const;
		operator glm::ivec4() const;

		//math functions

		/// <summary>
		/// Dot product between this vector and another
		/// </summary>
		/// <param name="other">the other vector</param>
		/// <returns>dot product</returns>
		float Dot(const Vector& other) const;

		/// <summary>
		/// Dot product between two Vectors
		/// </summary>
		/// <param name="a">The first vector</param>
		/// <param name="b">The second vector</param>
		/// <returns>the dot product</returns>
		static float Dot(const Vector& a, const Vector& b);

		/// <summary>
		/// Cross product between this vector and another
		/// </summary>
		/// <param name="other">the other vector</param>
		/// <returns>Cross product</returns>
		Vector Cross(const Vector& other) const;

		/// <summary>
		/// Cross product between two vectors
		/// </summary>
		/// <param name="a">the first vector</param>
		/// <param name="b">the second vector</param>
		/// <returns>the cross product</returns>
		static Vector Cross(const Vector& a, const Vector& b);

		/// <summary>
		/// Get the lengh of a vector
		/// </summary>
		/// <returns>the length</returns>
		float Length() const;
		/// <summary>
		/// Get the squared length of a vector. Faster than the regular length.
		/// </summary>
		/// <returns>the squared length</returns>
		float LengthSq() const;

		/// <summary>
		/// Get the distance between this vector and another
		/// </summary>
		/// <param name="other">the other vector</param>
		/// <returns>the distance</returns>
		float Distance(const Vector& other) const;

		/// <summary>
		/// Get the distance between two vectors
		/// </summary>
		/// <param name="a">the first vector</param>
		/// <param name="b">the second vector</param>
		/// <returns>the distance</returns>
		static float Distance(const Vector& a, const Vector& b);
		
		/// <summary>
		/// Get the squared distance between this vector and another. Faster than the regular distance.
		/// </summary>
		/// <param name="other">the other vector</param>
		/// <returns>the squared distance</returns>
		float DistanceSq(const Vector& other) const;
		
		/// <summary>
		/// Get the squared distance between two vectors. Faster than the regular distance.
		/// </summary>
		/// <param name="a">the first vector</param>
		/// <param name="b">the second vector</param>
		/// <returns>the squared distance</returns>
		static float DistanceSq(const Vector& a, const Vector& b);

		/// <summary>
		/// Normalize this vector, or, make its lenght 1 without changing direction
		/// </summary>
		/// <returns>reference to this vector</returns>
		Vector& Normalize();

		/// <summary>
		/// Get a lua type from a rotator
		/// </summary>
		/// <returns></returns>
		sol::table ToScriptTable() const;
	};


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
		Rotator(float roll, float pitch, float yaw) 
			: Roll(roll), Pitch(pitch), Yaw(yaw) 
		{
			Clamp();
		}
		/// <summary>
		/// create a rotator from angles around axis.
		/// Note that the order is different from normal creation!
		/// </summary>
		/// <param name="vec">the Vector specifiying the angles around the axis</param>
		Rotator(glm::vec3 vec) 
			: Roll(vec.z), Pitch(vec.x), Yaw(vec.y) 
		{
			Clamp();
		}

		/// <summary>
		/// Construct from quaternions
		/// </summary>
		/// <param name="q">the quatenion</param>
		Rotator(glm::quat q);
		
		/// <summary>
		/// Construct a rotator from the equivilent lua type
		/// </summary>
		/// <param name="t"></param>
		Rotator(sol::table t);

		/// <summary>
		/// clamp the angles in the rotator to be -180 to 180
		/// </summary>
		void Clamp();

		/// <summary>
		/// add two rotators together. Noramlization occurs
		/// </summary>
		/// <param name="other"> the other rotator</param>
		/// <returns>the new rotator</returns>
		Rotator operator+ (const Rotator& other) const;

		/// <summary>
		/// get the difference of two rotators. Normalization occurs
		/// </summary>
		/// <param name="other">the other rotator</param>
		/// <returns>the new rotator</returns>
		Rotator operator- (const Rotator& other) const;

		/// <summary>
		/// multiply a rotator by a float. Linear scaling
		/// </summary>
		/// <param name="other">the float</param>
		/// <returns>the new rotator</returns>
		Rotator operator*(const float& other) const;

		/// <summary>
		/// divide a rotator by a float. Linear scalaing
		/// </summary>
		/// <param name="other">the float</param>
		/// <returns>the new rotator</returns>
		Rotator operator/(const float& other) const;

		/// <summary>
		/// convert the euler angles to quaternion
		/// </summary>
		/// <returns></returns>
		glm::quat ToQuaternion() const;

		/// <summary>
		/// Rotate a vector by oneself
		/// </summary>
		/// <param name="vec">the vector to rotate</param>
		/// <returns></returns>
		Vector RotateVector(const Vector& vec) const;

		/// <summary>
		/// Get the forward vector from the rotation
		/// </summary>
		/// <returns>the forward vector</returns>
		inline Vector GetForwardVector() const { return RotateVector({0.0f, 0.0f, 1.0f}); }

		/// <summary>
		/// Get the right vector from the rotation 
		/// </summary>
		/// <returns>the right vector</returns>
		inline Vector GetRightVector() const { return RotateVector({ 1.0f, 0.0f, 0.0f }); }

		/// <summary>
		/// Get the up vector from the rotation
		/// </summary>
		/// <returns></returns>
		inline Vector GetUpVector() const { return RotateVector({ 0.0f, 1.0f, 0.0f }); }

		/// <summary>
		/// get the inverse of a rotator
		/// </summary>
		/// <returns></returns>
		inline Rotator Inverse() const {
			return Rotator{ -Roll, -Pitch, -Yaw };
		};

		/// <summary>
		/// Get a lua type from a rotator
		/// </summary>
		/// <returns></returns>
		sol::table ToScriptTable() const;
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
		/// Construct a transform from a lua script
		/// </summary>
		/// <param name="t"></param>
		Transform(sol::table t);

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
		Transform operator+ (const Transform& other) const;

		/// <summary>
		/// inverse combine two transforms
		/// </summary>
		/// <param name="other">the transform to remove from the current one</param>
		/// <returns>tje resulting new transform</returns>
		Transform operator- (const Transform& other) const;

		/// <summary>
		/// Get a lua type from a rotator
		/// </summary>
		/// <returns></returns>
		sol::table ToScriptTable() const;
	};

	
	
}

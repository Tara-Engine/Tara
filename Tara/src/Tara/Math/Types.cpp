#include "tarapch.h"
#include "Types.h"
#include "Tara/Core/Script.h"
#include <cmath>

namespace Tara{

	Vector::Vector(sol::table t)
	{
		x = t.get_or("x", 0.0f);
		y = t.get_or("y", 0.0f);
		z = t.get_or("z", 0.0f);
	}

	Vector Vector::operator+(const Vector& other) const
	{
		return Vector(x+other.x, y+other.y, z+other.z);
	}

	Vector Vector::operator-(const Vector& other) const
	{
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Vector Vector::operator*(const Vector& other) const
	{
		return Vector(x * other.x, y * other.y, z * other.z);
	}

	Vector Vector::operator/(const Vector& other) const
	{
		return Vector(x / other.x, y / other.y, z / other.z);
	}

	Vector Vector::operator+(const glm::vec3& other) const
	{
		return Vector(x + other.x, y + other.y, z + other.z);
	}

	Vector Vector::operator-(const glm::vec3& other) const
	{
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Vector Vector::operator*(const glm::vec3& other) const
	{
		return Vector(x * other.x, y * other.y, z * other.z);
	}

	Vector Vector::operator/(const glm::vec3& other) const
	{
		return Vector(x / other.x, y / other.y, z / other.z);
	}

	Vector Vector::operator+(const float& other) const
	{
		return Vector(x+other, y+other, z+other);
	}

	Vector Vector::operator-(const float& other) const
	{
		return Vector(x - other, y - other, z - other);
	}

	Vector Vector::operator*(const float& other) const
	{
		return Vector(x * other, y * other, z * other);
	}

	Vector Vector::operator/(const float& other) const
	{
		return Vector(x / other, y / other, z / other);
	}

	Vector Vector::operator+(const double& other) const
	{
		return Vector(x + (float)other, y + (float)other, z + (float)other);
	}

	Vector Vector::operator-(const double& other) const
	{
		return Vector(x - (float)other, y - (float)other, z - (float)other);
	}

	Vector Vector::operator*(const double& other) const
	{
		return Vector(x * (float)other, y * (float)other, z * (float)other);
	}

	Vector Vector::operator/(const double& other) const
	{
		return Vector(x / (float)other, y / (float)other, z / (float)other);
	}

	Vector Vector::operator+(const int32_t& other) const
	{
		return Vector(x + other, y + other, z + other);
	}

	Vector Vector::operator-(const int32_t& other) const
	{
		return Vector(x - other, y - other, z - other);
	}

	Vector Vector::operator*(const int32_t& other) const
	{
		return Vector(x * other, y * other, z * other);
	}

	Vector Vector::operator/(const int32_t& other) const
	{
		return Vector(x / other, y / other, z / other);
	}

	Vector& Vector::operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector& Vector::operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector& Vector::operator*=(const Vector& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector& Vector::operator/=(const Vector& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vector& Vector::operator+=(const glm::vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector& Vector::operator-=(const glm::vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector& Vector::operator*=(const glm::vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector& Vector::operator/=(const glm::vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vector& Vector::operator+=(const float& other)
	{
		x += other;
		y += other;
		z += other;
		return *this;
	}

	Vector& Vector::operator-=(const float& other)
	{
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	Vector& Vector::operator*=(const float& other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vector& Vector::operator/=(const float& other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	Vector& Vector::operator+=(const double& other)
	{
		x += (float)other;
		y += (float)other;
		z += (float)other;
		return *this;
	}

	Vector& Vector::operator-=(const double& other)
	{
		x -= (float)other;
		y -= (float)other;
		z -= (float)other;
		return *this;
	}

	Vector& Vector::operator*=(const double& other)
	{
		x *= (float)other;
		y *= (float)other;
		z *= (float)other;
		return *this;
	}

	Vector& Vector::operator/=(const double& other)
	{
		x /= (float)other;
		y /= (float)other;
		z /= (float)other;
		return *this;
	}

	Vector& Vector::operator+=(const int32_t& other)
	{
		x += other;
		y += other;
		z += other;
		return *this;
	}

	Vector& Vector::operator-=(const int32_t& other)
	{
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	Vector& Vector::operator*=(const int32_t& other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vector& Vector::operator/=(const int32_t& other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	bool Vector::operator==(const Vector& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool Vector::operator!=(const Vector& other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	Vector Vector::operator-() const
	{
		return Vector(-x, -y, -z);
	}

	Vector::operator glm::vec1() const
	{
		return glm::vec1(x);
	}

	Vector::operator glm::vec2() const
	{
		return glm::vec2(x, y);
	}

	Vector::operator glm::vec3() const
	{
		return glm::vec3(x, y, z);
	}

	Vector::operator glm::vec4() const
	{
		return glm::vec4(x, y, z, 1.0f);
	}

	Vector::operator glm::ivec1() const
	{
		return glm::ivec1((int)x);
	}

	Vector::operator glm::ivec2() const
	{
		return glm::ivec2((int)x, (int)y);
	}

	Vector::operator glm::ivec3() const
	{
		return glm::ivec3((int)x, (int)y, (int)z);
	}

	Vector::operator glm::ivec4() const
	{
		return glm::ivec4((int)x, (int)y, (int)z, 1);
	}
	

	float Vector::Dot(const Vector& other) const
	{
		return x*other.x + y*other.y + x*other.z;
	}

	float Vector::Dot(const Vector& a, const Vector& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	Vector Vector::Cross(const Vector& other) const
	{
		return Vector(
			y * other.z - other.y * z,
			z * other.x - other.z * x,
			x * other.y - other.x * y
		);
	}

	Vector Vector::Cross(const Vector& a, const Vector& b)
	{
		return Vector(
				a.y * b.z - b.y * a.z,
				a.z * b.x - b.z * a.x,
				a.x * b.y - b.x * a.y
		);
	}

	float Vector::Length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Vector::LengthSq() const
	{
		return x * x + y * y + z * z;
	}

	float Vector::Distance(const Vector& other) const
	{
		return (*this - other).Length();
	}

	float Vector::Distance(const Vector& a, const Vector& b)
	{
		return (a-b).Length();
	}

	float Vector::DistanceSq(const Vector& other) const
	{
		return (*this - other).LengthSq();
	}

	float Vector::DistanceSq(const Vector& a, const Vector& b)
	{
		return (a - b).LengthSq();
	}

	Vector& Vector::Normalize()
	{
		*this /= Length();
		return *this;
	}

	sol::table Vector::ToScriptTable() const
	{
		auto ret = (Script::Get()->GetState()["Vector"]["new"](x, y, z));
		if (!ret.valid()) {
			auto table = sol::table(Script::Get()->GetState(), sol::create);
			table["x"] = x;
			table["y"] = y;
			table["z"] = z;
			return table;
		}
		return ret;
	}





	//algorithm from the wikipedia page on converting between euler angels and quaternions
	Rotator::Rotator(glm::quat q)
	{
		Roll = glm::degrees(atan2(
			2 * (q.w * q.x + q.y * q.z), 
			1 - 2 * (q.x * q.x + q.y * q.y)
		));
		float sinp = 2 * (q.w * q.y + q.z * q.x);
		if (abs(sinp) >= 1) {
			Pitch = std::copysign(90.0f, sinp);
		}
		else {
			Pitch = glm::degrees(asin(sinp));
		}
		Yaw = glm::degrees(atan2(
			2 * (q.w * q.z + q.x * q.y), 
			1 - 2 * (q.y * q.y + q.z * q.z)
		));
		Clamp();
	}

	Rotator::Rotator(sol::table t)
	{
		Roll = t.get_or("Roll", t.get_or("roll", 0.0f));
		Pitch = t.get_or("Pitch", t.get_or("pitch", 0.0f));
		Yaw = t.get_or("Yaw", t.get_or("yaw", 0.0f));
	}

	void Rotator::Clamp()
	{
		Roll = fmod(Roll, 360);
		if (Roll >  180) { Roll -= 360; }
		if (Roll < -180) { Roll += 360; }

		Pitch = fmod(Pitch, 180);
		if (Pitch >  90) { Pitch -= 180; }
		if (Pitch < -90) { Pitch += 180; }

		Yaw = fmod(Yaw, 360);
		if (Yaw >  180) { Yaw -= 360; }
		if (Yaw < -180) { Yaw += 360; }


		//Roll = fmod(Roll, 180.0f);
		//Pitch = fmod(Pitch, 180.0f);
		//Yaw = fmod(Yaw, 180.0f);
	}

	Rotator Rotator::operator+(const Rotator& other) const
	{
		Rotator r = { Roll + other.Roll, Pitch + other.Pitch, Yaw + other.Yaw };
		r.Clamp();
		return r;
	}

	Rotator Rotator::operator-(const Rotator& other) const
	{
		Rotator r = { Roll - other.Roll, Pitch - other.Pitch, Yaw - other.Yaw };
		r.Clamp();
		return r;
	}

	Rotator Rotator::operator*(const float& other) const
	{
		Rotator r = { Roll * other, Pitch * other, Yaw * other};
		r.Clamp();
		return r;
	}


	Rotator Rotator::operator/(const float& other) const
	{
		Rotator r = { Roll / other, Pitch / other, Yaw / other };
		r.Clamp();
		return r;
	}

	//algorithm from the wikipedia page on converting between euler angels and quaternions
	glm::quat Rotator::ToQuaternion() const
	{
		return glm::quat(
			cosf(Roll * 0.5f) * cosf(Pitch * 0.5f) * cosf(Yaw * 0.5f) + sinf(Roll * 0.5f) * sinf(Pitch * 0.5f) * sinf(Yaw * 0.5f),
			sinf(Roll * 0.5f) * cosf(Pitch * 0.5f) * cosf(Yaw * 0.5f) + cosf(Roll * 0.5f) * sinf(Pitch * 0.5f) * sinf(Yaw * 0.5f),
			cosf(Roll * 0.5f) * sinf(Pitch * 0.5f) * cosf(Yaw * 0.5f) + sinf(Roll * 0.5f) * cosf(Pitch * 0.5f) * sinf(Yaw * 0.5f),
			cosf(Roll * 0.5f) * cosf(Pitch * 0.5f) * sinf(Yaw * 0.5f) + sinf(Roll * 0.5f) * sinf(Pitch * 0.5f) * cosf(Yaw * 0.5f)
		);
	}


	Vector Rotator::RotateVector(const Vector& vec) const
	{
		return Vector(glm::rotateY(
			glm::rotateX(
				glm::rotateZ(
					(glm::vec3)vec, glm::radians(Roll)
				), glm::radians(Pitch)
			), glm::radians(Yaw)
		));
	}

	sol::table Rotator::ToScriptTable() const
	{
		//TODO: when lua side library for tables, complete, replace this
		auto table = sol::table(Script::Get()->GetState(), sol::create);
		table["Roll"] = Roll;
		table["Pitch"] = Pitch;
		table["Yaw"] = Yaw;
		return table;
	}


	Transform::Transform(sol::table t)
		:Position(0.0f), Rotation( 0.0f, 0.0f, 0.0f ), Scale(0.0f)
	{
		sol::object pos = t["Position"];
		sol::object rot = t["Rotation"];
		sol::object sca = t["Scale"];
		if (pos.valid() && pos.is<sol::table>()) {
			Position = Vector(pos.as<sol::table>());
		}
		if (rot.valid() && rot.is<sol::table>()) {
			Rotation = Rotator(rot.as<sol::table>());
		}
		if (sca.valid() && sca.is<sol::table>()) {
			Scale = Vector(sca.as<sol::table>());
		}
	}

	glm::mat4 Transform::GetTransformMatrix() const
	{
		/*Explanation:
		* SO, this works in order of operation: Scale first, then rotate, then move.
		* Rotation happens in the extrinsic order or Roll, then Pitch, then Yaw. Or, around Z, then around X, then around Y.
		* Multiplication happens in the opposite order.
		*/

		glm::mat4 scale = glm::scale(glm::mat4(1), (glm::vec3)Scale);
		
		glm::mat4 rot = 
			glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Yaw), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Pitch), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Roll), glm::vec3(0, 0, 1));

		
		glm::mat4 pos = glm::translate(glm::mat4(1), (glm::vec3)Position);
		
		return pos * rot * scale;
	}


	Transform Transform::operator+(const Transform& other) const
	{
		return Transform(Position+ Rotation.RotateVector(other.Position) * Scale, Rotation+other.Rotation, Scale*other.Scale);
	}


	Transform Transform::operator-(const Transform& other) const
	{
		return Transform(Position - Rotation.Inverse().RotateVector(other.Position) / Scale, Rotation - other.Rotation, Scale / other.Scale);
	}

	Transform Transform::operator-() const
	{
		return Transform(-Position, Rotation.Inverse(), -Scale);
	}

	sol::table Transform::ToScriptTable() const
	{
		//TODO: when lua side library for tables, complete, replace this
		auto table = sol::table(Script::Get()->GetState(), sol::create);
		table["Position"] = Position.ToScriptTable();
		table["Rotation"] = Rotation.ToScriptTable();
		table["Scale"] = Scale.ToScriptTable();
		return table;
	}

}

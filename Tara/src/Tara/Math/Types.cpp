#include "tarapch.h"
#include "Types.h"
#include <cmath>

namespace Tara{

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

	void Rotator::Clamp()
	{
		Roll = fmod(Roll, 180.0f);
		Pitch = fmod(Pitch, 180.0f);
		Yaw = fmod(Yaw, 180.0f);
	}

	Rotator Rotator::operator+(const Rotator& other)
	{
		Rotator r = { Roll + other.Roll, Pitch + other.Pitch, Yaw + other.Yaw };
		r.Clamp();
		return r;
	}

	Rotator Rotator::operator-(const Rotator& other)
	{
		Rotator r = { Roll - other.Roll, Pitch - other.Pitch, Yaw - other.Yaw };
		r.Clamp();
		return r;
	}

	Rotator Rotator::operator*(const float& other)
	{
		Rotator r = { Roll * other, Pitch * other, Yaw * other};
		r.Clamp();
		return r;
	}


	Rotator Rotator::operator/(const float& other)
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
		return glm::rotateY(
			glm::rotateX(
				glm::rotateZ(
					vec, glm::radians(Roll)
				), glm::radians(Pitch)
			), glm::radians(Yaw)
		);
	}


	glm::mat4 Transform::GetTransformMatrix() const
	{
		/*Explanation:
		* SO, this works in order of operation: Scale first, then rotate, then move.
		* Rotation happens in the extrinsix order or Roll, then Pitch, then Yaw. Or, around Z, then around X, then around Y.
		*/

		glm::mat4 scale = glm::scale(glm::mat4(1), Scale);
		
		glm::mat4 rot = glm::rotate(
			glm::rotate(
				glm::rotate(
					glm::mat4(1), 
					glm::radians(Rotation.Roll), 
					{ 0.0f, 0.0f, 1.0f }
				), 
				glm::radians(Rotation.Pitch),
				{ 1.0f, 0.0f, 0.0f }
			), 
			glm::radians(Rotation.Yaw),
			{ 0.0f, 1.0f, 0.0f }
		);
		
		glm::mat4 pos = glm::translate(glm::mat4(1), Position);
		

		return pos * rot * scale;

		//whole nasty thing in one line. (except its somehow broken, causing scale to move things too.)
		//return glm::translate(glm::rotate(glm::rotate(glm::rotate(glm::scale(glm::mat4(1), Scale),Rotation.Roll,{ 0.0f, 0.0f, 1.0f }),Rotation.Pitch,{ 1.0f, 0.0f, 0.0f }),Rotation.Yaw,{ 0.0f, 1.0f, 0.0f }), Position);
	}


	Transform Transform::operator+(const Transform& other)
	{
		return Transform(Position+ Rotation.RotateVector(other.Position) * Scale, Rotation+other.Rotation, Scale*other.Scale);
	}


	Transform Transform::operator-(const Transform& other)
	{
		return Transform(Position - Rotation.Inverse().RotateVector(other.Position) / Scale, Rotation - other.Rotation, Scale / other.Scale);
	}
}

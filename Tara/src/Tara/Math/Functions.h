#pragma once



namespace Tara {
	
	/// <summary>
	/// Check if an Integer is a power of 2 (ie, 1 bit is true, the rest false)
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	inline int PowOfTwo(int x) {
		return ((x != 0) && !(x & (x - 1)));
	}


	/// <summary>
	/// Map a value in one range to another range
	/// </summary>
	/// <param name="a">the value</param>
	/// <param name="minA">minimum of the origin range</param>
	/// <param name="maxA">maximum of the origin range</param>
	/// <param name="minB">minimum of the destination range</param>
	/// <param name="maxB">maximum of the destination range</param>
	/// <returns></returns>
	inline float MapRange(float a, float minA, float maxA, float minB, float maxB) {
		return minB + (((maxB - minB) / (maxA - minA)) * (a - minA));
	}

	/// <summary>
	/// Linear Interpolate two values
	/// </summary>
	/// <typeparam name="T">Any type than can be multiplied with a float and result in itself</typeparam>
	/// <param name="a">The origin</param>
	/// <param name="b">The destination</param>
	/// <param name="alpha">0 to 1 value placing the result between the origin and destination</param>
	/// <returns></returns>
	template<typename T>
	inline T Lerp(T a, T b, float alpha) {
		return a * (1 - alpha) + b * alpha;
	}

	/// <summary>
	/// Cubic Interpolate two values
	/// </summary>
	/// <typeparam name="T">Any type than can be multiplied with a float and result in itself</typeparam>
	/// <param name="a">The origin</param>
	/// <param name="b">The destination</param>
	/// <param name="alpha">0 to 1 value placing the result between the origin and destination</param>
	/// <returns></returns>
	template<typename T>
	inline T CubicInterp(T a, T b, float alpha) {
		float v = 3 * alpha * alpha - 2 * alpha * alpha * alpha;
		return a * (1 - v) + b * v;
	}

	/// <summary>
	/// PolyInterpolation (fifth-degree polynomial)
	/// This interpolation offers better results for smoothness, particularly due to having 
	/// a flat normal at the endpoints (2nd derrivative is 0). It is, however, more expensive than any others.
	/// </summary>
	/// <typeparam name="T">Any type than can be multiplied with a float and result in itself</typeparam>
	/// <param name="a">The origin</param>
	/// <param name="b">The destination</param>
	/// <param name="alpha">0 to 1 value placing the result between the origin and destination</param>
	/// <returns></returns>
	template<typename T>
	inline T PolyInterp(T a, T b, float alpha) {
		float v = 6.0f * powf(alpha, 5.0f) - 15.0f * powf(alpha, 4.0f) + 10.0f * powf(alpha, 2.0f);
		return a * (1 - v) + b * v;
	}

	/// <summary>
	/// Nearly Equal function. Check if two things are within Delta of each other
	/// </summary>
	/// <returns></returns>
	inline bool NearlyEqual(float a, float b, float delta = 0.001) {
		return abs(a - b) < delta;
	}

	/// <summary>
	/// Nearly Equal function. Check if two things are within Delta of each other
	/// </summary>
	/// <returns></returns>
	inline bool NearlyEqual(glm::vec2 a, glm::vec2 b, float delta = 0.001) {
		return abs(a - b).x < delta && abs(a - b).y < delta;
	}

	/// <summary>
	/// Nearly Equal function. Check if two things are within Delta of each other
	/// </summary>
	/// <returns></returns>
	inline bool NearlyEqual(glm::vec3 a, glm::vec3 b, float delta = 0.001) {
		return abs(a - b).x < delta && abs(a - b).y < delta && abs(a - b).z < delta;
	}

	/// <summary>
	/// Nearly Equal function. Check if two things are within Delta of each other
	/// </summary>
	/// <returns></returns>
	inline bool NearlyEqual(Vector a, Vector b, float delta = 0.001) {
		return abs(a - b).x < delta && abs(a - b).y < delta && abs(a - b).z < delta;
	}

	/// <summary>
	/// Nearly Equal function. Check if two things are within Delta of each other
	/// </summary>
	/// <returns></returns>
	inline bool NearlyEqual(glm::vec4 a, glm::vec4 b, float delta = 0.001) {
		return abs(a - b).x < delta && abs(a - b).y < delta && abs(a - b).z < delta && abs(a - b).w < delta;
	}

	
}
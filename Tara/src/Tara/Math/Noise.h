#pragma once
#include "tarapch.h"

namespace Tara {

	/// <summary>
	/// OpenSimplexNoise
	/// 
	/// based off of the implementation by Stefan Gustavson
	/// https://github.com/stegu
	/// and his paper on the topic,
	/// https://weber.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
	/// 
	/// </summary>
	class Noise {

	public:
		/// <summary>
		/// Construct a new Noise object
		/// </summary>
		/// <param name="seed">the seed, an integer</param>
		/// <param name="frequency">scale of the noise</param>
		/// <param name="amplitude">variation of the noise (ie, in the range of -amplitude to amplitude</param>
		/// <param name="ocatves">how much detail</param>
		Noise(uint32_t seed = 0, float frequency = 0.1f, float amplitude = 1.0f, float persistance = 0.5f, uint32_t octaves = 8);

		
		~Noise();

		/// <summary>
		/// 1D Noise function
		/// </summary>
		/// <param name="x">the x coordinate</param>
		/// <returns>Noise value</returns>
		float operator() (float x);
		
		/// <summary>
		/// 2D Noise function
		/// </summary>
		/// <param name="x"><the x coordinate/param>
		/// <param name="y">the y coordinate</param>
		/// <returns>Noise value</returns>
		float operator() (float x, float y);
		
		/// <summary>
		/// 3D Noise Function
		/// </summary>
		/// <param name="x">the x coordinate</param>
		/// <param name="y">the y coordinate</param>
		/// <param name="z">the z coordinate</param>
		/// <returns>Noise value</returns>
		float operator() (float x, float y, float z);
		
		/// <summary>
		/// 4D Noise Function
		/// </summary>
		/// <param name="x">the x coordinate</param>
		/// <param name="y">the y coordinate</param>
		/// <param name="z">the z coordinate</param>
		/// <param name="w">the w coordinate</param>
		/// <returns>Noise value</returns>
		float operator() (float x, float y, float z, float w);

		/// <summary>
		/// 2D Noise, vector coordinates
		/// </summary>
		/// <param name="pos">the position vector</param>
		/// <returns>Noise value</returns>
		inline float operator() (glm::vec2 pos) { return(*this)(pos.x, pos.y); }
		
		/// <summary>
		/// 3D Noise, vector coordinates
		/// </summary>
		/// <param name="pos">the position vector</param>
		/// <returns>Noise value</returns>
		inline float operator() (glm::vec3 pos) { return(*this)(pos.x, pos.y, pos.z); }
		
		/// <summary>
		/// 4D Noise, vector coordinates
		/// </summary>
		/// <param name="pos">the position vector</param>
		/// <returns>Noise value</returns>
		inline float operator() (glm::vec4 pos) { return(*this)(pos.x, pos.y, pos.z, pos.w); }
	
	private:

		float get (float x);
		float get (float x, float y);
		float get (float x, float y, float z);
		float get (float x, float y, float z, float w);

		static float  gradient(int hash, float x);
		static float  gradient(int hash, float x, float y);
		static float  gradient(int hash, float x, float y, float z);
		static float  gradient(int hash, float x, float y, float z, float t);

	private:
		uint32_t m_Seed;
		float m_Frequency;
		float m_Amplitude;
		float m_Persistance;
		uint32_t m_Octaves;
		std::array<uint8_t, 512> m_Permutation;
	};

}
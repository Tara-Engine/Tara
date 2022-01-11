#pragma once
#include "Tara/Math/Types.h"

namespace Tara {

	/// <summary>
	/// Light Type
	/// </summary>
	enum class LightType {
		POINT		= 0, 
		SPOT		= 1, 
		DIRECTIOAL	= 2, 
		AMBIENT		= 3, 
		RECT		= 4
	};

	/// <summary>
	/// Generalized data for a light
	/// </summary>
	struct LightData {
		Vector Position;
		Vector Color;
		float Intensity;
		LightType Type;
		Vector ForwardVector;
		float Custom1;
		float Custom2;
		//Texture2Dref FalloffTexture

		LightData(Vector pos, Vector color, float intensity, LightType type, Vector forward = {0,0,0}, float custom1 = 0.0f, float custom2 = 0.0f)
			:Position(pos), Color(color), Intensity(intensity), Type(type), ForwardVector(forward), Custom1(custom1), Custom2(custom2)
		{}
	};

	/// <summary>
	/// pure virtual Base class for all LightEntities. They have this in addition to Entity
	/// </summary>
	class LightBase {
	public:
		LightBase(Vector color, float intensity, LightType type)
			: m_LightColor(color), m_LightIntensity(intensity), m_LightType(type)
		{}

		/// <summary>
		/// Pure Virutal function to Get generalized light data
		/// </summary>
		/// <returns></returns>
		virtual LightData GetLightData() = 0;

		/// <summary>
		/// Get the Light Color
		/// </summary>
		/// <returns></returns>
		virtual Vector GetLightColor() { return m_LightColor; }

		/// <summary>
		/// Get the Light Intensity
		/// </summary>
		/// <returns></returns>
		virtual float GetLightIntensity() { return m_LightIntensity; }

		/// <summary>
		/// Get the light type
		/// </summary>
		/// <returns></returns>
		virtual LightType GetLightType() { return m_LightType; }

		/// <summary>
		/// Set the Light Color
		/// </summary>
		/// <param name="color"></param>
		virtual void SetLightColor(Vector color) { m_LightColor = color; }

		/// <summary>
		/// Set the Light Intensity
		/// </summary>
		/// <param name="intensity"></param>
		virtual void SetLightIntensity(float intensity) { m_LightIntensity = intensity; }

		//intentional no setter for m_LightType

		//intentionally protected so that the derrived classes can directly interact
	protected:
		/// <summary>
		/// The Color of the Light
		/// </summary>
		Vector m_LightColor;
		/// <summary>
		/// The Intensity of the Light
		/// </summary>
		float m_LightIntensity;
		/// <summary>
		/// The Type of the Light
		/// Intentionally not const, though it could be
		/// due to leaving the freedom of creating custom 
		/// light entities that can change type
		/// </summary>
		LightType m_LightType;
	};

}
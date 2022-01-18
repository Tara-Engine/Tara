#pragma once
#include "Tara/Math/Types.h"

namespace Tara {
	REFTYPE(LightBase);

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
		float MaxRadius;
		LightType Type;
		Vector ForwardVector;
		float Custom1;
		float Custom2;
		//Texture2Dref FalloffTexture

		LightData(Vector pos, Vector color, float intensity, float maxRadius, LightType type, Vector forward = { 0,0,0 }, float custom1 = 0.0f, float custom2 = 0.0f)
			:Position(pos), Color(color), Intensity(intensity), MaxRadius(maxRadius), Type(type), ForwardVector(forward), Custom1(custom1), Custom2(custom2)
		{}
	};

	/// <summary>
	/// pure virtual Base class for all LightEntities. They have this in addition to Entity
	/// </summary>
	class LightBase /* : std::enable_shared_from_this<LightBase>*/ {
	public:
		LightBase(Vector color, float intensity, float radius, LightType type)
			: m_LightColor(color), m_LightIntensity(intensity), m_LightRadius(radius), m_LightType(type)
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
		/// Get the Light Effect radius
		/// </summary>
		/// <returns></returns>
		virtual float GetLightRadius() { return m_LightRadius; }

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

		/// <summary>
		/// Set the Light Effect Radius
		/// </summary>
		/// <param name="radius"></param>
		virtual void SetLightRadius(float radius) { m_LightRadius = radius; }

		/// <summary>
		/// Set the light radius, based on its intensity
		/// </summary>
		/// <param name="constant">constant falloff factor, normally 1.0</param>
		/// <param name="linear">linear falloff factor, defaults to 0.07</param>
		/// <param name="quadratic">quadratic falloff factor, defaults to 0.017</param>
		virtual void SetLightRadiusFromIntensity(float constant = 1.0f, float linear = 0.07, float quadratic = 0.017) {
			m_LightRadius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / 1.0f) * m_LightIntensity))) / (2 * quadratic);
		}

		/// <summary>
		/// Get the Depth Target for a light
		/// </summary>
		/// <returns></returns>
		virtual RenderTargetRef GetDepthTarget() { return nullptr; };

		/// <summary>
		/// Get the projection matrix for this light
		/// </summary>
		/// <returns></returns>
		inline virtual glm::mat4 GetLightProjectionMatrix() { return glm::mat4(1.0); };

		/// <summary>
		/// Get if the light should render its depth buffer
		/// </summary>
		/// <returns></returns>
		inline virtual bool ShouldDrawDepth() { return false; }

		/// <summary>
		/// This should return true if and only if the depth is panoramic.
		/// </summary>
		/// <returns></returns>
		inline virtual bool DepthIsPanoramic() { return false; }

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
		/// The effect area of the light
		/// </summary>
		float m_LightRadius;

		/// <summary>
		/// The Type of the Light
		/// Intentionally not const, though it could be
		/// due to leaving the freedom of creating custom 
		/// light entities that can change type
		/// </summary>
		LightType m_LightType;
	};

}
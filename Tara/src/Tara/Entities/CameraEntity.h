#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Camera.h"

namespace Tara {
	
	REFTYPE(CameraEntity)
	NOREFTYPE(CameraEntity)

	/// <summary>
	/// A Entity used to hold and control a camera in the world
	/// Dynamically changes the underlying camera type based on the selected projection type
	/// </summary>
	class CameraEntity : public Entity {
	public:
		/// <summary>
		/// Constructor for a CameraEntiy
		/// Note: you should use the Tara::CreateEntity function instead, to get full funcionality.
		/// </summary>
		/// <param name="parent">the parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="projectionType">the projection type of the camera</param>
		/// <param name="transform">the transform of the camera</param>
		/// <param name="name">the name of the camera</param>
		CameraEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Camera::ProjectionType projectionType, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "CameraEntity");

		/// <summary>
		/// Get the projection type of the current camera
		/// </summary>
		/// <returns>the projection type</returns>
		inline Camera::ProjectionType GetProjectionType() const { 
			if (m_Camera) { 
				return m_Camera->GetProjectionType(); 
			}
			else {
				return Camera::ProjectionType::None;
			}
		}

		/// <summary>
		/// Set the projection type of the camera
		/// This will replace the current camera if the current camera's 
		/// projection type does not match the supplied type
		/// </summary>
		/// <param name="type">the new projection type</param>
		void SetProjectionType(Camera::ProjectionType type);

		/// <summary>
		/// When in orthographic mode, set the orthographic camera extent
		/// </summary>
		/// <param name="extent">the new extent</param>
		void SetOrthographicExtent(OrthographicCamera::OrthoExtent extent);

		/// <summary>
		/// When in orthographic mode, set the orthographic camera extent via a width
		/// </summary>
		/// <param name="width">the new width</param>
		void SetOrthographicExtent(float width);

		/// <summary>
		/// Set if the camera should use the world scale for its projection matrix
		/// </summary>
		/// <param name="v"></param>
		inline void SetUseWorldScale(bool v) { m_UseWorldScale = v; }

		/// <summary>
		/// Get if the camera should use the world scale for its projection matrix
		/// </summary>
		/// <returns></returns>
		inline bool GetUseWorldScale() const { return m_UseWorldScale; }

		/// <summary>
		/// Get the camera being used by this entity
		/// </summary>
		/// <returns>the camera</returns>
		inline const CameraRef& GetCamera()const { return m_Camera; }

		/// <summary>
		/// The overriden update function
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void OnUpdate(float deltaTime) override;

	private:
		/// <summary>
		/// the internal camera
		/// </summary>
		CameraRef m_Camera;

		/// <summary>
		/// Orhtographic - specific
		/// </summary>
		OrthographicCamera::OrthoExtent m_OrthoExtent;

		/// <summary>
		/// Use the world scale of the camera for the projection matrix
		/// </summary>
		bool m_UseWorldScale = false;
	};
	
}
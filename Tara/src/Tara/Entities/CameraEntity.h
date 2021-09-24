#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Camera.h"

namespace Tara {
	
	/// <summary>
	/// A Entity used to hold and control a camera in the world
	/// Dynamically changes the underlying camera type based on the selected projection type
	/// </summary>
	class CameraEntity : public Entity {
	public:
		using CameraEntityRef = std::shared_ptr<CameraEntity>;
	public:
		/// <summary>
		/// Constructor for a CameraEntiy
		/// Note: you should use the static Create function instead, to get full funcionality.
		/// </summary>
		/// <param name="parent">the parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="projectionType">the projection type of the camera</param>
		/// <param name="transform">the transform of the camera</param>
		/// <param name="name">the name of the camera</param>
		CameraEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Camera::ProjectionType projectionType, Tara::Transform transform, std::string name);

		/// <summary>
		/// Create a new CameraEntity
		/// </summary>
		/// <param name="parent">the parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="projectionType">the projection type of the camera</param>
		/// <param name="transform">the transform of the camera</param>
		/// <param name="name">the name of the camera</param>
		/// <returns>the new CameraEntity, as a reference</returns>
		static CameraEntityRef Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Camera::ProjectionType projectionType, Tara::Transform transform, std::string name);

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
	};
	
	/// <summary>
	/// A Reference to a CameraEntity, an Entity used to hold and control a camera in the world
	/// Dynamically changes the underlying camera type based on the selected projection type
	/// </summary>
	using CameraEntityRef = std::shared_ptr<CameraEntity>;
}
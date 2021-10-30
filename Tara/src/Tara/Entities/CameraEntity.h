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
		/// When in perspective mode, set the perspective FOV
		/// </summary>
		/// <param name="fov"></param>
		void SetPerspectiveFOV(float fov);

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

		/// <summary>
		/// overriden begin play function, to enable events
		/// </summary>
		virtual void OnBeginPlay() override;

		/// <summary>
		/// Overriden event function, to update the render area
		/// </summary>
		/// <param name="e"></param>
		virtual void OnEvent(Event& e) override;


	public:
		//Lua Stuff

		void __SCRIPT__SetProjectionType(const char* type);
		std::string __SCRIPT__GetProjectionType() const;
		void __SCRIPT__SetOrthographicExtent(sol::object extent);

		/// <summary>
		/// Register the lua type
		/// </summary>
		/// <param name="lua"></param>
		static void RegisterLuaType(sol::state& lua);

	private:
		CameraRef m_Camera;
		OrthographicCamera::OrthoExtent m_OrthoExtent;
		float m_PerspectiveFOV;
		bool m_UseWorldScale = false;
	};
	
}
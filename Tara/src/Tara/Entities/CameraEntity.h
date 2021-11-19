#pragma once
#include "Tara/Core/Entity.h"
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
		/// Set if the internal camera should be auto-resized to match the window size.
		/// Will resize any RenderTarget attached to that camera.
		/// </summary>
		/// <param name="mimic"></param>
		void SetMimicWindowSize(bool mimic) { m_MimicWindowSize = mimic; }

		/// <summary>
		/// Set if the camera should be rendered from every frame
		/// </summary>
		/// <param name="render"></param>
		void SetRenderEveryFrame(bool render) { m_RenderEveryFrame = render; }

		/// <summary>
		/// Set if the camera should be rendered from next frame.
		/// </summary>
		/// <param name="render"></param>
		void SetRenderNextFrame(bool render) { m_RenderNextFrame = render; }

		/// <summary>
		/// Get if the internal camera should be auto-resized to match the window size.
		/// </summary>
		/// <returns></returns>
		bool GetMimicWindowSize() const { return m_MimicWindowSize; }

		/// <summary>
		/// Get if the camera should be rendered from every frame
		/// </summary>
		/// <returns></returns>
		bool GetRenderEveryFrame() const { return m_RenderEveryFrame; }

		/// <summary>
		/// Get if the camera should be rendered from next frame.
		/// </summary>
		/// <returns></returns>
		bool GetRenderNextFrame() const { return m_RenderNextFrame; }

		/// <summary>
		/// Get the internal camera render target
		/// </summary>
		/// <returns></returns>
		const RenderTargetRef& GetRenderTarget() const { return m_Camera->GetRenderTarget(); }

		/// <summary>
		/// Set the internal camera render target
		/// </summary>
		/// <param name="target"></param>
		void SetRenderTarget(const RenderTargetRef& target) { m_Camera->SetRenderTarget(target); }

		/// <summary>
		/// Get a ray from the camera's position and rotation
		/// </summary>
		/// <param name="x">screen coord x</param>
		/// <param name="y">screen coord y</param>
		/// <returns></returns>
		std::pair<Vector, Vector> GetRayFromScreenCoordinate(float x, float y) const { return m_Camera->GetRayFromScreenCoordinate(x, y); }

		/// <summary>
		/// Get the filtering bits for what cameras can be render this entity
		/// </summary>
		/// <returns></returns>
		inline virtual uint32_t GetRenderFilterBits() const { return m_Camera->GetRenderFilterBits(); }

		/// <summary>
		/// Set the filtering bits for what cameras can be render this entity
		/// </summary>
		/// <param name="bits"></param>
		inline virtual void SetRenderFilterBits(uint32_t bits) { m_Camera->SetRenderFilterBits(bits); }

	public:
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
		std::pair<sol::table, sol::table> __SCRIPT__GetRayFromScreenCoordinate(float x, float y) const;

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
		bool m_MimicWindowSize = true;
		bool m_RenderEveryFrame = false;
		bool m_RenderNextFrame = false;
	};
	
}
#pragma once
#include "Tara/Math/Types.h"
#include "Tara/Renderer/RenderTarget.h"
#include "Tara/Renderer/MaterialBase.h"

namespace Tara {

	REFTYPE(Camera)
	REFTYPE(OrthographicCamera)
	REFTYPE(PerspectiveCamera)
	REFTYPE(ScreenCamera)

	/// <summary>
	/// Camera Base Class
	/// This class is subclasses to make each camera type
	/// </summary>
	class Camera {
	public:
		/// <summary>
		/// The type of projection that this camera does.
		/// None leavs the projection matrix to identity. This is the base camera behavior
		/// Orthographic does orthographic projection in worldspace
		/// Perspective does prespective projection is worldspace
		/// Screen does orthographic projection in windowspace (pixel coordinates)
		/// </summary>
		enum class ProjectionType : uint8_t {
			None = 0, Ortographic, Perspective, Screen
		};

	public:
		/// <summary>
		/// Empty default constructor
		/// changing the type here does not change projection matrix.
		/// It will only lead to undefined behavior.
		/// </summary>
		Camera(ProjectionType type = ProjectionType::None)
			: m_Transform(), 
			m_Type(type),
			m_ProjectionMatrix(1),
			m_RenderTarget(nullptr),
			m_RenderFilterBits(~0),
			m_NearClipPlane(-1),
			m_FarClipPlane(1)
		{}

		virtual ~Camera() = default;
		
		/// <summary>
		/// Set the world Transform of a camera
		/// </summary>
		/// <param name="t">the new transform</param>
		void SetTransform(Transform t) { m_Transform = t; }
		
		/// <summary>
		/// Set the world Position of a camera
		/// </summary>
		/// <param name="pos">the new position</param>
		void SetPosition(Vector pos) { m_Transform.Position = pos; }
		
		/// <summary>
		/// Set the world rotation of a camera
		/// </summary>
		/// <param name="rot">the new rotation</param>
		void SetRotation(Rotator rot) { m_Transform.Rotation = rot; }
		
		/// <summary>
		/// Get the world Transform of a camera
		/// </summary>
		/// <returns>the transform</returns>
		Transform GetTransform() const { return m_Transform; }
		
		/// <summary>
		/// Get the world Position of a camera
		/// </summary>
		/// <returns>the position</returns>
		Vector GetPosition() const { return m_Transform.Position; }
		
		/// <summary>
		/// Get the world Rotation of a camera
		/// </summary>
		/// <returns>The rotation</returns>
		Rotator GetRotation() const { return m_Transform.Rotation; }
		
		/// <summary>
		/// Get the camera's projection type
		/// </summary>
		/// <returns>the propjection type</returns>
		ProjectionType GetProjectionType() const { return m_Type; }
		
		/// <summary>
		/// Get the projection matrix of the camera
		/// </summary>
		/// <returns>4x4 projection matrix, column-major</returns>
		virtual glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		
		/// <summary>
		/// Get the view matrix of the camera
		/// </summary>
		/// <returns>view matrix, column-major</returns>
		virtual glm::mat4 GetViewMatrix() const { return glm::inverse(m_Transform.GetTransformMatrix()); }
		
		/// <summary>
		/// Get the projection and view matrix pre-combined
		/// </summary>
		/// <returns>projection*view matrix, column-major</returns>

		virtual glm::mat4 GetViewProjectionMatrix() const {return GetProjectionMatrix() * GetViewMatrix();}
		
		/// <summary>
		/// Set the near clipping plane. Silently fails if <= 0
		/// </summary>
		/// <param name="plane"></param>
		inline void SetNearClipPlane(float plane) { if (plane > 0) { m_NearClipPlane = plane; UpdateProjectionMatrix(); } }

		/// <summary>
		/// Set the far clipping plane. Silently fails if <= near clipping plane
		/// </summary>
		/// <param name="plane"></param>
		inline void SetFarClipPlane(float plane) { if (plane > m_NearClipPlane) { m_FarClipPlane = plane; UpdateProjectionMatrix(); } }

		/// <summary>
		/// Get the Near clipping plane
		/// </summary>
		/// <returns></returns>
		inline float GetNearClipPlane() { return m_NearClipPlane; }

		/// <summary>
		/// Get the Far clipping plane
		/// </summary>
		/// <returns></returns>
		inline float GetFarClipPlane() { return m_FarClipPlane; }

		/// <summary>
		/// Get the render target of a camera
		/// </summary>
		/// <returns></returns>
		const RenderTargetRef& GetRenderTarget() const { return m_RenderTarget; }

		/// <summary>
		/// Set the render target of a camera
		/// </summary>
		/// <param name="target"></param>
		void SetRenderTarget(const RenderTargetRef& target) { m_RenderTarget = target; }

	public:
		/// <summary>
		/// Update the camera for a new size of area being rendererd to
		/// </summary>
		/// <param name="width">width of area</param>
		/// <param name="height">height of area</param>
		virtual void UpdateRenderArea(uint32_t width, uint32_t height);
		
	public:
		/// <summary>
		/// Given a location on the screen, make a unit ray into the world, relative to the same space as the camera sees it.
		/// </summary>
		/// <param name="x">the X pixel location on the screen</param>
		/// <param name="y">the Y pixel location on the screen</param>
		/// <returns>a pair of vectors, as (start, offset) of the ray</returns>
		virtual std::pair<Vector, Vector> GetRayFromScreenCoordinate(float x, float y) const;

		/// <summary>
		/// Get the filtering bits for what entities can be rendered by the camera
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetRenderFilterBits() const { return m_RenderFilterBits; }

		/// <summary>
		/// Set the filtering bits for what entities can be rendered by the camera
		/// </summary>
		/// <param name="bits"></param>
		inline void SetRenderFilterBits(uint32_t bits) { m_RenderFilterBits = bits; }

		/// <summary>
		/// Get the camera's lighting material
		/// </summary>
		/// <returns></returns>
		inline const MaterialBaseRef& GetLightingMaterial() { return m_LightingMaterial; }

		/// <summary>
		/// Set the camera's lighting material. Silently fails if the material is not of the Lighting type
		/// </summary>
		/// <param name="mat"></param>
		inline void SetLightingMaterial(MaterialBaseRef mat) { if (mat->GetType() == MaterialType::LIGHTING) { m_LightingMaterial = mat; } }

	protected:
		inline virtual void UpdateProjectionMatrix() { return; }
		
		std::pair<float, float> GetRenderTargetSize() const;

	protected:
		Transform m_Transform;
		const ProjectionType m_Type;
		glm::mat4 m_ProjectionMatrix;
		RenderTargetRef m_RenderTarget;
		uint32_t m_RenderFilterBits;
		MaterialBaseRef m_LightingMaterial;
		float m_NearClipPlane;
		float m_FarClipPlane;
	};

	/// <summary>
	/// An Orthographic camera
	/// </summary>
	class OrthographicCamera : public Camera {
	public:
		/// <summary>
		/// A structure to hold the extent of the camera.
		/// Contains 6 paramaters: Left, Right, Bottom, Top, Near, and Far.
		/// </summary>
		struct OrthoExtent {
			float Left, Right, Bottom, Top, Near, Far;
			OrthoExtent(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f, float near = -1.0f, float far = 1.0f)
				: Left(left), Right(right), Bottom(bottom), Top(top), Near(near), Far(far) {}
		};
	protected:
		//this exists soley for ScreenCamera to use
		OrthographicCamera(ProjectionType type);
	public:
		/// <summary>
		/// Create an Orthographic camera that maintains the aspect ratio at its creation
		/// </summary>
		/// <param name="width">the width of the window</param>
		OrthographicCamera(float width);
		/// <summary>
		/// Create an Orthographic camera that uses a specified extent.
		/// </summary>
		/// <param name="extent"></param>
		OrthographicCamera(OrthoExtent extent);

		/// <summary>
		/// Set the extent of the window, via its width, maintaining aspect ratio
		/// </summary>
		/// <param name="width">the new width</param>
		virtual void SetExtent(float width);
		/// <summary>
		/// Set the extent of the window directly, ignoring aspect ratio.
		/// </summary>
		/// <param name="extent">the new extent</param>
		virtual void SetExtent(OrthoExtent extent);

		/// <summary>
		/// Get the current extent
		/// </summary>
		/// <returns>the extent</returns>
		virtual OrthoExtent GetExtent() const { return m_Extent; }

		/// <summary>
		/// Update the camera for a new size of area being rendererd to
		/// </summary>
		/// <param name="width">width of area</param>
		/// <param name="height">height of area</param>
		virtual void UpdateRenderArea(uint32_t width, uint32_t height) override;

	protected:
		/// <summary>
		/// update the projection matrix according to extent
		/// </summary>
		virtual void UpdateProjectionMatrix() override;

	private:
		OrthoExtent m_Extent;
		bool m_MaintainAspectRatio;
	};

	/// <summary>
	/// A persective camera for all your 3D needs
	/// </summary>
	class PerspectiveCamera : public Camera {
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="fov">the field of view</param>
		/// <param name="aspectRatio">the aspect ratio of the renderTarget</param>
		PerspectiveCamera(float fov, float aspectRatio = -1.0f);

		/// <summary>
		/// Get the field of view
		/// </summary>
		/// <returns></returns>
		inline float GetFOV() const { return m_FOV; }

		/// <summary>
		/// Set the field of view. Triggers a projection matrix update, so, might be slower than expected.
		/// </summary>
		/// <param name="fov"></param>
		inline void SetFOV(float fov) { m_FOV = fov; UpdateProjectionMatrix(); }

		/// <summary>
		/// Get the aspect ratio
		/// </summary>
		/// <returns></returns>
		inline float GetAspectRatio() const { return m_AspectRatio; }

		/// <summary>
		/// Set the aspect ratio. use a negative to calculate from current RenderTarget. Triggers a projection matrix update, so, might be slower than expected.
		/// </summary>
		/// <param name="aspect"></param>
		inline void SetAspectRatio(float aspect) { m_AspectRatio = aspect; UpdateProjectionMatrix(); }

		/// <summary>
		/// Update the size of the area being rendered to (chages the apsect ratio). Triggers a projection matrix update, so, might be slower than expected.
		/// </summary>
		/// <param name="width">the new width</param>
		/// <param name="height">the new hight</param>
		virtual void UpdateRenderArea(uint32_t width, uint32_t height) override;

		/// <summary>
		/// Given a location on the screen, make a unit ray into the world, relative to the same space as the camera sees it.
		/// </summary>
		/// <param name="x">the X pixel location on the screen</param>
		/// <param name="y">the Y pixel location on the screen</param>
		/// <returns>a pair of vectors, as (start, offset) of the ray</returns>
		virtual std::pair<Vector, Vector> GetRayFromScreenCoordinate(float x, float y) const override;
	protected:
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_FOV;
		float m_AspectRatio;
	};

	/// <summary>
	/// A screenspace camera. Coordinates are in pixels. Origin is the top left, and screen coordinates match world coordinates (unless the camera is moved)
	/// </summary>
	class ScreenCamera : public OrthographicCamera {
	public:
		ScreenCamera();
		
		virtual void SetExtent(float width) override {};
		virtual void SetExtent(OrthoExtent extent) override {};

		/// <summary>
		/// Update the camera for a new size of area being rendererd to
		/// </summary>
		/// <param name="width">width of area</param>
		/// <param name="height">height of area</param>
		virtual void UpdateRenderArea(uint32_t width, uint32_t height) override;

		/// <summary>
		/// Get the view matrix of the camera
		/// </summary>
		/// <returns>view matrix, column-major</returns>
		virtual glm::mat4 GetViewMatrix() const override;

		/// <summary>
		/// Given a location on the screen, make a unit ray into the world, relative to the same space as the camera sees it.
		/// </summary>
		/// <param name="x">the X pixel location on the screen</param>
		/// <param name="y">the Y pixel location on the screen</param>
		/// <returns>a pair of vectors, as (start, offset) of the ray</returns>
		virtual std::pair<Vector, Vector> GetRayFromScreenCoordinate(float x, float y) const override;

		/// <summary>
		/// Get the width of the area being rendered to
		/// </summary>
		/// <returns></returns>
		inline float GetTargetWidth() const { return m_ScreenWidth; }

		/// <summary>
		/// Get the height of the area being rendererd to
		/// </summary>
		/// <returns></returns>
		inline float GetTargetHeight() const { return m_ScreenHeight; }

	protected:
		/// <summary>
		/// update the projection matrix according to extent
		/// </summary>
		virtual void UpdateProjectionMatrix() override;
	private:
		float m_ScreenWidth, m_ScreenHeight;
	};
}
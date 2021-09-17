#pragma once
#include "Tara/Math/Types.h"

namespace Tara {

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
			m_ProjectionMatrix(1) 
		{};

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
		Camera::ProjectionType GetProjectionType() const { return m_Type; }

		/// <summary>
		/// Get the projection matrix of the camera
		/// </summary>
		/// <returns>4x4 projection matrix, column-major</returns>
		virtual glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; };
		/// <summary>
		/// Get the view matrix of the camera
		/// </summary>
		/// <returns>view matrix, column-major</returns>
		virtual glm::mat4 GetViewMatrix() const { return glm::inverse(m_Transform.GetTransformMatrix()); };
		/// <summary>
		/// Get the projection and view matrix pre-combined
		/// </summary>
		/// <returns>projection*view matrix, column-major</returns>
		virtual glm::mat4 GetViewProjectionMatrix() const {return m_ProjectionMatrix * glm::inverse(m_Transform.GetTransformMatrix());};

		/// <summary>
		/// Update the camera for a new size of area being rendererd to
		/// </summary>
		/// <param name="width">width of area</param>
		/// <param name="height">height of area</param>
		inline virtual void UpdateRenderArea(uint32_t width, uint32_t height) { return; }

	protected:
		inline virtual void UpdateProjectionMatrix() { return; }

	protected:
		/// <summary>
		/// The camera's transform. Scale should be ignored.
		/// </summary>
		Transform m_Transform;
		/// <summary>
		/// The camera's projection type
		/// </summary>
		const ProjectionType m_Type;
		/// <summary>
		/// the camera's projection matrix
		/// </summary>
		glm::mat4 m_ProjectionMatrix;
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
		inline virtual void UpdateRenderArea(uint32_t width, uint32_t height) override;

	protected:
		/// <summary>
		/// update the projection matrix according to extent
		/// </summary>
		virtual void UpdateProjectionMatrix() override;

	private:
		/// <summary>
		/// The camera extent
		/// </summary>
		OrthoExtent m_Extent;
		/// <summary>
		/// if the aspect ratio should be maintained.
		/// </summary>
		bool m_MaintainAspectRatio;
	};


	using CameraRef = std::shared_ptr<Camera>;
}
#include "tarapch.h"
#include "Camera.h"
#include "Tara/Math/Functions.h"
#include "Tara/Core/Application.h"

namespace Tara {

	void Camera::UpdateRenderArea(uint32_t width, uint32_t height)
	{
		if (m_RenderTarget) {
			m_RenderTarget->SetSize(width, height);
		}
	}


	std::pair<Vector, Vector> Camera::GetRayFromScreenCoordinate(float x, float y) const
	{
		//this is for orthographic camera only
		auto sizes = GetRenderTargetSize();
		float wWidth = sizes.first;
		float wHeight = sizes.second;
		//Get the positions in the viewing volume of -1 to 1
		float vx = MapRange((float)x, 0.0f, wWidth, -1.0f, 1.0f);
		float vy = MapRange((float)y, 0.0f, wHeight, 1.0f, -1.0f); //y is inverted between pixel space and view space
		
		//make a vec4 for matrix operations
		glm::vec4 viewPlanePos = { vx, vy, 0.0f, 1.0f }; 
		
		//Get the transform matrix, for use later
		auto tMatrix = m_Transform.GetTransformMatrix(); 
		
		//transform the vector to be relative to the camera location in the world. 
		Vector origin(tMatrix * glm::inverse(m_ProjectionMatrix) * viewPlanePos); 

		//now, get the offset.
		//just get the forward vector of the camera (since its basic orthographic)
		Vector offset = m_Transform.Rotation.GetForwardVector().Normalize(); 

		//Return the pair of start and end of the ray
		return std::make_pair(origin, offset);
	}


	std::pair<float, float> Camera::GetRenderTargetSize() const
	{
		if (m_RenderTarget) {
			return std::make_pair((float)m_RenderTarget->GetWidth(), (float)m_RenderTarget->GetHeight());
		}
		else {
			auto window = Application::Get()->GetWindow();
			return std::make_pair((float)window->GetWidth(), (float)window->GetHeight());
		}
	}

	OrthographicCamera::OrthographicCamera(ProjectionType type)
		: Camera(ProjectionType::Ortographic), m_Extent(-1, 1), m_MaintainAspectRatio(true)
	{}

	OrthographicCamera::OrthographicCamera(float width)
			: Camera(ProjectionType::Ortographic), m_Extent(-(width/2), width/2), m_MaintainAspectRatio(true)
	{
		UpdateProjectionMatrix();
	}

	OrthographicCamera::OrthographicCamera(OrthoExtent extent)
		: Camera(ProjectionType::Ortographic), m_Extent(extent), m_MaintainAspectRatio(false)
	{
		UpdateProjectionMatrix();
	}
	
	void OrthographicCamera::SetExtent(float width)
	{
		m_MaintainAspectRatio = true;
		m_Extent = { -(width/2), width/2 };
		UpdateProjectionMatrix();
	}
	
	void OrthographicCamera::SetExtent(OrthoExtent extent)
	{
		m_MaintainAspectRatio = false;
		m_Extent = extent;
		UpdateProjectionMatrix();
	}
	
	inline void OrthographicCamera::UpdateRenderArea(uint32_t width, uint32_t height)
	{
		if (m_MaintainAspectRatio) { 
			float aspectRatio = (float)width / (float)height;
			float width = m_Extent.Right - m_Extent.Left;
			float height = width / aspectRatio;
			m_Extent.Top = height / 2.0f;
			m_Extent.Bottom = -m_Extent.Top;
			m_Extent.Near = -1.0f;
			m_Extent.Far = 1.0f;

			m_ProjectionMatrix = glm::ortho(
				m_Extent.Left, m_Extent.Right,
				m_Extent.Bottom, m_Extent.Top,
				m_Extent.Near, m_Extent.Far
			);
		}
		if (m_RenderTarget) {
			m_RenderTarget->SetSize(width, height);
		}
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		if (m_MaintainAspectRatio) {
			//TODO: make this depend on some other information than getting the window
			auto sizes = GetRenderTargetSize();
			float aspectRatio = sizes.first / sizes.second;
			float width = m_Extent.Right - m_Extent.Left;
			float height = width / aspectRatio;
			LOG_S(INFO) << "Ortho Camera Resize: Aspect: " << aspectRatio << " Width: " << width << " Height:" << height;
			m_Extent.Top = height / 2.0f;
			m_Extent.Bottom = -m_Extent.Top;
			m_Extent.Near = -1.0f;
			m_Extent.Far = 1.0f;
		}
		m_ProjectionMatrix = glm::ortho(
			m_Extent.Left, m_Extent.Right,
			m_Extent.Bottom, m_Extent.Top,
			m_Extent.Near, m_Extent.Far
		);
	}
	

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio)
		: Camera(ProjectionType::Perspective), m_FOV(fov), m_AspectRatio(aspectRatio)
	{
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::UpdateRenderArea(uint32_t width, uint32_t height)
	{
		if (m_RenderTarget) {
			m_RenderTarget->SetSize(width, height);
		}
		auto sizes = GetRenderTargetSize();
		m_AspectRatio = sizes.first / sizes.second;
		UpdateProjectionMatrix();
	}

	std::pair<Vector, Vector> PerspectiveCamera::GetRayFromScreenCoordinate(float x, float y) const
	{
		//Here is the version for perspective cameras
		auto sizes = GetRenderTargetSize();
		float wWidth = sizes.first;
		float wHeight = sizes.second;
		//Get the positions in the viewing volume of -1 to 1
		float vx = MapRange((float)x, 0.0f, wWidth, -1.0f, 1.0f);
		float vy = MapRange((float)y, 0.0f, wHeight, 1.0f, -1.0f); //y is inverted between pixel space and view space

		//make a vec4 for matrix operations
		glm::vec4 viewPlanePos = { vx, vy, 0.0f, 1.0f };

		//Get the transform matrix, for use later
		auto tMatrix = m_Transform.GetTransformMatrix();

		//transform the vector to be relative to the camera location in the world. 
		//TODO: make this robust enough to work for all camera types without overriding! (currently only works for orthographic types.)
		Vector offset(tMatrix * glm::inverse(m_ProjectionMatrix) * viewPlanePos);
		offset.Normalize();
		//now, get the offset.
		//just get the forward vector of the camera (since its basic orthographic)
		//TODO: make this robust enough for all camera types without overriding!
		//Vector offset = m_Transform.Rotation.GetForwardVector().Normalize();

		//Return the pair of start and end of the ray
		return std::make_pair(m_Transform.Position, offset);
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		//check if the aspect ratio is set to dynamically generate
		if (m_AspectRatio < 0) {
			//if so, get from window
			auto& window = Application::Get()->GetWindow();
			m_AspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
		}
		//TODO: add methods to set near and far clipping plane
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, -1.0f, 1.0f);
	}

	ScreenCamera::ScreenCamera()
		: OrthographicCamera(ProjectionType::Screen)
	{
		auto& window = Application::Get()->GetWindow();
		m_ScreenWidth = (float)window->GetWidth();
		m_ScreenHeight = (float)window->GetHeight();
		OrthographicCamera::SetExtent((float)window->GetWidth());
	}

	void ScreenCamera::UpdateRenderArea(uint32_t width, uint32_t height)
	{
		if (m_RenderTarget) {
			m_RenderTarget->SetSize(width, height);
		}
		m_ScreenWidth = (float)width;
		m_ScreenHeight = (float)height;
		OrthographicCamera::SetExtent((float)width);
	}

	glm::mat4 ScreenCamera::GetViewMatrix() const
	{
		auto t = Transform(m_Transform);
		t.Position.x += (m_ScreenWidth / 2);
		t.Position.y += (m_ScreenHeight / 2);
		return glm::inverse(t.GetTransformMatrix());
	}

	void ScreenCamera::UpdateProjectionMatrix()
	{
		auto extent = GetExtent();
		float aspectRatio = m_ScreenWidth / m_ScreenHeight;
		float width = extent.Right - extent.Left;
		float height = width / aspectRatio;
		extent.Top = height / 2.0f;
		extent.Bottom = -extent.Top;
		extent.Near = -1.0f;
		extent.Far = 1.0f;
		
		m_ProjectionMatrix = glm::ortho(
			extent.Left, extent.Right,
			extent.Bottom, extent.Top,
			extent.Near, extent.Far
		);
	}


	



	

}



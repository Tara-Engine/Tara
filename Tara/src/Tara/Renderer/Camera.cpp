#include "tarapch.h"
#include "Camera.h"

#include "Tara/Core/Application.h"

namespace Tara {
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
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		if (m_MaintainAspectRatio) {
			//TODO: make this depend on some other information than getting the window
			auto window = Application::Get()->GetWindow();
			float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
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
}

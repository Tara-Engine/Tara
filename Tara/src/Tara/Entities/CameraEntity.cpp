#include "tarapch.h"
#include "CameraEntity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Core/Script.h"

namespace Tara{

	CameraEntity::CameraEntity(EntityNoRef parent, LayerNoRef owningLayer, Camera::ProjectionType projectionType, Transform transform, std::string name)
		:Entity(parent, owningLayer, transform, name), m_Camera(nullptr), m_OrthoExtent(), m_PerspectiveFOV(45.0f)
	{
		SetProjectionType(projectionType);
	}

	void CameraEntity::SetProjectionType(Camera::ProjectionType type)
	{
		//if the same, don't do anything
		if (m_Camera && GetProjectionType() == type) { return; }

		switch (type) {
		case Camera::ProjectionType::None: {
			m_Camera = std::make_shared<Camera>();
			return;
		}
		case Camera::ProjectionType::Ortographic: {
			m_Camera = std::make_shared<OrthographicCamera>(m_OrthoExtent);
			return;
		}
		case Camera::ProjectionType::Perspective: {
			m_Camera = std::make_shared<PerspectiveCamera>(m_PerspectiveFOV);
			return;
		}
		default: {
			LOG_S(ERROR) << "Cameras other than None and Orthographic are not yet supported!";
			return;
		}
		}
	}

	void CameraEntity::SetOrthographicExtent(OrthographicCamera::OrthoExtent extent)
	{
		if (GetProjectionType() == Camera::ProjectionType::Ortographic) {
			std::shared_ptr<OrthographicCamera> orthoCam =  std::dynamic_pointer_cast<OrthographicCamera>(m_Camera);
			if (orthoCam) {
				orthoCam->SetExtent(extent);
				m_OrthoExtent = orthoCam->GetExtent();
				return;
			}
		}
		m_OrthoExtent = extent;
	}
	void CameraEntity::SetOrthographicExtent(float width)
	{
		if (GetProjectionType() == Camera::ProjectionType::Ortographic) {
			std::shared_ptr<OrthographicCamera> orthoCam = std::dynamic_pointer_cast<OrthographicCamera>(m_Camera);
			if (orthoCam) {
				orthoCam->SetExtent(width);
				m_OrthoExtent = orthoCam->GetExtent();
			}
		}
	}

	void CameraEntity::SetPerspectiveFOV(float fov)
	{
		if (GetProjectionType() == Camera::ProjectionType::Ortographic) {
			PerspectiveCameraRef perspCam = std::dynamic_pointer_cast<PerspectiveCamera>(m_Camera);
			if (perspCam) {
				perspCam->SetFOV(fov);
				m_PerspectiveFOV = fov;
			}
		}
	}

	void CameraEntity::OnUpdate(float deltaTime)
	{
		if (m_UseWorldScale){
			m_Camera->SetTransform(GetWorldTransform());
		}
		else {
			Transform t = GetWorldTransform();
			t.Scale = GetRelativeTransform().Scale;
			m_Camera->SetTransform(t);
		}
	}

	void CameraEntity::OnBeginPlay()
	{
		ListenForEvents(true);
	}

	void CameraEntity::OnEvent(Event& e)
	{
		//if we have a window resize event, then update the internal camera render area
		EventFilter filter(e);
		filter.Call<WindowResizeEvent>([this](WindowResizeEvent& ee) {
			this->GetCamera()->UpdateRenderArea(ee.GetWidth(), ee.GetHeight());
			return false;
		});
	}

	void CameraEntity::__SCRIPT__SetProjectionType(const char* type)
	{
		Camera::ProjectionType proj = Camera::ProjectionType::None;
		if (type == "Ortographic") { proj = Camera::ProjectionType::Ortographic; }
		if (type == "Perspective") { proj = Camera::ProjectionType::Perspective; }
		if (type == "Screen") { proj = Camera::ProjectionType::Screen; }
		SetProjectionType(proj);
	}

	std::string CameraEntity::__SCRIPT__GetProjectionType() const
	{
		auto proj = GetProjectionType();
		switch (proj) {
		case Camera::ProjectionType::Ortographic: {return "Ortographic"; }
		case Camera::ProjectionType::Perspective: {return "Perspective"; }
		case Camera::ProjectionType::Screen: {return "Screen"; }
		default: {return "None"; }
		}
	}

	void CameraEntity::__SCRIPT__SetOrthographicExtent(sol::object extent)
	{
		LOG_S(WARNING) << "Lua version of CamaeraEntity::SetOrthographicExtent not implemented yet.";
	}


	void CameraEntity::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<CameraEntity> type = lua.new_usertype<CameraEntity>("CameraEntity", sol::base_classes, sol::bases<Entity>());
		CONNECT_FUNCTION(CameraEntity, SetUseWorldScale);
		CONNECT_FUNCTION(CameraEntity, GetUseWorldScale);
		CONNECT_FUNCTION(CameraEntity, SetPerspectiveFOV);
		CONNECT_FUNCTION_OVERRIDE(CameraEntity, GetProjectionType);
		CONNECT_FUNCTION_OVERRIDE(CameraEntity, SetProjectionType);
		CONNECT_FUNCTION_OVERRIDE(CameraEntity, SetOrthographicExtent);


	}
}

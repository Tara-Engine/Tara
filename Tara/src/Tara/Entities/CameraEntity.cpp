#include "tarapch.h"
#include "CameraEntity.h"

namespace Tara{

	CameraEntity::CameraEntity(EntityNoRef parent, LayerNoRef owningLayer, Camera::ProjectionType projectionType, Transform transform, std::string name)
		:Entity(parent, owningLayer, transform, name), m_Camera(nullptr), m_OrthoExtent()
	{
		SetProjectionType(projectionType);
	}


	CameraEntityRef CameraEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Camera::ProjectionType projectionType, Tara::Transform transform, std::string name)
	{
		CameraEntityRef ref = std::make_shared<CameraEntity>(parent, owningLayer, projectionType, transform, name);
		Entity::Register(ref);
		return ref;
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
}

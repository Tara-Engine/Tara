#include "tarapch.h"
#include "UISpacerEntity.h"


namespace Tara{

	UISpacerEntity::UISpacerEntity(EntityNoRef parent, LayerNoRef owningLayer, const std::string& name)
		: UIBaseEntity(parent, owningLayer, name), m_Size(0,0)
	{}

	void UISpacerEntity::CalculateDesiredSize()
	{
		UIBaseEntity::CalculateDesiredSize();
		auto desired = GetDesiredSize();
		desired.x = std::max(desired.x, m_Size.x);
		desired.y = std::max(desired.y, m_Size.y);
		SetDesiredSize(desired);
	}

	


}

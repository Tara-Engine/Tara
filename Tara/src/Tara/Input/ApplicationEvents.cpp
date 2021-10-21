#include "tarapch.h"
#include "ApplicationEvents.h"
#include "Tara/Core/Entity.h"

namespace Tara {
	std::string OverlapEvent::ToString() const
	{
		std::stringstream ss;
		ss << "Overalp! " << " [Self:" << m_Manifold.A->GetName() << " Other:" << m_Manifold.B->GetName() << "]";
		return ss.str();
	}

}

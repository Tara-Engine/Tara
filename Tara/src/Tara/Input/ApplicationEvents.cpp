#include "tarapch.h"
#include "ApplicationEvents.h"
#include "Tara/Core/Entity.h"

std::string Tara::OverlapEvent::ToString() const
{
	std::stringstream ss;
	ss << "Overalp! " << " [Self:" << m_Self->GetName() << " Other:" << m_Other->GetName() << "]";
	return ss.str();
}

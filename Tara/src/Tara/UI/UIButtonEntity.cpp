#include "tarapch.h"
#include "UIButtonEntity.h"
#include "Tara/Components/ClickableComponent.h"
#include "Tara/Input/ApplicationEvents.h"


namespace Tara{

	UIButtonEntity::UIButtonEntity(EntityNoRef parent, LayerNoRef owningLayer, 
		PatchRef patchNormal, PatchRef patchHovered, PatchRef patchClicked, PatchRef patchDisabled, 
		const std::string& name
	)
		:UIVisualEntity(parent, owningLayer, patchNormal, name), 
		m_PatchNormal(patchNormal), m_PatchHover(patchHovered), m_PatchClick(patchClicked), m_PatchDisabled(patchDisabled)
	{

	}

	const PatchRef& UIButtonEntity::GetSpecificPatch(ButtonState patchID) const
	{
		switch (patchID) {
		case ButtonState::NORMAL: return m_PatchNormal;
		case ButtonState::HOVERED: return m_PatchHover;
		case ButtonState::CLICKED: return m_PatchClick;
		case ButtonState::DISABLED: return m_PatchDisabled;
		}
	}

	void UIButtonEntity::SetSpecificPatch(ButtonState patchID, const PatchRef& patch)
	{
		switch (patchID) {
		case ButtonState::NORMAL: m_PatchNormal = patch;  return;
		case ButtonState::HOVERED: m_PatchHover = patch;  return;
		case ButtonState::CLICKED: m_PatchClick = patch;  return;
		case ButtonState::DISABLED: m_PatchDisabled = patch;  return;
		}
	}

	void UIButtonEntity::OnBeginPlay()
	{
		//make a clickable component to handle click detection for the button
		CreateComponent<ClickableComponent>(weak_from_this(), "ButtonClickableComponent");
	}

	void UIButtonEntity::OnUpdate(float deltaTime)
	{
		UIVisualEntity::OnUpdate(deltaTime);

		switch (m_ButtonState){
		case ButtonState::NORMAL : {
			SetPatch(m_PatchNormal);
			break;
		}
		case ButtonState::HOVERED: {
			SetPatch(m_PatchHover);
			break;
		}
		case ButtonState::CLICKED: {
			SetPatch(m_PatchClick);
			break;
		}
		case ButtonState::DISABLED: {
			SetPatch(m_PatchDisabled);
			break;
		}
		}
	}

	void UIButtonEntity::OnEvent(Tara::Event& e)
	{
		if (m_ButtonState != ButtonState::DISABLED) {
			EventFilter filter(e);
			filter.Call<ClickEvent>(TARA_BIND_FN(UIButtonEntity::OnClickEvent));
			filter.Call<HoverEvent>(TARA_BIND_FN(UIButtonEntity::OnHoverEvent));
		}
	}

	bool UIButtonEntity::OnClickEvent(ClickEvent& e)
	{
		if (e.GetRelease()) {
			m_ButtonState = ButtonState::NORMAL;
			//activate the button!
			UIToggleEvent e(std::dynamic_pointer_cast<UIBaseEntity>(shared_from_this()), true);
			ReceiveEvent(e);
		}
		else {
			if (e.GetMouseMoveOff()) {
				m_ButtonState = ButtonState::NORMAL;
			}
			else {
				m_ButtonState = ButtonState::CLICKED;
			}
		}
		return false;
	}

	bool UIButtonEntity::OnHoverEvent(HoverEvent& e)
	{
		if (e.GetIsHovering()) {
			m_ButtonState = ButtonState::HOVERED;
		}
		else {
			m_ButtonState = ButtonState::NORMAL;
		}
		return false;
	}

}

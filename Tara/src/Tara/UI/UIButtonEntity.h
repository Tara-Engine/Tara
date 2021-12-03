#pragma once
#include "Tara/UI/UIVisualEntity.h"

namespace Tara {

	class ClickEvent;
	class HoverEvent;

	REFTYPE(UIButtonEntity);
	NOREFTYPE(UIButtonEntity);

	class UIButtonEntity : public UIVisualEntity {
	public:
		enum class ButtonState : uint8_t {
			NORMAL, HOVERED, CLICKED, DISABLED
		};
	
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="parent">the parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="patch">the visual patch</param>
		/// <param name="name">the name</param>
		UIButtonEntity(EntityNoRef parent, LayerNoRef owningLayer, 
			PatchRef patchNormal, PatchRef patchHovered, PatchRef patchClicked, PatchRef patchDisabled, 
			const std::string& name = "UIButtonEntity"
		);

		virtual ~UIButtonEntity() = default;

		/// <summary>
		/// Set if the button is enabled or not. Disabled buttons cannot be clicked.
		/// </summary>
		/// <param name="enabled"></param>
		inline void SetEnabled(bool enabled) { m_ButtonState = (enabled) ? ButtonState::NORMAL : ButtonState::DISABLED; }

		/// <summary>
		/// Get if the button is enabled.
		/// </summary>
		/// <returns></returns>
		inline bool GetEnabled() const { return !(m_ButtonState == ButtonState::DISABLED); }

		/// <summary>
		/// Get the state of the button (Normal, Hovered, Clicked, Disabled.
		/// </summary>
		/// <returns></returns>
		inline const ButtonState& GetState() const { return m_ButtonState; }

		/// <summary>
		/// Get a specific state patch
		/// </summary>
		/// <param name="patchID"></param>
		/// <returns></returns>
		const PatchRef& GetSpecificPatch(ButtonState patchID) const;

		/// <summary>
		/// Set a specific state patch
		/// </summary>
		/// <param name="patchID"></param>
		/// <param name="patch"></param>
		void SetSpecificPatch(ButtonState patchID, const PatchRef& patch);

	public:
		//overrides

		virtual void OnBeginPlay() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnEvent(Tara::Event& e) override;


	private:
		//Event functions
		bool OnClickEvent(ClickEvent& e);
	
		bool OnHoverEvent(HoverEvent& e);

	private:
		PatchRef m_PatchNormal;
		PatchRef m_PatchHover;
		PatchRef m_PatchClick;
		PatchRef m_PatchDisabled;
		ButtonState m_ButtonState;
	};
}
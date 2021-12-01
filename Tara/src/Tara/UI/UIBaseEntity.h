#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Math/UIBox.h"

namespace Tara {
	REFTYPE(UIBaseEntity);
	NOREFTYPE(UIBaseEntity);

	enum UISnapRule {
		NONE = 0x0,
		LEFT = 0x1,
		CENTER_HORIZONTAL = 0x2,
		RIGHT = 0x4,
		TOP = 0x8,
		CENTER_VERTICAL = 0x10,
		BOTTOM = 0x20
	};

	/// <summary>
	/// A base entity for all UIEntities. 
	/// </summary>
	class UIBaseEntity : public Entity {
	public:
		UIBaseEntity(EntityNoRef parent, LayerNoRef owningLayer, const std::string& name = "UIBaseEntity");

		virtual ~UIBaseEntity() = default;
		
		inline static void SetEnableDebugDraw(bool enable) { s_EnableDebugRendering = enable; }

		inline static bool GetEnableDebugDraw() { return s_EnableDebugRendering; }

	public:

		/// <summary>
		/// Get the minimum size that this UIEntity wants to be. 
		/// This is determined by its children and any sized internal content (like text)
		/// </summary>
		/// <returns></returns>
		glm::vec2 GetDesiredSize() const;

		/// <summary>
		/// Overridable calculate desired size. Must set it into m_DesieredSize;
		/// </summary>
		virtual void CalculateDesiredSize();

		

		/// <summary>
		/// Set the area that the UI is allowed to render in.
		/// Should be called by its parent every frame
		/// </summary>
		/// <param name="area"></param>
		void SetAllowedArea(const UIBox& area);

		/// <summary>
		/// Get the area that the UI is allowed to render in.
		/// </summary>
		/// <returns></returns>
		UIBox GetAllowedArea() const;
		
		/// <summary>
		/// Set the border around the element's children
		/// </summary>
		/// <param name="left">left side in pixels</param>
		/// <param name="right">right side in pixels</param>
		/// <param name="top">top in pixels</param>
		/// <param name="bottom">bottom in pixels</param>
		inline void SetBorder(float left, float right, float top, float bottom) {SetBorder({left, right, top, bottom});}
		
		/// <summary>
		/// Set the border around the element's children
		/// </summary>
		/// <param name="border">vec4 of {left, right, top, bottom} sides in pixels</param>
		inline void SetBorder(glm::vec4 border) { m_Border = border; m_DesiredSizeDirty = true; }

		/// <summary>
		/// Get the border around the element's children
		/// </summary>
		/// <returns>vec4 of {left, right, top, bottom} sides in pixels</returns>
		inline  glm::vec4 GetBorder() const {return m_Border;}

		/// <summary>
		/// Set the spacing between children, if appropreate
		/// </summary>
		/// <param name="x">the x spacing in pixels</param>
		/// <param name="y">the y spacing in pixels</param>
		inline void SetSpacing(float x, float y) { SetSpacing({ x,y }); }

		/// <summary>
		/// Set the spacing between children, if appropreate
		/// </summary>
		/// <param name="spacing"> {x,y} spacing in pixels</param>
		inline void SetSpacing(glm::vec2 spacing) { m_Spacing = spacing; m_DesiredSizeDirty = true; }

		/// <summary>
		/// Get the spacing between children, if appropreate
		/// </summary>
		/// <returns>{x,y} spacing in pixels</returns>
		inline glm::vec2 GetSpacing() const { return m_Spacing; }

		/// <summary>
		/// Set the snap rules for the UIEntity
		/// </summary>
		/// <param name="snapType">the snap rules to set</param>
		inline void SetSnapRules(uint32_t snapRules) { m_SnapRules = snapRules; m_DesiredSizeDirty = true;}

		/// <summary>
		/// Add a new snap rule for the UIEntity
		/// </summary>
		/// <param name="snapType">the snap rule to add</param>
		inline void AddSnapRule(uint32_t snapRules) { m_SnapRules |= snapRules; m_DesiredSizeDirty = true;}

		/// <summary>
		/// Remove a snap rule from the UIEntity
		/// </summary>
		/// <param name="snapType">the snap rule to remove</param>
		inline void RemoveSnapRule(uint32_t snapRules) { m_SnapRules = (m_SnapRules | snapRules) ^ snapRules; m_DesiredSizeDirty = true;}

		/// <summary>
		/// Get the current snap rules for the UIEntity
		/// </summary>
		/// <returns>the snap rules as bitmask</returns>
		inline uint32_t GetSnapRules() const { return m_SnapRules; }
		
		/// <summary>
		/// Set the positional offsets for drawing
		/// </summary>
		/// <param name="left">the left offset</param>
		/// <param name="right">the right offset</param>
		/// <param name="top">the top offset</param>
		/// <param name="bottom">the bottom offset</param>
		inline void SetOffsets(float left, float right, float top, float bottom) { SetOffsets(glm::vec4(left, right, top, bottom)); }

		/// <summary>
		/// Set the positional offsets for drawings
		/// </summary>
		/// <param name="offsets">UIBox offsets</param>
		inline void SetOffsets(glm::vec4 offsets) { m_Offsets = offsets; m_DesiredSizeDirty = true; }

		/// <summary>
		/// Get the positional offsets for drawing
		/// </summary>
		/// <returns>Offsets as a UIBox</returns>
		inline const glm::vec4& GetOffsets() const { return m_Offsets; }

		/// <summary>
		/// Get the renderable area, based off of minimum size, offsets, and Snap Rules
		/// </summary>
		/// <returns></returns>
		UIBox GetRenderArea() const;

	protected:
		/// <summary>
		/// Set the desired size. Will not mark m_DesiredSizeDirty to true. Used in CalculateDesiredSize
		/// </summary>
		void SetDesiredSize(glm::vec2 size);

	public:
		//Overriden stuff

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnEvent(Event& e) override;

		inline virtual void OnBeginPlay() override { ListenForEvents(true); };

		inline virtual BoundingBox GetSpecificBoundingBox() const;

		//DEBUG
		virtual void OnDraw(float deltaTime) override;


		inline virtual void SelfOverlapChecks() override {};
		
		inline virtual void OtherOverlapChecks(EntityRef other) override {};
	protected:
		//Transform m_Transform
		//floats x1, y1, x2, y1, vec4 border
		bool m_DesiredSizeDirty = true; //should be marked true on all children/parent change events
	private:
		glm::vec2 m_Spacing;
		glm::vec4 m_Border;
		uint32_t m_SnapRules;
		glm::vec4 m_Offsets;

	public:
		//delete all functions related to Transform setting - They will break the data compression
		//and are useless for UI
		//well, since only some can actually be deleted, make them empty functions. 
#pragma warning( push )
#pragma warning( disable : 4172 )

		virtual inline const Transform& GetRelativeTransform()			const	override { return TRANSFORM_DEFAULT; }
		virtual inline Transform GetWorldTransform()					const	override { return TRANSFORM_DEFAULT; }
		virtual inline void SetRelativeTransform(const Transform& transform)	override {}
		virtual inline void SetWorldTransform(const Transform& transform)		override {}
		virtual inline const Vector& GetRelativePosition()				const	override { return { 0,0,0 }; }
		virtual inline void SetRelativePosition(const Vector& pos)				override {}
		virtual inline Vector GetWorldPosition()						const	override { return { 0,0,0 }; }
		virtual inline void SetWorldPosition(const Vector& pos)					override {}
		virtual inline const Rotator& GetRelativeRotation()				const	override { return { 0,0,0 }; }
		virtual inline void SetRelativeRotation(const Rotator& rot)				override {}
		virtual inline Rotator GetWorldRotation()						const	override { return { 0,0,0 }; }
		virtual inline void SetWorldRotation(const Rotator& rot)				override {}
		virtual inline const Vector& GetRelativeScale()					const	override { return { 0,0,0 }; }
		virtual inline void SetRelativeScale(const Vector& scale)				override {}
		virtual inline Vector GetWorldScale()							const	override { return { 0,0,0 }; }
		virtual inline void SetWorldScale(const Vector& scale)					override {}
#pragma warning( pop )

	protected:
		static bool s_EnableDebugRendering;
	};

}
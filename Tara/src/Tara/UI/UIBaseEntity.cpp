#include "tarapch.h"
#include "UIBaseEntity.h"
#include "Tara/Core/Application.h"
#include "Tara/Renderer/Renderer.h"

#include "Tara/Utility/Profiler.h"

namespace Tara{

	bool UIBaseEntity::s_EnableDebugRendering = false;

	UIBaseEntity::UIBaseEntity(EntityNoRef parent, LayerNoRef owningLayer, const std::string& name)
		: Entity(parent, owningLayer, { {0,0,0},{0,0,0},{0,0,0} }, name),
		m_Spacing({0,0}),
		m_Border({0,0,0,0}),
		m_SnapRules(UISnapRule::TOP | UISnapRule::LEFT | UISnapRule::BOTTOM | UISnapRule::RIGHT),
		m_Offsets(0,0,0,0),
		m_DesiredSizeDirty(true),
		m_RenderAreaCache(0,0,0,0),
		m_RenderAreaCacheDirty(true)
	{
		SetUpdateChildrenFirst(false);
		SetUpdateComponentsFirst(true);
	}

	glm::vec2 UIBaseEntity::GetDesiredSize() const
	{
		return UIBox::DecompressBoxAndSize(m_Transform).second;
	}

	void UIBaseEntity::CalculateDesiredSize()
	{
		//for the base entity, the desired size is that of the largest 
		//child for x and y independantly
		auto& children = GetChildren();
		glm::vec2 size{ 0,0 };
		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				auto childDesiredSize = asUI->GetDesiredSize();
				size.x = std::max(size.x, childDesiredSize.x);
				size.y = std::max(size.y, childDesiredSize.y);
			}
		}
		//add the border on, as that will take space too
		auto border = GetBorder();
		size.x += border.x + border.y;
		size.y += border.z + border.w;
		//set the size
		SetDesiredSize(size);
	}

	void UIBaseEntity::SetDesiredSize(glm::vec2 size)
	{
		m_Transform = UIBox::CompressBoxAndSize(UIBox::DecompressBoxAndSize(m_Transform).first, size);
		auto parent = GetParent().lock();
		if (parent) {
			auto pparent = std::dynamic_pointer_cast<Tara::UIBaseEntity>(parent);
			if (pparent) {
				pparent->m_DesiredSizeDirty = true;
			}
		}
	}
	
	void UIBaseEntity::SetAllowedArea(const UIBox& area)
	{
		auto prev = UIBox::DecompressBoxAndSize(m_Transform);
		m_Transform = UIBox::CompressBoxAndSize(area, prev.second);
		m_RenderAreaCacheDirty = true;
	}
	
	UIBox UIBaseEntity::GetAllowedArea() const
	{
		return UIBox::DecompressBoxAndSize(m_Transform).first;
	}
	


	UIBox UIBaseEntity::GetRenderArea() const
	{
		//SCOPE_PROFILE("GetRenderArea");
		if (!m_RenderAreaCacheDirty) {
			return m_RenderAreaCache;
		}
		m_RenderAreaCacheDirty = false;

		UIBox allowed = GetAllowedArea();
		//LOG_S(INFO) << "Allowed Area: {" << allowed.x1 << "," << allowed.y1 << "} - {" << allowed.x2 << "," << allowed.y2;
		glm::vec2 desired = GetDesiredSize();
		UIBox target{ 0,0,0,0 };
		uint32_t snapRules = GetSnapRules();
		glm::vec4 offsets = GetOffsets(); //left, right, top, bottom
		//Get each corner

		//left
		if (snapRules & UISnapRule::LEFT) {
			target.x1 = allowed.x1 + offsets.x;
		}
		else if (snapRules & UISnapRule::CENTER_HORIZONTAL) {
			target.x1 = allowed.x1 + (allowed.Width() / 2) - (desired.x / 2) - offsets.x;
		}
		else if (snapRules & UISnapRule::RIGHT) {
			target.x1 = allowed.x2 - (desired.x + (offsets.x + offsets.y));
		}
		else {
			//no rule!
			LOG_S(WARNING) << "UIEntity has no horizontal snap rules! This is an issue.";
			target.x1 = allowed.x1 + offsets.x;
		}

		//right (done in reverse of left)
		if (snapRules & UISnapRule::RIGHT) {
			target.x2 = allowed.x2 - offsets.y;
		}
		else if (snapRules & UISnapRule::CENTER_HORIZONTAL) {
			target.x2 = allowed.x2 - (allowed.Width() / 2) + (desired.x / 2) + offsets.y;
		}
		else if (snapRules & UISnapRule::LEFT) {
			target.x2 = allowed.x1 + (desired.x + (offsets.x + offsets.y));
		}
		else {
			//no rule!
			//warning will have been done above
			target.x2 = allowed.x2 - offsets.y;
		}


		//bottom
		if (snapRules & UISnapRule::TOP) {
			target.y1 = allowed.y1 + offsets.z;
		}
		else if (snapRules & UISnapRule::CENTER_VERTICAL) {
			target.y1 = allowed.y1 + (allowed.Height() / 2) - (desired.y / 2) - offsets.z;
		}
		else if (snapRules & UISnapRule::BOTTOM) {
			target.y1 = allowed.y2 - (desired.y + (offsets.z + offsets.w));
		}
		else {
			//no rule!
			LOG_S(WARNING) << "UIEntity has no vertical snap rules! This is an issue.";
			target.y1 = allowed.y1 + offsets.z;
		}

		//top (done in reverse of bottom)
		if (snapRules & UISnapRule::BOTTOM) {
			target.y2 = allowed.y2 - offsets.w;
		}
		else if (snapRules & UISnapRule::CENTER_VERTICAL) {
			target.y2 = allowed.y2 - (allowed.Height() / 2) + (desired.y / 2) + offsets.w;
		}
		else if (snapRules & UISnapRule::TOP) {
			target.y2 = allowed.y1 + (desired.y + (offsets.z + offsets.w));
		}
		else {
			//no rule!
			//warning will have been done above
			target.y2 = allowed.y2 - offsets.w;
		}

		//clamp the target to the allowed area
		target.x1 = std::clamp(target.x1, allowed.x1, allowed.x2);
		target.x2 = std::clamp(target.x2, allowed.x1, allowed.x2);
		
		target.y1 = std::clamp(target.y1, allowed.y1, allowed.y2);
		target.y2 = std::clamp(target.y2, allowed.y1, allowed.y2);

		//LOG_S(INFO) << "Render Area: {" << target.x1 << "," << target.y1 << "} - {" << target.x2 << "," << target.y2;
		target.Rectify();
		m_RenderAreaCache = target;
		return target;
	}

	void UIBaseEntity::OnUpdate(float deltaTime)
	{
		//the idea here is that the Allowed area of all the children should be updated every frame
		
		//update desiered size
		if (m_DesiredSizeDirty) {
			CalculateDesiredSize();
			m_DesiredSizeDirty = false;
		}

		//get the allowed area for this entity, and remove the border from it
		UIBox allowed =  GetRenderArea();
		glm::vec4 border = GetBorder();
		allowed.x1 += border.x; //left
		allowed.x2 -= border.y; //right
		allowed.y2 -= border.z; //top
		allowed.y1 += border.w; //bottom

		auto& children = GetChildren();
		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				//normally, this would be needed, but we are ignoring it.
				//Other sublcasses of UIBaseEntity will probably override OnUpdate
				//themselves, for the same reason, as they want to do it differently. EX, a UIVerticalListEntity
				
				//auto childDesiredSize = asUI->GetDesiredSize();

				//just set the allowed area. Children of UIBase Entity are allowed to overlap
				asUI->SetAllowedArea(allowed);
			}
		}

	}
	void UIBaseEntity::OnEvent(Event& e)
	{
		//mark the desired size dirty if any hirarchy-related events happen
		if (e.GetCategoryFlags() & EventCategoryHirarchy) {
			m_DesiredSizeDirty = true;
		}

		EventFilter filter(e);
		filter.Call<WindowResizeEvent>([this](WindowResizeEvent& ee) {
			if (!(this->GetParent().lock())) {
				//no parent, resize to window size
				this->SetAllowedArea(UIBox(0.0f, 0.0f, (float)ee.GetWidth(), (float)ee.GetHeight()));
			}
			return false;
		});
	}

	inline void UIBaseEntity::OnBeginPlay()
	{
		ListenForEvents(true);
		auto window = Application::Get()->GetWindow();
		if (window) {
			//LOG_S(INFO) << "Window Dimentions: " << window->GetWidth() << ", " << window->GetHeight();
			SetAllowedArea(UIBox{ 0.0f,0.0f, (float)window->GetWidth(), (float)window->GetHeight() });
		}
		else {
			SetAllowedArea(UIBox{ 0.0f,0.0f,1.0f,1.0f }); //default unit box
		}

	}


	inline BoundingBox UIBaseEntity::GetSpecificBoundingBox() const
	{
		auto renderArea = GetRenderArea();
		return BoundingBox(renderArea.x1, renderArea.y1, 0, renderArea.Width(), renderArea.Height(), 1);
	}

	void UIBaseEntity::OnDraw(float deltaTime)
	{
		//Debug rendering of invisible entities
		if (s_EnableDebugRendering){
			auto target = GetRenderArea();

			srand((unsigned int)(this)); //use this as the seed, so every time its the same color, but all spacers are different
			glm::vec4 randomColor{
				rand() % 100 / 100.0f,
				rand() % 100 / 100.0f,
				rand() % 100 / 100.0f,
				1
			};
			//TEMP just fill allowed area
			Renderer::Quad(target.ToTransform(), randomColor);
		}
	}
}

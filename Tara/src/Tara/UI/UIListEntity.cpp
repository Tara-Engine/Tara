#include "tarapch.h"
#include "UIListEntity.h"

#include "Tara/Renderer/Renderer.h"

namespace Tara{
	UIListEntity::UIListEntity(EntityNoRef parent, LayerNoRef owningLayer, const std::string& name)
		: UIBaseEntity(parent, owningLayer, name), m_Direction(Direction::Vertical)
	{}

	void UIListEntity::CalculateDesiredSize()
	{
		//unlike base version, this has to take into account that all children must be stacked
		auto& children = GetChildren();
		glm::vec2 size{ 0,0 };
		auto spacing = GetSpacing();
		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				auto childDesiredSize = asUI->GetDesiredSize();
				//adjust sizes to also get the offsets
				auto childOffsets = asUI->GetOffsets();
				childDesiredSize.x += childOffsets.x + childOffsets.y;
				childDesiredSize.y += childOffsets.z + childOffsets.w;

				if (m_Direction == Direction::Vertical) {
					size.x = std::max(size.x, childDesiredSize.x);
					size.y += (childDesiredSize.y + spacing.y);
				}
				else {
					size.x += (childDesiredSize.x + spacing.x);
					size.y = std::max(size.y, childDesiredSize.y);
				}
			}
		}
		//add the border on, as that will take space too
		auto border = GetBorder();
		size.x += border.x + border.y;
		size.y += border.z + border.w;

		if (m_Direction == Direction::Vertical) {
			size.y -= spacing.y;
		}
		else {
			size.x -= spacing.x;
		}
		//set the size
		SetDesiredSize(size);
	}

	void UIListEntity::OnUpdate(float deltaTime)
	{
		//the idea here is that the Allowed area of all the children should be updated every frame
		
		//update desiered size
		if (m_DesiredSizeDirty) {
			CalculateDesiredSize();
			m_DesiredSizeDirty = false;
		}

		//get the allowed area for this entity, and remove the border from it
		UIBox allowed = GetRenderArea();
		glm::vec4 border = GetBorder();
		auto spacing = GetSpacing();
		allowed.x1 += border.x; //left
		allowed.x2 -= border.y; //right
		allowed.y2 -= border.z; //top
		allowed.y1 += border.w; //bottom

		auto& children = GetChildren();
		float totalHeight = 0;
		std::vector<float> heightParts;//make a vector with initial size. Should not have resizing
		heightParts.reserve(children.size());

		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				auto childDesiredSize = asUI->GetDesiredSize();
				//adjust sizes to also get the offsets
				auto childOffsets = asUI->GetOffsets();
				childDesiredSize.x += childOffsets.x + childOffsets.y;
				childDesiredSize.y += childOffsets.z + childOffsets.w;
				
				if (m_Direction == Direction::Vertical){
					heightParts.push_back(childDesiredSize.y);
					totalHeight += childDesiredSize.y + spacing.y;
				}
				else {
					heightParts.push_back(childDesiredSize.x);
					totalHeight += childDesiredSize.x + spacing.x;
				}
			}
		}
		
		int i = 0;
		UIBox unique{ allowed.x1, allowed.y1, allowed.x2, allowed.y2 };
		for (auto& child : children) {
			auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
			if (asUI) {
				//move unique to next slot
				//take the base (y1), and the total height scaled by the percentage that is wanted.
				if (m_Direction == Direction::Vertical) {
					unique.y1 = unique.y2 - (allowed.Height() * (heightParts[i] / totalHeight));
				}
				else {
					unique.x2 = unique.x1 + allowed.Width() * (heightParts[i] / totalHeight);
				}

				//set the child's allowed area
				asUI->SetAllowedArea(unique);

				//adjust unique start for spacing
				//ie, take end of last block, and move it up by spacing, then set to first
				if (m_Direction == Direction::Vertical){
					unique.y2 = unique.y1 - spacing.y;
				}
				else {
					unique.x1 = unique.x2 + spacing.x;
				}

				//adjust i
				i++;
			}
		}
	}
	
	void UIListEntity::OnDraw(float deltaTime)
	{
		if (s_EnableDebugRendering){
			srand((unsigned int)(this));


			//get the allowed area for this entity, and remove the border from it
			UIBox allowed = GetRenderArea();
			
			//draw full box
			glm::vec4 randomColor{
				rand() % 100 / 100.0f,
				rand() % 100 / 100.0f,
				rand() % 100 / 100.0f,
				1
			};
			//TEMP just fill allowed area
			Renderer::Quad(allowed.ToTransform(), randomColor);


			glm::vec4 border = GetBorder();
			auto spacing = GetSpacing();
			allowed.x1 += border.x; //left
			allowed.x2 -= border.y; //right
			allowed.y2 -= border.z; //top
			allowed.y1 += border.w; //bottom

			auto& children = GetChildren();
			float totalHeight = 0;
			std::vector<float> heightParts;//make a vector with initial size. Should not have resizing
			heightParts.reserve(children.size());

			for (auto& child : children) {
				auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
				if (asUI) {
					auto childDesiredSize = asUI->GetDesiredSize();
					//adjust sizes to also get the offsets
					auto childOffsets = asUI->GetOffsets();
					childDesiredSize.x += childOffsets.x + childOffsets.y;
					childDesiredSize.y += childOffsets.z + childOffsets.w;

					if (m_Direction == Direction::Vertical) {
						heightParts.push_back(childDesiredSize.y);
						totalHeight += childDesiredSize.y + spacing.y;
					}
					else {
						heightParts.push_back(childDesiredSize.x);
						totalHeight += childDesiredSize.x + spacing.x;
					}
				}
			}

			int i = 0;
			UIBox unique{ allowed.x1, allowed.y1, allowed.x2, allowed.y2 };
			for (auto& child : children) {
				auto asUI = std::dynamic_pointer_cast<Tara::UIBaseEntity>(child);
				if (asUI) {
					//move unique to next slot
					//take the base (y1), and the total height scaled by the percentage that is wanted.
					if (m_Direction == Direction::Vertical) {
						unique.y1 = unique.y2 - (allowed.Height() * (heightParts[i] / totalHeight));
					}
					else {
						unique.x2 = unique.x1 + allowed.Width() * (heightParts[i] / totalHeight);
					}

					
					glm::vec4 randomColor{ 
						rand() % 100 / 100.0f,
						rand() % 100 / 100.0f,
						rand() % 100 / 100.0f,
						1 
					};

					Renderer::Quad(unique.ToTransform(), randomColor);

					//adjust unique start for spacing
					//ie, take end of last block, and move it up by spacing, then set to first
					if (m_Direction == Direction::Vertical) {
						unique.y2 = unique.y1 - spacing.y;
					}
					else {
						unique.x1 = unique.x2 + spacing.x;
					}

					//adjust i
					i++;
				}
			}
		}
	}
	

}

#pragma once
#include "Tara/UI/UIBaseEntity.h"
#include "Tara/Asset/Font.h"

namespace Tara {
	REFTYPE(UITextEntity);
	NOREFTYPE(UITextEntity);

	class UITextEntity : public UIBaseEntity {
	public:
		UITextEntity(EntityNoRef parent, LayerNoRef owningLayer, FontRef font, const std::string& name = "UITextEntity");

		virtual ~UITextEntity() = default;

		/// <summary>
		/// Get the font used to render
		/// </summary>
		/// <returns></returns>
		inline const FontRef& GetFont() const { return m_Font; }

		/// <summary>
		/// Set the font used to render
		/// </summary>
		/// <param name="font"></param>
		inline void SetFont(const FontRef& font) { m_Font = font; m_TextDirty = true; }

		/// <summary>
		/// Get the color of the text
		/// </summary>
		/// <returns></returns>
		inline const glm::vec4& GetColor() const { return m_Color; }

		/// <summary>
		/// Set the color of the text
		/// </summary>
		/// <param name="color"></param>
		inline void SetColor(const glm::vec4& color) { m_Color = color; }

		/// <summary>
		/// Get the currently displayed text
		/// </summary>
		/// <returns></returns>
		inline const std::string& GetText() const { return m_Text; }

		/// <summary>
		/// Set the currently displayed text
		/// </summary>
		/// <param name="string"></param>
		inline void SetText(const std::string& string) { m_Text = string; m_TextDirty = true; }

		/// <summary>
		/// Get the size of the text in pixels
		/// </summary>
		/// <returns></returns>
		inline float GetTextSize() const { return m_TextSize; }

		/// <summary>
		/// Set the size of the text in pixels
		/// </summary>
		/// <param name="size"></param>
		inline void SetTextSize(float size) { m_TextSize = size; ; m_TextDirty = true;}
	public:
		//overriden stuff

		virtual void CalculateDesiredSize();

		virtual void OnDraw(float deltaTime) override;

		virtual void OnUpdate(float deltaTime) override;

		inline virtual void OnBeginPlay() override {}
	private:
		//private internal functions

		/// <summary>
		/// called to update the display string and meshes.
		/// </summary>
		void UpdateDisplayString();
	private:
		FontRef m_Font;
		glm::vec4 m_Color;
		std::string m_Text;
		std::vector<Transform> m_CachedTrasforms;
		std::vector<glm::vec2> m_CachedMinUVs;
		std::vector<glm::vec2> m_CachedMaxUVs;
		glm::vec2 m_TextMeshSize;
		float m_TextSize;
		float m_VerticalOffset;

		bool m_TextDirty;
	};
}


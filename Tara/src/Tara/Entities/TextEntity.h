#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Asset/Font.h"

namespace Tara {

	REFTYPE(TextEntity)
	NOREFTYPE(TextEntity)

	/// <summary>
	/// 
	/// </summary>
	class TextEntity : public Entity {
	public:

		/// <summary>
		/// Constructor. Use Tara::CreateEntity<Tara::TextEntity>(...) with same params
		/// </summary>
		/// <param name="parent">the parent entity</param>
		/// <param name="owningLayer">the owning layer</param>
		/// <param name="font">the font to use (nesecary)</param>
		/// <param name="text">the text to render (defaults to "Hello World!")</param>
		/// <param name="transform">the Transform (defualts to world origin)</param>
		/// <param name="name">the entity name (defaults to "TextEntity")</param>
		TextEntity(EntityNoRef parent, LayerNoRef owningLayer, FontRef font, const std::string& text = "Hello World!", Transform transform = TRANSFORM_DEFAULT, std::string name = "TextEntity");

		virtual ~TextEntity() = default;

		virtual void OnDraw(float deltaTime) override;

		/// <summary>
		/// Set the font. Will print an error and ignore if the new font is nullptr.
		/// </summary>
		/// <param name="font">the new font</param>
		inline void SetFont(FontRef font) { if (font) { m_Font = font; m_CacheDirty = true; } else { LOG_S(ERROR) << "TextEntity must have a valid font. Ignoring set to null"; } }
		
		/// <summary>
		/// Get the current font
		/// </summary>
		/// <returns>the font</returns>
		inline const FontRef& GetFont() { return m_Font; }

		/// <summary>
		/// Set the text to be rendered
		/// </summary>
		/// <param name="text">the new text</param>
		inline void SetText(const std::string& text) { m_Text = text; m_CacheDirty = true; }
		
		/// <summary>
		/// Get the current text
		/// </summary>
		/// <returns>the current text</returns>
		inline const std::string& GetText() { return m_Text; }

		/// <summary>
		/// Set the tint of the texture
		/// </summary>
		/// <param name="color">the new color</param>
		inline void SetColor(glm::vec4 tint) { m_Color = tint; }

		/// <summary>
		/// Set the tint of the texture
		/// </summary>
		/// <param name="r">red component</param>
		/// <param name="g">green component</param>
		/// <param name="b">blue component</param>
		/// <param name="a">alpha component</param>
		inline void SetColor(float r, float g, float b, float a = 1.0f) { m_Color = { r,g,b,a }; }

		/// <summary>
		/// Get the tint of the texture
		/// </summary>
		/// <returns>the current color</returns>
		inline const glm::vec4& GetColor() const { return m_Color; }
	public:
		//Lua stuff
		void __SCRIPT__SetColor(sol::object r, sol::object g, sol::object b, sol::object a);
		sol::table __SCRIPT__GetColor();
		/// <summary>
		/// Register the lua type
		/// </summary>
		/// <param name="lua"></param>
		static void RegisterLuaType(sol::state& lua);

	private:
		FontRef m_Font;
		std::string m_Text;
		glm::vec4 m_Color;
		//the quads and uvs are cached for more efficent rendering times, only changed when makred "dirty" by setting text or font
		std::vector<Transform> m_CachedTrasforms;
		std::vector<glm::vec2> m_CachedMinUVs;
		std::vector<glm::vec2> m_CachedMaxUVs;
		bool m_CacheDirty = true;
	};

}

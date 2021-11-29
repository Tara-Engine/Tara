#include "tarapch.h"
#include "TextEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Core/Script.h"

namespace Tara{
	TextEntity::TextEntity(EntityNoRef parent, LayerNoRef owningLayer, FontRef font, const std::string& text, Transform transform, std::string name)
		: Entity(parent, owningLayer, transform, name), m_Font(font), m_Text(text), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_CacheDirty(true)
	{
	}

	void TextEntity::OnDraw(float deltaTime)
	{
		if (m_CacheDirty){
			//if and only if the cache is dirty should we regenerate.
			m_Font->GetTextQuads(m_Text, m_CachedTrasforms, m_CachedMinUVs, m_CachedMaxUVs);
			m_CacheDirty = false;
		}
		auto t = GetWorldTransform(); //cache of world transform
		auto& tex = m_Font->GetTexture(); //cache of texture. audo does deduce const, but not &
		//for each character, draw its font. Don't use m_Text.size() because there may be more transforms than in the original text. using \t is an example of this
		for (int i = 0; i < m_CachedTrasforms.size(); i++) {
			if (m_CachedMinUVs[i] == glm::vec2(0, 0) && m_CachedMaxUVs[i] == glm::vec2(0, 0)) {
				//there is more allocated cells than displayed characters.
				break;
			}
			Renderer::Quad(t + m_CachedTrasforms[i], m_Color, tex, m_CachedMinUVs[i], m_CachedMaxUVs[i]);
		}
	}

	void TextEntity::__SCRIPT__SetColor(sol::object r, sol::object g, sol::object b, sol::object a)
	{
		glm::vec4 color = { 1, 1, 1, 1 };
		if (r.valid()) {
			if (r.get_type() == sol::type::table) {
				//dealing with a table
				sol::table tbl = r.as<sol::table>();
				color.r = tbl.get_or("r", tbl.get_or("red", tbl.get_or("x", tbl.get_or(1, 1.0f))));
				color.g = tbl.get_or("g", tbl.get_or("green", tbl.get_or("y", tbl.get_or(2, 1.0f))));
				color.b = tbl.get_or("b", tbl.get_or("blue", tbl.get_or("z", tbl.get_or(3, 1.0f))));
				color.a = tbl.get_or("a", tbl.get_or("alpha", tbl.get_or("w", tbl.get_or(4, 1.0f))));
				SetColor(color);
				return;
			}
			else if (r.is<float>()) {
				color.r = r.as<float>();
			}
			else {
				LOG_S(ERROR) << "Lua: Called ScriptEntity::SetTint with invalid first value. use a table or a series of values";
			}
		}
		if (g.valid() && g.is<float>()) {
			color.g = g.as<float>();
		}
		if (b.valid() && b.is<float>()) {
			color.b = b.as<float>();
		}
		if (a.valid() && a.is<float>()) {
			color.a = a.as<float>();
		}
		SetColor(color);
	}

	sol::table TextEntity::__SCRIPT__GetColor()
	{
		auto table = sol::table(Script::Get()->GetState(), sol::create);
		table["r"] = m_Color.r;
		table["g"] = m_Color.g;
		table["b"] = m_Color.b;
		table["a"] = m_Color.a;
		return table;
	}

	void TextEntity::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<TextEntity> type = lua.new_usertype<TextEntity>("TextEntity", sol::base_classes, sol::bases<Entity>());
		CONNECT_METHOD(TextEntity, SetFont);
		CONNECT_METHOD(TextEntity, GetFont);
		CONNECT_METHOD(TextEntity, SetText);
		CONNECT_METHOD(TextEntity, GetText);

		CONNECT_METHOD_OVERRIDE(TextEntity, SetColor); // table form
		CONNECT_METHOD_OVERRIDE(TextEntity, GetColor); // table form
	}


}

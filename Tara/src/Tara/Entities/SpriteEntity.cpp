#include "tarapch.h"
#include "SpriteEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Core/Script.h"

namespace Tara {

	SpriteEntity::SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, const std::string& name, SpriteRef sprite)
		: Entity(parent, owningLayer, transform, name), m_Sprite(sprite), 
		m_CurrentFrame(0), m_CurrentSequence(0,0,0.0f), m_CurrentFrameTimer(0.0f), 
		m_Tint(1.0f,1.0f,1.0f,1.0f), m_FlipBits(0)
	{}

	void SpriteEntity::OnUpdate(float deltaTime)
	{
		if (m_CurrentSequence.IFrameRate > 0.0f) {
			m_CurrentFrameTimer += deltaTime;
			if (m_CurrentFrameTimer > m_CurrentSequence.IFrameRate) {
				m_CurrentFrameTimer = 0.0f;
				m_CurrentFrame++;
				if (m_CurrentFrame > m_CurrentSequence.End) {
					m_CurrentFrame = m_CurrentSequence.Start;
				}
			}
		}
	}

	void SpriteEntity::OnDraw(float deltaTime)
	{
		if (m_Sprite){
			auto UVs = m_Sprite->GetUVsForFrame(m_CurrentFrame);
			if (m_FlipBits & SPRITE_FLIP_H) {
				auto tmp = UVs.first.x;
				UVs.first.x = UVs.second.x;
				UVs.second.x = tmp;
			}
			if (m_FlipBits & SPRITE_FLIP_V) {
				auto tmp = UVs.first.y;
				UVs.first.y = UVs.second.y;
				UVs.second.y = tmp;
			}
			Renderer::Quad(GetWorldTransform(), m_Tint, m_Sprite->GetTexture(), UVs.first, UVs.second);
		}
		else {
			//draw color if no asset
			Renderer::Quad(GetWorldTransform(), m_Tint);
		}
	}

	void SpriteEntity::PlayAnimation(const std::string& name)
	{
		auto anim = m_Sprite->GetAnimationSequence(name);
		if (anim) {
			SetCurrentSequence(*anim);
		}
		else {
			LOG_S(WARNING) << "Animation: \"" << name << "\" not available for this sprite.";
		}
	}



	void SpriteEntity::__SCRIPT__SetCurrentSequence(sol::object seq)
	{
		if (seq.valid() && seq.is<sol::table>()) {
			sol::table t = seq.as<sol::table>();
			Sprite::AnimationSequence seq(0, 0, 0.0f);
			seq.Start = t.get_or("Start", 0);
			seq.End = t.get_or("End", 0);
			seq.IFrameRate = t.get_or("IFrameRate", 0.0f);
		}
	}

	sol::table SpriteEntity::__SCRIPT__GetCurrentSequence() const
	{
		auto table = sol::table(Script::Get()->GetState(), sol::create);
		auto seq = GetCurrentSequence();
		table["Start"] = seq.Start;
		table["End"] = seq.End;
		table["IFrameRate"] = seq.IFrameRate;
		return table;
	}

	void SpriteEntity::__SCRIPT__SetTint(sol::object r, sol::object g, sol::object b, sol::object a)
	{
		glm::vec4 color = { 1, 1, 1, 1 };
		if (r.valid()) {
			if (r.get_type() == sol::type::table) {
				//dealing with a table
				sol::table tbl = r.as<sol::table>();
				color.r = tbl.get_or("r", tbl.get_or("red",   tbl.get_or("x", tbl.get_or(1, 1.0f))));
				color.g = tbl.get_or("g", tbl.get_or("green", tbl.get_or("y", tbl.get_or(2, 1.0f))));
				color.b = tbl.get_or("b", tbl.get_or("blue",  tbl.get_or("z", tbl.get_or(3, 1.0f))));
				color.a = tbl.get_or("a", tbl.get_or("alpha", tbl.get_or("w", tbl.get_or(4, 1.0f))));
				SetTint(color);
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
		SetTint(color);
	}

	void SpriteEntity::__SCRIPT__SetFlip(sol::object one, sol::object two)
	{
		uint32_t bits = 0;
		if (one.valid() && one.is<char*>()) {
			char* s = one.as<char*>();
			if (s == "FlipX" || s == "FlipHorizontal" || s == "SPRITE_FLIP_H") {
				bits |= SPRITE_FLIP_H;
			}
			else if (s == "FlipY" || s == "FlipVertical" || s == "SPRITE_FLIP_V") {
				bits |= SPRITE_FLIP_V;
			}
		}
		if (two.valid() && two.is<char*>()) {
			char* s = two.as<char*>();
			if (s == "FlipX" || s == "FlipHorizontal" || s == "SPRITE_FLIP_H") {
				bits |= SPRITE_FLIP_H;
			}
			else if (s == "FlipY" || s == "FlipVertical" || s == "SPRITE_FLIP_V") {
				bits |= SPRITE_FLIP_V;
			}
		}
		SetFlip(bits);
	}

	std::tuple<bool, bool> SpriteEntity::__SCRIPT__GetFlip() const
	{
		return std::make_tuple((bool)(m_FlipBits & SPRITE_FLIP_H), (bool)(m_FlipBits & SPRITE_FLIP_V));
	}

	void SpriteEntity::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<SpriteEntity> type = lua.new_usertype<SpriteEntity>("SpriteEntity", sol::base_classes, sol::bases<Entity>());
		CONNECT_METHOD(SpriteEntity, SetSprite);
		CONNECT_METHOD(SpriteEntity, GetSprite);
		CONNECT_METHOD(SpriteEntity, SetCurrentFrame);
		CONNECT_METHOD(SpriteEntity, GetCurrentFrame);
		CONNECT_METHOD(SpriteEntity, PlayAnimation);

		CONNECT_METHOD_OVERRIDE(SpriteEntity, GetCurrentSequence); // table form
		CONNECT_METHOD_OVERRIDE(SpriteEntity, SetCurrentSequence); // table form
		CONNECT_METHOD_OVERRIDE(SpriteEntity, SetTint); // table form
		CONNECT_METHOD_OVERRIDE(SpriteEntity, SetFlip); // string pair form
		CONNECT_METHOD_OVERRIDE(SpriteEntity, GetFlip); // return pair of strings

	}

}
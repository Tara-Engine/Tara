#pragma once
#include "Tara/Core/Layer.h"
#include "Tara/Core/Entity.h"
#include "Tara/Asset/Sprite.h"

#define SPRITE_FLIP_H 0x1
#define SPRITE_FLIP_V 0x2
//#define SPRITE_FLIP_CORNER 0x4

namespace Tara {

	REFTYPE(SpriteEntity);
	NOREFTYPE(SpriteEntity);

	class SpriteEntity : public Entity	{

	public:
		/// <summary>
		/// Construct a new sprite entity. Should not normally be called manually, use Tara::CreateEntity<Tara::SpriteEntity> instead
		/// </summary>
		/// <param name="parent">The parent entity</param>
		/// <param name="owningLayer">The owning layer</param>
		/// <param name="transform">the transform (defaults to the world origin)</param>
		/// <param name="name">the entity's name (defaults to "SpriteEntity")</param>
		/// <param name="texture">the Sprite to render (defults to nullptr)</param>
		SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform = TRANSFORM_DEFAULT, const std::string& name = "SpriteEntity", SpriteRef sprite = nullptr);


	public:
		virtual ~SpriteEntity() {};

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnDraw(float deltaTime) override;

		//virtual void OnEvent(Event& e) override; //No need to override

		inline virtual BoundingBox GetSpecificBoundingBox() const override { return BoundingBox::FromTransform(GetWorldTransform()); }

		/// <summary>
		/// Set the sprite that this entity is using
		/// </summary>
		/// <param name="sprite">the new sprite</param>
		inline void SetSprite(SpriteRef sprite) { m_Sprite = sprite; SetCurrentFrame(m_CurrentFrame); }
		
		/// <summary>
		/// Get a reference to the current sprite that this entity is using
		/// </summary>
		/// <returns></returns>
		inline const SpriteRef& GetSprite() const { return m_Sprite; }

		/// <summary>
		/// Set the current frame of the sprite.
		/// This will cancell any animation sequence currently playing,
		/// </summary>
		/// <param name="frame"></param>
		inline void SetCurrentFrame(uint32_t frame) { 
			SetCurrentSequence(Sprite::AnimationSequence(0, 0, 0.0f)); 
			m_CurrentFrame = std::min(frame, m_Sprite->GetLastFrame());
		}
		
		/// <summary>
		/// Get the current frame of the sprite
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetCurrentFrame() const { return m_CurrentFrame; }

		/// <summary>
		/// Set the current animation sequence
		/// </summary>
		/// <param name="seq"></param>
		inline void SetCurrentSequence(const Sprite::AnimationSequence& seq) { 
			if (m_Sprite->ValidSequence(seq)) { 
				m_CurrentSequence = seq; 
				m_CurrentFrameTimer = 0.0f; 
				m_CurrentFrame = seq.Start; 
			} 
		}

		/// <summary>
		/// Get the current animation sequence
		/// </summary>
		/// <returns></returns>
		inline const Sprite::AnimationSequence& GetCurrentSequence() const { return m_CurrentSequence; }

		/// <summary>
		/// Play an animation saved in the sprite by name
		/// </summary>
		/// <param name="name"></param>
		void PlayAnimation(const std::string& name);

		/// <summary>
		/// Set the tint of the sprite
		/// </summary>
		/// <param name="tint"></param>
		inline void SetTint(glm::vec4 tint) { m_Tint = tint; }

		/// <summary>
		/// Set the tint of the sprite
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		inline void SetTint(float r, float g, float b, float a = 1.0f) { m_Tint = {r,g,b,a}; }

		/// <summary>
		/// Get the tint of the sprite
		/// </summary>
		/// <returns></returns>
		inline const glm::vec4& GetTint() const { return m_Tint; }

		/// <summary>
		/// Set the flip bits
		/// </summary>
		/// <param name="flipBits">a series of flip bit macros or'ed together</param>
		inline void SetFlip(uint8_t flipBits) { m_FlipBits = flipBits; }

		/// <summary>
		/// Get the flip bits
		/// </summary>
		/// <returns>the flit bits</returns>
		inline const uint8_t GetFlip() const { return m_FlipBits; }

	public:
		//lua stuff
		void __SCRIPT__SetCurrentSequence(sol::object seq);
		sol::table __SCRIPT__GetCurrentSequence() const;
		void __SCRIPT__SetTint(sol::object r, sol::object g, sol::object b, sol::object a);
		void __SCRIPT__SetFlip(sol::object one, sol::object two);
		std::tuple<bool, bool> __SCRIPT__GetFlip() const;

		/// <summary>
		/// Register the lua type
		/// </summary>
		/// <param name="lua"></param>
		static void RegisterLuaType(sol::state& lua);

	private:
		SpriteRef m_Sprite;
		uint32_t m_CurrentFrame;
		Sprite::AnimationSequence m_CurrentSequence;
		float m_CurrentFrameTimer;
		glm::vec4 m_Tint;
		uint8_t m_FlipBits;
	};
}
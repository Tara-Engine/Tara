#pragma once
#include "Tara/Core/Layer.h"
#include "Tara/Core/Entity.h"
#include "Tara/Asset/Sprite.h"


namespace Tara {
	class SpriteEntity : public Entity	{

	public:
		/// <summary>
		/// Construct a new sprite entity. Should not normally be called manually
		/// </summary>
		/// <param name="parent">The parent entity</param>
		/// <param name="owningLayer">The owning layer</param>
		/// <param name="transform">the transform (defaults to the world origin)</param>
		/// <param name="name">the entity's name (defaults to "SpriteEntity")</param>
		/// <param name="texture">the Sprite to render (defults to nullptr)</param>
		SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, SpriteRef sprite);

		/// <summary>
		/// Create a new sprite entity
		/// </summary>
		/// <param name="parent">The parent entity</param>
		/// <param name="owningLayer">The owning layer</param>
		/// <param name="transform">the transform (defaults to the world origin)</param>
		/// <param name="name">the entity's name (defaults to "SpriteEntity")</param>
		/// <param name="texture">the Sprite to render (defults to nullptr)</param>
		/// <returns>the new entity</returns>
		static std::shared_ptr<SpriteEntity> Create(EntityNoRef parent, LayerNoRef owningLayer, Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", SpriteRef texture = nullptr);

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

	private:
		SpriteRef m_Sprite;
		uint32_t m_CurrentFrame;
		Sprite::AnimationSequence m_CurrentSequence;
		float m_CurrentFrameTimer;
	};

	using SpriteEntityRef = std::shared_ptr<SpriteEntity>;
}
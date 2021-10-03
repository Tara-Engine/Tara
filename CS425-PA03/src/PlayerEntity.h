#pragma once
#include "Tara/Entities/SpriteEntity.h"

//this should not be here ... 
namespace Tara {

	/// <summary>
	/// The Player is also a subclass of SpriteEntity
	/// </summary>
	class PlayerEntity : public SpriteEntity {

	public:
		//Constructor
		PlayerEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name);
		//construtor
		PlayerEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name, Texture2DRef texture);

		/// <summary>
		/// Create a new Player
		/// </summary>
		/// <param name="parent">parent entity</param>
		/// <param name="owningLayer">owning layer</param>
		/// <param name="transform">initial transform</param>
		/// <param name="name">entity name</param>
		/// <param name="texture">entity texture</param>
		/// <returns>a ref to the particle</returns>
		static std::shared_ptr<PlayerEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::Texture2DRef texture = nullptr);

		virtual ~PlayerEntity() {};
		
		/// <summary>
		/// Called every frame in the Update stage. 
		/// The player polls for keys and uses them to move.
		/// Because it polls, there is no need to listen for events
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void OnUpdate(float deltaTime) override;

		//virtual void OnDraw(float deltaTime) override; //No need to override, handled by SpriteEntity

		//virtual void OnEvent(Tara::Event& e) override; //No need to override, handled by Entity
	};
}
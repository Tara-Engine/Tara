#pragma once
#include "Tara/Entities/SpriteEntity.h"

//this should not be ehr
namespace Tara {

	/// <summary>
	/// The WallEntity is also a subclass of SpriteEntity, 
	/// only so we can cast to it in the ParticleEntity's OnEvent function
	/// So, its just a cheap, minimal wrapper
	/// </summary>
	class WallEntity : public SpriteEntity {

	public:
		//constructor
		WallEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name);
		//constructor
		WallEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name, Texture2DRef texture);

		/// <summary>
		/// Create a new wall entity
		/// </summary>
		/// <param name="parent">parent entity</param>
		/// <param name="owningLayer">owning layer</param>
		/// <param name="transform">initial transform</param>
		/// <param name="name">entity name</param>
		/// <param name="texture">entity texture</param>
		/// <returns>a ref to the particle</returns>
		static std::shared_ptr<WallEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::Texture2DRef texture = nullptr);

		virtual ~WallEntity() {};

		//virtual void OnUpdate(float deltaTime) override; //No need to override

		//virtual void OnDraw(float deltaTime) override; //No need to override

		//virtual void OnEvent(Tara::Event& e) override; //No need to override
	};
}
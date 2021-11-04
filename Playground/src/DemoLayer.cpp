#include "DemoLayer.h"
#include <random>

DemoLayer::DemoLayer()
{
}

DemoLayer::~DemoLayer()
{
}

void DemoLayer::Activate()
{
	//Most of our work is here!
	
	//first, load some assets

	//load a texture file
	auto playerTexture = Tara::Texture2D::Create("assets/Particle.png", "playerTexture"); 
	auto otherTexture = Tara::Texture2D::Create("assets/UV_Checker.png", "UV_Checker");

	//take that texture and turn it into a sprite (which can be animated, but is not in this case)
	auto playerSprite = Tara::Sprite::Create(playerTexture, 1, 1, "playerSprite"); 
	auto otherSprite = Tara::Sprite::Create(otherTexture, 1, 1, "UV_Checker_sprite");


	//now, make some entities!

	//make some background entities using the other sprite, so they are behind the player
	auto rd = std::random_device();
	for (int i = 0; i < 10; i++) {
		Tara::CreateEntity<Tara::SpriteEntity>(
			Tara::EntityNoRef(),
			weak_from_this(),
			TRANSFORM_2D((rd()%18) - 9.0f, (rd()&10) - 5.0f, 0, 1, 1),
			"randomEntity", //yes, they can all have the same name!
			otherSprite
		);
	}
	
	//make our player entity using the player sprite
	auto player = Tara::CreateEntity<Tara::SpriteEntity>(
		Tara::EntityNoRef(),
		weak_from_this(),
		TRANSFORM_2D(0, 0, 0, 1, 1),
		"playerEntity",
		playerSprite
	);
	

	//make a camera, and attach it to the player
	//no need to supply transform, it will have a default value. Same with its name.
	auto camera = Tara::CreateEntity<Tara::CameraEntity>(player, weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT);
	camera->SetOrthographicExtent(16); //set the camera to view an area 16 units wide

	//Tell the layer we want to use this camera for rendering
	SetLayerCamera(camera);
	
	//The player needs movement controls. Make a script to do that! (yay lua!)
	auto playerController = Tara::CreateComponent<Tara::ScriptComponent>(player, "assets/PlayerController.lua", "playerController");
}

void DemoLayer::Deactivate()
{
	Deactivate();
}

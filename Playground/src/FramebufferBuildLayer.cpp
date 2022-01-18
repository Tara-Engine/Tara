#include "FramebufferBuildLayer.h"
#include <random>
//access to raw OpenGL calls






FramebufferBuildLayer::FramebufferBuildLayer()
	: Tara::Layer()
{}

FramebufferBuildLayer::~FramebufferBuildLayer()
{
	Deactivate();
}

void FramebufferBuildLayer::Activate()
{
	//load two texture files
	auto playerTexture = Tara::Texture2D::Create("assets/Particle.png", "playerTexture");
	auto otherTexture = Tara::Texture2D::Create("assets/UV_Checker.png", "UV_Checker");

	//take those textures and turn them into sprites (which can be animated, but is not in this case)
	auto playerSprite = Tara::Sprite::Create(playerTexture, 1, 1, "playerSprite");
	auto otherSprite = Tara::Sprite::Create(otherTexture, 1, 1, "UV_Checker_sprite");

	//now, make some entities!

	//make some background entities using the other sprite, so they are behind the player
	auto rd = std::random_device();
	for (int i = 0; i < 10; i++) {
		auto entity = Tara::CreateEntity<Tara::SpriteEntity>(
			Tara::EntityNoRef(),
			weak_from_this(),
			TRANSFORM_2D((rd() % 18) - 9.0f, (rd() % 10) - 5.0f, 0, 1, 1),
			"randomEntity", //yes, they can all have the same name!
			otherSprite
		);
		entity->SetRenderFilterBits(0xF0);
	}

	//make our player entity using the player sprite
	auto player = Tara::CreateEntity<Tara::SpriteEntity>(
		Tara::EntityNoRef(),
		weak_from_this(),
		TRANSFORM_2D(0, 0, 0, 1, 1),
		"playerEntity",
		playerSprite
		);
	player->SetRenderFilterBits(0xF0);

	//make a camera, and attach it to the player
	//no need to supply transform, it will have a default value. Same with its name.
	auto camera = Tara::CreateEntity<Tara::CameraEntity>(player, weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT, "WorldCamera");
	camera->SetOrthographicExtent(16); //set the camera to view an area 16 units wide
	camera->GetCamera()->SetRenderFilterBits(0xF0); //Choose what entities to render.
	//Tell the layer we want to use this camera for rendering
	SetLayerCamera(camera);

	//The player needs movement controls. Make a script to do that! (yay lua!)
	auto playerController = Tara::CreateComponent<Tara::ScriptComponent>(player, "assets/PlayerController.lua", "playerController");

	//make a framebuffer and SceneCamera
	

	//m_ScreenCamera = std::make_shared<Tara::ScreenCamera>();
	//m_ScreenCamera->UpdateRenderArea(1200, 700); //manual width/height. Will not auto-update in test

	auto framebuffer = Tara::RenderTarget2D::Create(1200, 700, "framebuffer"); //also matching screen size
	camera->GetCamera()->SetRenderTarget(framebuffer); //make a framebuffer
	
	auto sprite = Tara::Sprite::Create(framebuffer, 1, 1, "framebufferSprite");

	m_DisplayEntity = Tara::CreateEntity<Tara::SpriteEntity>(
		Tara::EntityNoRef(),
		weak_from_this(),
		TRANSFORM_2D(0, 700, 0, 1200, -700),
		"displayEntity",
		sprite
	);
	m_DisplayEntity->SetRenderFilterBits(0x0F); //different display bits

	m_ScreenCamera = Tara::CreateEntity<Tara::CameraEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Camera::ProjectionType::Screen, TRANSFORM_DEFAULT, "ScreenCamera");
	m_ScreenCamera->GetCamera()->SetRenderFilterBits(0x0F); //match display entity
	m_ScreenCamera->SetRenderEveryFrame(true); //also render with this camera!
}

void FramebufferBuildLayer::Deactivate()
{
}

void FramebufferBuildLayer::Draw(float deltaTime)
{
	
	////draw the layer
	Layer::Draw(deltaTime);
	//
	////render the framebuffer onto a quad
	//auto& window = Tara::Application::Get()->GetWindow();
	//
	//Tara::Renderer::BeginScene(m_ScreenCamera);
	//Tara::Renderer::Quad(TRANSFORM_2D(0, 0, 0, (float)window->GetWidth()/2, (float)window->GetHeight()/2), { 1.0f,1.0f,1.0f,1.0f }, m_Framebuffer);
	////Tara::Renderer::Quad(TRANSFORM_2D(0, 0, 0, (float)m_Framebuffer->GetWidth(), (float)m_Framebuffer->GetHeight()), { 1.0f,1.0f,1.0f,1.0f }, m_Framebuffer);
	//Tara::Renderer::EndScene();
}

void FramebufferBuildLayer::OnEvent(Tara::Event& e)
{
	Layer::OnEvent(e);

	Tara::EventFilter filter(e);
	filter.Call<Tara::WindowResizeEvent>([this](Tara::WindowResizeEvent& ee) {
		//m_Framebuffer->SetSize(ee.GetWidth(), ee.GetHeight());
		m_ScreenCamera->GetCamera()->UpdateRenderArea(ee.GetWidth(), ee.GetHeight());
		m_DisplayEntity->SetWorldPosition(Tara::Vector(0, (float)ee.GetHeight(), 0));
		m_DisplayEntity->SetWorldScale(Tara::Vector{ (float)ee.GetWidth(), -(float)ee.GetHeight(), 1.0f });
		return false;
	});
}



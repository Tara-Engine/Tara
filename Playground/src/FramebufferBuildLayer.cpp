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
		Tara::CreateEntity<Tara::SpriteEntity>(
			Tara::EntityNoRef(),
			weak_from_this(),
			TRANSFORM_2D((rd() % 18) - 9.0f, (rd() % 10) - 5.0f, 0, 1, 1),
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

	//make a framebuffer and SceneCamera
	m_ScreenCamera = std::make_shared<Tara::ScreenCamera>();
	m_ScreenCamera->UpdateRenderArea(1200, 700); //manual width/height. Will not auto-update in test

	m_Framebuffer = Tara::RenderTarget::Create(1200, 700, "framebuffer"); //also matching screen size
}

void FramebufferBuildLayer::Deactivate()
{
}

void FramebufferBuildLayer::Draw(float deltaTime)
{
	//use the framebuffer as a rendertarget
	m_Framebuffer->RenderTo(true);
	Tara::RenderCommand::Clear();
	//draw the layer
	Layer::Draw(deltaTime);
	//clear what is being rendered to the bad way
	m_Framebuffer->RenderTo(false);
	
	//render the framebuffer onto a quad
	auto& window = Tara::Application::Get()->GetWindow();

	//LOG_S(INFO) << "Window:      Width:" << window->GetWidth() << " Height: " << window->GetHeight();
	//LOG_S(INFO) << "Framebuffer: Width:" << m_Framebuffer->GetWidth() << " Height: " << m_Framebuffer->GetHeight();

	Tara::Renderer::BeginScene(m_ScreenCamera);
	Tara::Renderer::Quad(TRANSFORM_2D(0, 0, 0, (float)window->GetWidth(), (float)window->GetHeight()), { 1.0f,1.0f,1.0f,1.0f }, m_Framebuffer);
	//Tara::Renderer::Quad(TRANSFORM_2D(0, 0, 0, (float)m_Framebuffer->GetWidth(), (float)m_Framebuffer->GetHeight()), { 1.0f,1.0f,1.0f,1.0f }, m_Framebuffer);
	Tara::Renderer::EndScene();
}

void FramebufferBuildLayer::OnEvent(Tara::Event& e)
{
	Layer::OnEvent(e);

	Tara::EventFilter filter(e);
	filter.Call<Tara::WindowResizeEvent>([this](Tara::WindowResizeEvent& ee) {
		m_Framebuffer->SetSize(ee.GetWidth(), ee.GetHeight());
		m_ScreenCamera->UpdateRenderArea(ee.GetWidth(), ee.GetHeight());
		return false;
	});
}



#pragma once
#include <Tara.h>



class FramebufferBuildLayer : public Tara::Layer {
public:
	/// <summary>
	/// Constructor
	/// </summary>
	FramebufferBuildLayer();

	/// <summary>
	/// Destructor
	/// </summary>
	virtual ~FramebufferBuildLayer();

	/// <summary>
	/// Activattion function, called when layer is added to scene.
	/// graphics context and all engine systems are guaranteed to be running
	/// </summary>
	virtual void Activate() override;

	/// <summary>
	/// Deactivation function, called when layer is removed from scene.
	/// </summary>
	virtual void Deactivate() override;

	/// <summary>
	/// Update function, called every frame (before draw). 
	/// Used to handle game logic.
	/// No need to override. If you do, make sure to call Layer::Update(deltaTime)
	/// </summary>
	/// <param name="deltaTime"></param>
	//virtual void Update(float deltaTime) override;

	/// <summary>
	/// Draw function, called every frame.
	/// Used to draw stuff
	/// No need to overide. If you do, make sure to call Layer::Draw()
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void Draw(float deltaTime) override;

	/// <summary>
	/// Event handler. Called for every window event.
	/// No need to override. If you do, make sure to call Layer::OnEvent(e)
	/// </summary>
	/// <param name="e"></param>
	virtual void OnEvent(Tara::Event& e) override;

private:
	Tara::SpriteEntityRef m_DisplayEntity;
	Tara::CameraEntityRef m_ScreenCamera;
	//Tara::RenderTargetRef m_Framebuffer;
	//Tara::ScreenCameraRef m_ScreenCamera;
};
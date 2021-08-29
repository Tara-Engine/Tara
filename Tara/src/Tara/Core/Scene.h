#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"
#include "Tara/Core/Layer.h"
//TODO: build scene graph stuff

namespace Tara {


	class Scene {
	public:
		Scene();
		virtual ~Scene();

		/*
		used for "soft" loading/unloading. 
		IE, a deactivated scene should be able to be actiavated
		again, and behave just as right before it was deactivated.
		Activation is the step in construction (except for overloaded constructors!), 
		while deactivation is a step in deletion.
		*/
		virtual void Activate();
		virtual void Deactivate();

		virtual void Update();
		virtual void Draw();
		virtual void OnEvent(Event& e);



		bool PushLayer(Layer* layer);
		bool PushOverlay(Layer* layer);

	private:

		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::vector<std::unique_ptr<Layer>> m_Overlays;

	};

	using SceneRef = std::shared_ptr<Scene>;

}
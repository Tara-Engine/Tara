#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"


namespace Tara {

	class Layer {
	public:
		Layer(); 
		virtual ~Layer();

		virtual void Activate();
		virtual void Deactivate();

		virtual void Update();
		virtual void Draw();
		virtual void OnEvent(Event& e);

		//TODO: stuff relating to entities and components

	};

}
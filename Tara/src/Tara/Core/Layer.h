#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"


namespace Tara {

	/// <summary>
	/// Layer Class. Layers are "Draw slices" of a scene. 
	/// This class should be subclassed to make a custom application, and 
	/// a instance of that subclass pushed to a scene.
	/// </summary>
	class Layer {
	public:
		/// <summary>
		/// Layer Constructor
		/// </summary>
		Layer(); 
		virtual ~Layer();

		/// <summary>
		/// Soft Activation of a Layer.
		/// Automatically called when added to a scene
		/// </summary>
		virtual void Activate();
		/// <summary>
		/// Soft Deactivation of a Layer.
		/// </summary>
		virtual void Deactivate();

		/// <summary>
		/// Update function for a layer
		/// </summary>
		virtual void Update();
		/// <summary>
		/// Draw function for a Layer
		/// </summary>
		virtual void Draw();
		/// <summary>
		/// Event function for a Layer
		/// Used to pass events to the layer.
		/// </summary>
		/// <param name="e">the event</param>
		virtual void OnEvent(Event& e);

		//TODO: stuff relating to entities and components

	};

}
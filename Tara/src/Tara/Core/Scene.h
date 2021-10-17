#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"
#include "Tara/Core/Layer.h"
//TODO: build scene graph stuff

namespace Tara {

	/// <summary>
	/// Scene class, or, a level in a game.
	/// </summary>
	class Scene {
	public:
		/// <summary>
		/// Construct a new default scene
		/// </summary>
		Scene();
		virtual ~Scene();

		/// <summary>
		/// Activate a scene. DOES NOT automatically get called from constructor.
		/// Used for re-loading a deactivated scene
		/// </summary>
		virtual void Activate();
		/// <summary>
		/// Deactiveate a scene. DOES NOT automatically get called from destructor.
		/// Used for soft unloading a scene
		/// </summary>
		virtual void Deactivate();

		/// <summary>
		/// Update the Scene
		/// </summary>
		virtual void Update(float deltaTime);
		/// <summary>
		/// Draw the Scene
		/// </summary>
		virtual void Draw(float deltaTime);
		/// <summary>
		/// Pass an Event to scene
		/// </summary>
		/// <param name="e">the event</param>
		virtual void OnEvent(Event& e);


		/// <summary>
		/// Push a Layer into the Scene.
		/// The Scene takes ownership of the Layer. A Layer should never belong to two scenes, 
		/// this will cause undefined behavior.
		/// </summary>
		/// <param name="layer">the layer to push</param>
		/// <returns>true if layer pushed correctly</returns>
		bool PushLayer(LayerRef layer);
		/// <summary>
		/// Push an Overlay Layer into the Scene.
		/// The Scene takes ownership of the Layer. A Layer should never belong to two scenes, 
		/// this will cause undefined behavior.
		/// Overlay Layers are drawn after all regular layers, and should be used for UI.
		/// </summary>
		/// <param name="layer">the layer to push</param>
		/// <returns>true if layer pushed correctly</returns>
		bool PushOverlay(LayerRef layer);

		

	private:

		std::vector<LayerRef> m_Layers;
		std::vector<LayerRef> m_Overlays;

	};

	/// <summary>
	/// Reference to a Scene class, or, a level in a game.
	/// </summary>
	using SceneRef = std::shared_ptr<Scene>;

}
#include "tarapch.h"
#include "Scene.h"


namespace Tara {
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Activate()
	{
		//TODO: initialize two basic layers...
	}

	void Scene::Deactivate()
	{
	}


	void Scene::Update(float deltaTime)
	{
		for (auto& layer : m_Layers) {
			layer->Update(deltaTime);
		}
		for (auto& layer : m_Overlays) {
			layer->Update(deltaTime);
		}
	}

	void Scene::Draw(float deltaTime)
	{
		for (auto& layer : m_Layers) {
			layer->Draw(deltaTime);
		}
		for (auto& layer : m_Overlays) {
			layer->Draw(deltaTime);
		}
	}

	void Scene::OnEvent(Event& e)
	{
	}

	bool Scene::PushLayer(LayerRef layer)
	{
		m_Layers.push_back(layer);
		layer->Activate();
		return true;
	}

	bool Scene::PushOverlay(LayerRef layer)
	{
		m_Overlays.push_back(layer);
		layer->Activate();
		return false;
	}


}

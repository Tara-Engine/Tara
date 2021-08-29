#include "tarapch.h"
#include "Scene.h"


namespace Tara {
	Scene::Scene()
	{
		//TODO: initialize two basic layers...
		Activate();
	}

	Scene::~Scene()
	{
		Deactivate();

	}

	void Scene::Activate()
	{
	}

	void Scene::Deactivate()
	{
	}


	void Scene::Update()
	{
		for (auto& layer : m_Layers) {
			layer->Update();
		}
		for (auto& layer : m_Overlays) {
			layer->Update();
		}
	}

	void Scene::Draw()
	{
		for (auto& layer : m_Layers) {
			layer->Draw();
		}
		for (auto& layer : m_Overlays) {
			layer->Draw();
		}
	}

	void Scene::OnEvent(Event& e)
	{
	}

	bool Scene::PushLayer(Layer* layer)
	{
		m_Layers.push_back(std::unique_ptr<Layer>(layer));
		layer->Activate();
		return true;
	}

	bool Scene::PushOverlay(Layer* layer)
	{
		m_Overlays.push_back(std::unique_ptr<Layer>(layer));
		layer->Activate();
		return false;
	}


}

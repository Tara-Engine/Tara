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
		//LOG_S(INFO) << "Scene: Layercount:" << m_Layers.size();
		for (auto& layer : m_Layers) {
			layer->Update(deltaTime);
			layer->RunOverlapChecks();
		}
		for (auto& layer : m_Overlays) {
			layer->Update(deltaTime);
			layer->RunOverlapChecks();
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
		//LOG_S(INFO) << "Scene::OnEvent!";
		for (auto& layer : m_Overlays) {
			layer->OnEvent(e);
		}
		for (auto& layer : m_Layers) {
			layer->OnEvent(e);
		}
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

	bool Scene::RemoveLayer(LayerRef layer)
	{
		auto result = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (result != m_Layers.end()) {
			(*result)->Deactivate();
			m_Layers.erase(result);
			return true;
		}
		result = std::find(m_Overlays.begin(), m_Overlays.end(), layer);
		if (result != m_Overlays.end()) {
			(*result)->Deactivate();
			m_Overlays.erase(result);
			return true;
		}
		return false;
	}


}

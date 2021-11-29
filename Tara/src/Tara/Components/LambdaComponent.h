#pragma once
#include "Tara/Core/Component.h"
#include <any>
#include <optional>

#define LAMBDA_BEGIN_PLAY_DEFAULT [](Tara::LambdaComponent*){}
#define LAMBDA_UPDATE_DEFAULT [](Tara::LambdaComponent*, float){}
#define LAMBDA_EVENT_DEFAULT [](Tara::LambdaComponent*, Tara::Event&){}

namespace Tara {
	REFTYPE(LambdaComponent);
	NOREFTYPE(LambdaComponent);



	class LambdaComponent : public Component {
	public:
		LambdaComponent(EntityNoRef parent,
			std::function<void(LambdaComponent*)> beginPlayCallback = LAMBDA_BEGIN_PLAY_DEFAULT,
			std::function<void(LambdaComponent*, float)> onUpdateCallback = LAMBDA_UPDATE_DEFAULT,
			std::function<void(LambdaComponent*, Event&)> onEventCallback = LAMBDA_EVENT_DEFAULT,
			const std::string& name = "LambdaComponent");
		
		virtual ~LambdaComponent() = default;

		/// <summary>
		/// Get a read/write reference to a custom param value by name. Does not automatically create the param.
		/// </summary>
		/// <typeparam name="T">any arbitrary type</typeparam>
		/// <param name="name">the name of the param</param>
		/// <returns>a read/write reference, wrapped in std::optional.</returns>
		template<typename T> T* Param(const std::string& name);
		
		/// <summary>
		/// Create a new param value from a name and an rvalue reference. (if from an existing param, use std::move)
		/// If a param of that name already exists, it will be overriden.
		/// </summary>
		/// <typeparam name="T">the param type</typeparam>
		/// <param name="name">the name of the param</param>
		/// <param name="value">the value</param>
		/// <returns>true if a new param was made, false if an old one was overriden.</returns>
		template<typename T> bool CreateParam(const std::string& name, T&& value);
		

	public:
		//overriden functions

		virtual void OnBeginPlay() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnEvent(Event& e) override;

	private:
		std::function<void(LambdaComponent*)> m_BeginPlayCallback;
		std::function<void(LambdaComponent*, float)> m_OnUpdateCallback;
		std::function<void(LambdaComponent*, Event&)> m_OnEventCallback;
		std::unordered_map<std::string, std::any> m_Params;
	};

	template<typename T>
	inline T* LambdaComponent::Param(const std::string& name)
	{
		auto result = m_Params.find(name);
		if (result != m_Params.end()) {
			return std::any_cast<T>(&(result->second));
		}
		//Param does not exist!
		return nullptr;
	}

	template<typename T>
	inline bool LambdaComponent::CreateParam(const std::string& name, T&& value)
	{
		auto result = m_Params.find(name);
		if (result != m_Params.end()) {
			//param already exists, just set and return false
			result->second = value;
			return false;
		}
		m_Params[name] = std::make_any<T>(value);
		return true;
	}

}
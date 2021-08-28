#pragma once
#include "tarapch.h"
//All mappings should be added when using this file
#include "Tara/Input/Mappings.h"
#include "Tara/Core/Window.h"

//TODO: add cursor mode changing (normal, disabled, hidden) 
//[https://www.glfw.org/docs/latest/input_guide.html#cursor-mode]

namespace Tara {

	//Active Input manager. Polling ONLY
	class Input {
	public:
		static Input* Get() {
			static std::unique_ptr<Input> input = Input::Create();
			return &(*input); //hacky way of getting non-owning raw pointer for unique pointer
		}
		inline static void Init(WindowRef ref) {
			Get()->Initialize(ref);
		}
		inline static void Update() {
			Get()->m_PrevMousePos = Get()->GetMousePos();
		}
		
		//singleton stuff
		Input(Input const&) = delete;
		void operator=(Input const&) = delete;

		virtual ~Input() {
			LOG_S(INFO) << "Input Object Destroyed!";
		}

		virtual bool IsKeyPressed(int32_t key) const = 0;
		virtual bool IsMouseDown(int32_t button) const = 0;
		//MousePos
		virtual std::pair<uint32_t, uint32_t>GetMousePos() const = 0;
		inline uint32_t GetMouseX() const { auto p = GetMousePos(); return p.first; }
		inline uint32_t GetMouseY() const { auto p = GetMousePos(); return p.second; }
		//PrevMousePos
		inline std::pair<uint32_t, uint32_t> GetPrevMousePos() const { return m_PrevMousePos; }
		inline uint32_t GetPrevMouseX() const { return m_PrevMousePos.first; }
		inline uint32_t GetPrevMouseY() const { return m_PrevMousePos.second; }
		//MouseDelta
		inline std::pair<uint32_t, uint32_t> GetMouseDelta() const { 
			auto p = GetMousePos(); 
			return std::pair<uint32_t, uint32_t>{
				p.first - m_PrevMousePos.first, 
				p.second - m_PrevMousePos.second
			}; 
		}
		inline uint32_t GetMouseDeltaX() const { auto p = GetMouseDelta(); return p.first; }
		inline uint32_t GetMouseDeltaY() const { auto p = GetMouseDelta(); return p.second; }

		//TODO: add gamepad stuff here. Eventually
	protected:
		virtual void Initialize(WindowRef ref) = 0;
		Input() {
			LOG_S(INFO) << "Input Object Created!";
		}
	
	private:
		static std::unique_ptr<Input> Create(); //implemented in platform-specific file
	
	private:
		std::pair<uint32_t, uint32_t> m_PrevMousePos;

		
	};

}
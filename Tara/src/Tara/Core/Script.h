#pragma once
#include "tarapch.h"
#define SOL_NO_EXCEPTIONS 1
#include <sol/sol.hpp>

#define CONNECT_FUNCTION(classname, funcname) type[#funcname] = &##classname##::##funcname

namespace Tara {

	REFTYPE(Component);
	REFTYPE(Entity);


	/// <summary>
	/// This class is intended to handle lua script related buisness, from registering types in lua to maintaining the lua state
	/// This class is a singleton, for obvious reasons.
	/// </summary>
	class Script {
	private:
		Script();

	public:
		/// <summary>
		/// Get a pointer to the Script manager
		/// </summary>
		/// <returns></returns>
		static Script* Get();
		//singleton stuff
		Script(Script const&) = delete;
		void operator=(Script const&) = delete;

		virtual ~Script() = default;

		/// <summary>
		/// Initialize the LuaScript component. Lua State already exists
		/// </summary>
		void Init();

		/// <summary>
		/// Get a reference to the lua state
		/// </summary>
		/// <returns></returns>
		inline sol::state& GetState() { return m_LuaState; }

		/// <summary>
		/// set the default path to lua libraries
		/// </summary>
		/// <param name="path"></param>
		inline void SetDefaultLibraryPath(const std::string& path) { m_DefaultLibPath = path; }

		/// <summary>
		/// Get the default path to lua libraries
		/// </summary>
		/// <returns></returns>
		inline const std::string& GetDefaultLibraryPath() { return m_DefaultLibPath; }

		/// <summary>
		/// Reload lua libraries
		/// </summary>
		void ReloadDefaultLibrary();

		/// <summary>
		/// Register a class, if you can. Otherwise, compile error!
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name=""></param>
		template<class T> static void RegisterType(const std::string& name);


		template<class T> static std::shared_ptr<T> CastComponent(ComponentRef ref);
		template<class T> static std::shared_ptr<T> CastEntity(EntityRef ref);

	private:
		sol::state m_LuaState;
		std::string m_DefaultLibPath;
	};






	template<class T>
	inline void Script::RegisterType(const std::string& name)
	{
		T::RegisterLuaType(Get()->GetState());
		//type["is"] = &is_check;
		LOG_S(INFO) << "Lua casting func: " << ("CastTo" + name);
		if constexpr (std::is_base_of<Component, T>::value) {
			Script::Get()->GetState()["CastTo"+name] = &Script::CastComponent<T>;
		}
		if constexpr(std::is_base_of<Entity, T>::value) {
			Script::Get()->GetState()["CastTo" + name] = &Script::CastEntity<T>;
		}

	}



	template<class T> 
	inline std::shared_ptr<T> Script::CastComponent(ComponentRef ref) {
		static_assert(std::is_base_of<Component, T>::value, "Error: Tara::Script::CastComponent:: Provided class is not a subclass of Tara::Component");
		LOG_S(INFO) << "C++ performing cast from ComponentRef to some subclass";
		auto c = std::dynamic_pointer_cast<T>(ref);
		if (c) {
			LOG_S(INFO) << "it succeeded";
			return c;
		}
		else {
			LOG_S(INFO) << "it failed";
			return nullptr;
		}
	}

	template<class T>
	inline std::shared_ptr<T> Script::CastEntity(EntityRef ref) {
		static_assert(std::is_base_of<Entity, T>::value, "Error: Tara::Script::CastEntity:: Provided class is not a subclass of Tara::Entity");
		auto c = std::dynamic_pointer_cast<T>(ref);
		if (c) {
			return c;
		}
		else {
			return nullptr;
		}
	}
}
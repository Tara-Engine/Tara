#pragma once
#include "tarapch.h"

#define NEW_TABLE sol::table(Tara::Script::Get()->GetState(), sol::create)
#define CONNECT_METHOD(classname, funcname) type[#funcname] = &classname::funcname
#define CONNECT_METHOD_OVERRIDE(classname, funcname) type[#funcname] = &classname::__SCRIPT__##funcname
#define CONNECT_STATIC(classname, funcname) Tara::Script::Get()->GetState()[#classname][#funcname] = &classname::funcname
#define CONNECT_STATIC_OVERRIDE(classname, funcname) Tara::Script::Get()->GetState()[#classname][#funcname] = &classname::__SCRIPT__##funcname
#define CONNECT_FUNCTION(funcname) Tara::Script::Get()->GetState()[#funcname] = &funcname
#define CONNECT_FUNCTION_OVERRIDE(funcname) Tara::Script::Get()->GetState()[#funcname] = &__SCRIPT__##funcname

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
		/// DEBUG run a random script file
		/// </summary>
		/// <param name="script"></param>
		void DEBUG_RunScript(const std::string& script);

		/// <summary>
		/// Register a class, if you can. Otherwise, compile error!
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name=""></param>
		template<class T> static void RegisterType(const std::string& name);

		//casting strangeness
		template<class T> static std::shared_ptr<T> CastComponent(ComponentRef ref);
		template<class T> static ComponentRef UpCastComponent(std::shared_ptr<T> ref);
		
		template<class T> static std::shared_ptr<T> CastEntity(EntityRef ref);
		template<class T> static EntityRef UpCastEntity(std::shared_ptr<T> ref);

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
			Script::Get()->GetState()["UpCast"+name] = &Script::UpCastComponent<T>;
		}
		if constexpr(std::is_base_of<Entity, T>::value) {
			Script::Get()->GetState()["CastTo" + name] = &Script::CastEntity<T>;
			Script::Get()->GetState()["UpCast" + name] = &Script::UpCastEntity<T>;
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
	inline ComponentRef Script::UpCastComponent(std::shared_ptr<T> ref) {
		static_assert(std::is_base_of<Component, T>::value, "Error: Tara::Script::UpCastComponent:: Provided class is not a subclass of Tara::Component");
		LOG_S(INFO) << "C++ performing upcast from ComponentRef subclass to ComponentRef itself";
		return std::dynamic_pointer_cast<Component>(ref);
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

	template<class T>
	inline EntityRef Script::UpCastEntity(std::shared_ptr<T> ref) {
		static_assert(std::is_base_of<Entity, T>::value, "Error: Tara::Script::UpCastEntity:: Provided class is not a subclass of Tara::Entity");
		LOG_S(INFO) << "C++ performing upcast from ComponentRef subclass to ComponentRef itself";
		return std::dynamic_pointer_cast<Entity>(ref);
	}
}
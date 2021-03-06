#pragma once

#include <cstdarg>
#include <future>
#include "Tara/Core/Application.h"

namespace Tara {

	

	struct AfterCallable {
		float secondsRemaining;
		std::future<void> callable;
		AfterCallable(float s, std::future<void>&& callable)
			:secondsRemaining(s), callable(std::move(callable))
		{}
	};


	template <typename Callable, typename... Ts> void After(Callable&& callable, float seconds, Ts&&... args) {
		//va_list args;
		//va_start(args, seconds);
		//LOG_S(INFO) << "After being called!";
		std::future<void> future = std::async(std::launch::deferred, std::move(callable), std::forward<Ts>(args)...);
		AfterCallable*  c = new AfterCallable( seconds, std::move(future) );
		Application::Get()->After(c);
		//va_end(args);
	}

	inline void __SCRIPT__After(sol::protected_function func, float seconds) {
		//LOG_S(INFO) << "Script After called!";
		After([func]() {
			auto result = func();
			if (!result.valid()) {
				sol::error err = result;
				LOG_S(ERROR) << "Error in Lua Script: " << err.what();
				LOG_S(ERROR) << loguru::stacktrace().c_str();
			}
		}, seconds);
	}
}
#pragma once
#include "Tara/Renderer/RenderCommand.h"
//#include "Tara/Core/Base.h"
//#include "tarapch.h"

namespace Tara {

	REFTYPE(Bindable);

	/// <summary>
	/// The Bindable interface is a unifying interface for Renderer objects that can be bound
	/// </summary>
	class Bindable : public std::enable_shared_from_this<Bindable> {
	public:
		/// <summary>
		/// Bind it with no paramaters
		/// </summary>
		inline virtual void Bind() { Bind(0, 0); };

		/// <summary>
		/// Bind it with one paramater
		/// </summary>
		/// <param name="a"></param>
		inline virtual void Bind(int a) {Bind(a, 0);}

		/// <summary>
		/// Bind it  with two paramaters
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		inline virtual void Bind(int a, int b) { RenderCommand::Bind(shared_from_this(), a, b); }

		/// <summary>
		/// Implementation-specific binding. Called by RenderCommand. Never call manually.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		virtual void ImplBind(int a, int b) const = 0;

	};

}
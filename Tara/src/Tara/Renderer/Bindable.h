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
		/// Bind it  with two paramaters
		/// </summary>
		/// <param name="a">this paramater is used for the GPU (mainly with textures)</param>
		/// <param name="b">this paramater is used to select what to bind (ie, multiple textures in a RenderTarget)</param>
		inline virtual void Bind(int a = 0, int b = 0) { RenderCommand::Bind(shared_from_this(), a, b); }

		/// <summary>
		/// Implementation-specific binding. Called by RenderCommand. Never call manually.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		virtual void ImplBind(int a, int b) const = 0;

	};

}
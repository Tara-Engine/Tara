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
		inline virtual void Bind(int a = 0, int b = 0) { RenderCommand::Bind(shared_from_this(), true, a, b); m_LastBindPoint = std::make_pair(a, b); }

		/// <summary>
		/// Unbind
		/// </summary>
		inline virtual void Unbind() { RenderCommand::Bind(shared_from_this(), false, 0, 0); }

		/// <summary>
		/// Implementation-specific binding. Called by RenderCommand. Never call manually.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		virtual void ImplBind(int a, int b) const = 0;

		/// <summary>
		/// Unbind
		/// </summary>
		virtual void ImplUnbind() const = 0;

		/// <summary>
		/// Get the last binding set for the Texture. No guarantee it is still bound there.
		/// </summary>
		/// <returns></returns>
		inline virtual std::pair<int32_t, int32_t> GetLastBindPoint() const { return std::make_pair(m_LastBindPoint.first, m_LastBindPoint.second); };

	private:
		std::pair<int32_t, int32_t> m_LastBindPoint = std::make_pair(0,0);
	};

}
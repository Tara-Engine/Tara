#pragma once


#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Bindable.h"
#include "Tara/Renderer/Texture.h"

namespace Tara {
	REFTYPE(RenderTarget);


	/// <summary>
	/// RenderTarget are special Texture2D objects that can be rendered to.
	/// Create a Camera and give it a RenderTarget to do this!
	/// </summary>
	class RenderTarget : public Texture2D {
	public:
		enum class InternalType {
			INT8, INT16, INT32,
			UINT8, UINT16, UINT32,
			FLOAT
		};
	public:
		RenderTarget(const std::string& name) : Texture2D(name) {}

		/// <summary>
		/// Enable rendering to this framebuffer.
		/// </summary>
		/// <param name="render">true if you actually want to render to this RenderTarget, false if you want to stop rendering to it</param>
		void RenderTo(bool render);

		/// <summary>
		/// Unqueued enable rendering to this framebuffer.
		/// </summary>
		/// <param name="render"></param>
		virtual void ImplRenderTo(bool render) const = 0;

		/// <summary>
		/// Copy the depth from this RenderTarget to a different (or the default) RenderTarget.
		/// this *WILL* change which RenderTarget is currently bound and indeed will then bind the other RenderTarget.
		/// </summary>
		/// <param name="other">The rendertarget to copy to.</param>
		virtual void BlitDepthToOther(RenderTargetRef other) = 0;

		/// <summary>
		/// Set the new size of the RenderTarget
		/// </summary>
		/// <param name="width">the new width</param>
		/// <param name="height">the new height</param>
		virtual void SetSize(uint32_t width, uint32_t height) = 0;

		/// <summary>
		/// Get the number of individual texture layers in the rendertarget
		/// </summary>
		/// <returns></returns>
		virtual uint32_t GetTextureCount() = 0;

		/// <summary>
		/// Create a RenderTarget (full version)
		/// </summary>
		/// <param name="width">the initial height of the RenderTarget image</param>
		/// <param name="height">the initial width of the RenderTarget image</param>
		/// <param name="colorTargets">the number of textures there should be (ie, color outputs of shaders rendering to this)</param>
		/// <param name="type">the format of the textures. IE, the datatype for storing each component of each pixel</param>
		/// <param name="depthIsTexture">if true, the depth buffer is a texture, located at the end of the texture list</param>
		/// <param name="name">the name of the asset</param>
		/// <returns></returns>
		static RenderTargetRef Create(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, bool depthIsTexture, const std::string& name);


		/// <summary>
		/// Create a RenderTarget (Half version)
		/// </summary>
		/// <param name="width">the initial height of the RenderTarget image</param>
		/// <param name="height">the initial width of the RenderTarget image</param>
		/// <param name="name">the name of the asset</param>
		/// <returns>Reference to the new RenderTarget</returns>
		static RenderTargetRef Create(uint32_t width, uint32_t height, const std::string& name);
	};

}
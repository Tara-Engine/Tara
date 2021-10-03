#pragma once

#include "Tara/Asset/Asset.h"

namespace Tara {

	/// <summary>
	/// Texture Base Class. All textures are this class
	/// </summary>
	class Texture : public Asset {
	public:
		Texture(const std::string& name) : Asset(name){}
		virtual ~Texture() = default;
		/// <summary>
		/// Get the width of a texture
		/// </summary>
		/// <returns>the width in pixels</returns>
		virtual uint32_t GetWidth() const = 0;
		/// <summary>
		/// Get the height of a texture
		/// </summary>
		/// <returns>the height in pixles</returns>
		virtual uint32_t GetHeight() const = 0;
		/// <summary>
		/// Bind the texture for use
		/// </summary>
		/// <param slot="index">slot to use</param>
		virtual void Bind(int slot = 0) const = 0;
	};

	/// <summary>
	/// 2D Image Texture, as loaded from a file
	/// </summary>
	class Texture2D : public Texture {
		using Texture2DRef = std::shared_ptr<Texture2D>;
	public:
		Texture2D(const std::string& name) : Texture(name) {}
		virtual ~Texture2D() = default;

		/// <summary>
		/// Create a new Texture2D
		/// The name in the AssetLibrary is the texture path with all slashes replaced with periods.
		/// For example, "assets/textures/funny_rabbit.png" becomes "assets.textures.funny_rabbit.png"
		/// </summary>
		/// <param name="path">the path to the texture</param>
		/// <returns>Reference to the new texture</returns>
		static Texture2DRef Create(const std::string& path, const std::string& name = "");
	};

	/// <summary>
	/// Texture Base Class Reference. All textures are this class
	/// </summary>
	using TextureRef = std::shared_ptr<Texture>;
	/// <summary>
	/// Reference to a 2D Image Texture, as loaded from a file
	/// </summary>
	using Texture2DRef = std::shared_ptr<Texture2D>;
}
#pragma once

#include "Tara/Asset/Asset.h"

namespace Tara {

	REFTYPE(Textuyre)
	REFTYPE(Texture2D)

	/// <summary>
	/// Texture Base Class. All textures are this class
	/// </summary>
	class Texture : public Asset {
	public:
		/// <summary>
		/// Enum for texture filtering modes
		/// </summary>
		enum class Filtering : uint8_t {Nearest,Linear};
	
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

		/// <summary>
		/// Set the texture filtering mode
		/// </summary>
		/// <param name="filter">the new filtering mode</param>
		virtual void SetFiltering(Filtering filter) = 0;

	public:
		inline static void SetDefaultTextureFiltering(Filtering filter) { s_DefaultTextureFiltering = filter; }

	protected:
		static Filtering s_DefaultTextureFiltering;
	};

	/// <summary>
	/// 2D Image Texture, as loaded from a file
	/// </summary>
	class Texture2D : public Texture {
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

		/// <summary>
		/// Create a new Texture2D from a byte array. After this call, the byte array is safe to clean up.
		/// </summary>
		/// <param name="bytes">a pointer to the array of bytes</param>
		/// <param name="width">the width of the texture in pixels</param>
		/// <param name="height">the height of the texture in pixels</param>
		/// <param name="bytesPerPixel">the bytes per pixel, AND the number of channels. must be in the range [1,4]</param>
		/// <param name="name">the name of the asset</param>
		/// <returns>Reference to the new texture</returns>
		static Texture2DRef Create(const uint8_t* bytes, uint32_t width, uint32_t height, uint32_t bytesPerPixel, const std::string& name);
	};

}
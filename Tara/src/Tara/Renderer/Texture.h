#pragma once

#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Bindable.h"

namespace Tara {

	REFTYPE(Textuyre);
	REFTYPE(Texture2D);
	

	/// <summary>
	/// Texture Base Class. All textures are this class
	/// </summary>
	class Texture : public Asset, public Bindable {
	public:
		/// <summary>
		/// Enum for texture filtering modes
		/// </summary>
		enum class Filtering : uint8_t {Nearest,Linear};

		enum class Wrapping : uint8_t{Clamp, Border, Repeat, Mirror};
	
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
		/// Set the texture filtering mode
		/// </summary>
		/// <param name="filter">the new filtering mode</param>
		virtual void SetFiltering(Filtering filter) = 0;

		/// <summary>
		/// Set the texture wrapping mode
		/// </summary>
		/// <param name="wrap"></param>
		virtual void SetWrap(Wrapping wrap) = 0;

		/// <summary>
		/// Set the texture border color for Texture wrap mode Border
		/// </summary>
		/// <param name="color"></param>
		virtual void SetBorderColor(const glm::vec4& color) = 0;

		
	public:
		/// <summary>
		/// Set the default texture filtering mode for all newly created textures
		/// </summary>
		/// <param name="filter"></param>
		inline static void SetDefaultTextureFiltering(Filtering filter) { s_DefaultTextureFiltering = filter; }

		/// <summary>
		/// Set the default texture wrapping mode for all newly created textures
		/// </summary>
		/// <param name="wrap"></param>
		inline static void SetDefaultTextureWrapping(Wrapping wrap) { s_DefaultTextureWrapping = wrap; }

	protected:
		static Filtering s_DefaultTextureFiltering;
		static Wrapping s_DefaultTextureWrapping;
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
#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Math/Types.h"
#include "stb_rect_pack.h"
#include "stb_truetype.h"

namespace Tara {
	REFTYPE(Font)

	/// <summary>
	/// Font class, for loading true-type font files, and managing how characters are written
	/// </summary>
	class Font : public Asset {
	public:
		/// <summary>
		/// Constructor. Use Font::Create instead (same params)
		/// </summary>
		/// <param name="filePath">the path to the font</param>
		/// <param name="imageSize">the size of the image,in pixels, that the font is rasterized to. Should not exceed 4096. Defaults to 1024. Powers of 2 prefered.</param>
		/// <param name="characterHeightPx">the height of a single character, in pixels. the larger it is, the more detail, but the larger an image size needed. Defaults to 96</param>
		/// <param name="name">the asset name. Defaults to a name generated from the file path.</param>
		Font(const std::string& filePath, uint32_t imageSize, uint32_t characterHeightPx, const std::string& name);

		/// <summary>
		/// Get the texture being used by the font for rendering.
		/// </summary>
		/// <returns>the texture</returns>
		const Texture2DRef& GetTexture() const { return m_Texture; }

		/// <summary>
		/// Turn some text into transforms and UV data. The transforms describe quads based at the origin. This data is ideal for running into Renderer::Quad(...)
		/// </summary>
		/// <param name="text">the text to render</param>
		/// <param name="transforms">a reference to a pre-created vector. Automatically reized and filled. After return, holds Transform data</param>
		/// <param name="minUVs">a reference to a pre-created vector. Automatically resized and filled. After return, holds min UV data</param>
		/// <param name="maxUVs">a reference to a pre-created vector. Automatically resized and filled. After return, holds max UV data</param>
		void GetTextQuads(const std::string& text, std::vector<Transform>& transforms, std::vector<glm::vec2>& minUVs, std::vector<glm::vec2>& maxUVs);

		/// <summary>
		/// Get the number of spaces that a tab takes
		/// </summary>
		/// <returns></returns>
		inline float GetSpacesPerTab() const { return m_SpacesPerTab; }

		/// <summary>
		/// set the number of spaces a tab takes.
		/// </summary>
		/// <param name="spaces"></param>
		inline void SetSpacesPerTab(float spaces) { m_SpacesPerTab = spaces; }
	public:
		/// <summary>
		/// Create a new font from a file
		/// </summary>
		/// <param name="filePath">the path to the font</param>
		/// <param name="imageSize">the size of the image,in pixels, that the font is rasterized to. Should not exceed 4096. Defaults to 1024. Powers of 2 prefered.</param>
		/// <param name="characterHeightPx">the height of a single character, in pixels. the larger it is, the more detail, but the larger an image size needed. Defaults to 96</param>
		/// <param name="name">the asset name. Defaults to a name generated from the file path.</param>
		/// <returns>A reference to the new font</returns>
		static FontRef Create(const std::string& filePath, uint32_t imageSize = 128, uint32_t characterHeightPx = 96, const std::string& name = "");

	private:
		void LoadFromFile();

		static std::string FilterString(const std::string& instr);
	private:
		std::string m_Path;	//file path
		Texture2DRef m_Texture;	//the texture they are rasterized to
		stbtt_packedchar m_CharacterData[128]; //character data
		uint32_t m_ImageSize; //the chosen image size
		uint32_t m_CharacterHeightPx; //the chosen pixel size of a character
		float m_SpaceWidth;
		float m_SpacesPerTab;
	};


}
#include "tarapch.h"
#include "Tara/Asset/AssetLibrary.h"
#include "Font.h"
#include <cstdio>
#include <algorithm>

#include "Tara/Renderer/Renderer.h"

//no messages from fopen. (intentionally being used instead of fopen_s)
//well the messages are generated anyway
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

namespace Tara{

	Font::Font(const std::string& filePath, uint32_t imageSize, uint32_t characterHeightPx, const std::string& name)
		: Asset(name), m_Path(filePath), m_ImageSize(imageSize), m_CharacterHeightPx(characterHeightPx), m_SpaceWidth(0), m_SpacesPerTab(4)
	{
		LoadFromFile();
	}

	FontRef Font::Create(const std::string& filePath, uint32_t imageSize, uint32_t characterHeightPx, const std::string& name)
	{
		std::string lName = name;
		if (name == "") { lName = GetAssetNameFromPath(filePath); }
		auto ref = AssetLibrary::Get()->GetAssetIf<Font>(lName);
		if (ref == nullptr) {
			ref = std::make_shared<Font>(filePath, imageSize, characterHeightPx, lName);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}

	void Font::GetTextQuads(const std::string& text, std::vector<Transform>& transforms, std::vector<glm::vec2>& minUVs, std::vector<glm::vec2>& maxUVs)
	{
		//Filter string to apply \b and \r
		std::string modText(FilterString(text));

		//get basic data needed.
		const char* str = modText.c_str();
		float x = 0.0f, y = 0.0f;
		//resize all the vectors to fit.
		transforms.clear();
		minUVs.clear();
		maxUVs.clear();
		transforms.resize(modText.size());
		minUVs.resize(modText.size());
		maxUVs.resize(modText.size());
		int index = 0;
		int lineCount = 0;
		//Vector offset = { 0,0,0 };
		//for every character in string, get unit quad transform and its UVs.
		while (*str) {
			switch(*str){
			case '\n': {
				//newling
				y += m_CharacterHeightPx;
				x = 0.0f;
				lineCount++;
				break;
			}
			case '\v': {
				//Vertical Tab. Don't reset x
				y += m_CharacterHeightPx;
				lineCount++;
				break;
			}
			case '\t' : {
				x += (m_SpaceWidth * m_SpacesPerTab);
				break;
			}
			case ' ': {
				x += (m_SpaceWidth);
				break;
			}
			default: {
				//regular character
				//get the quad data for this character
				stbtt_aligned_quad q;
				stbtt_GetPackedQuad(m_CharacterData, m_ImageSize, m_ImageSize, *str, &x, &y, &q, 1); //last param shoudl be 1 for OpenGl and Direct3d 10+. 0 for Direct3D 9-
				Transform t = TRANSFORM_2D(q.x0, q.y0, 0, q.x1 - q.x0, q.y1 - q.y0);
				t.Position /= (int)m_CharacterHeightPx;//scale to make everything in the relm of 0 to 1, since it was in pixel coordinates
				t.Scale /= (int)m_CharacterHeightPx;	  //
				t.Scale.y *= -1;				   //invert the Y, so its not upside down
				t.Position.y = (1.0f - t.Position.y) - 1; //flip Y coordinate, 
				//set into vectors
				transforms[index] = t;
				minUVs[index] = { q.s0, q.t0 };
				maxUVs[index] = { q.s1, q.t1 };
				index++;
			}
			}
			str++;
		}

		//vertical adjust all lines
		for (auto& tf : transforms) {
			tf.Position.y += lineCount;
		}
	}



	void Font::LoadFromFile()
	{
		long size;
		uint8_t* fileBuffer;
		uint8_t* pixels;

		//read file size. Using fopen_s somehow breaks image loading. DO NOT USE fopen_s.
		FILE* fontFile = fopen(m_Path.c_str(), "rb");
		fseek(fontFile, 0, SEEK_END);
		size = ftell(fontFile);
		fseek(fontFile, 0, SEEK_SET);
		//create buffer and fill
		fileBuffer = (uint8_t*)malloc(size); //yay! raw malloc in c++!
		if (!fileBuffer) { LOG_S(ERROR) << "Failed to read file due to memory limitations!"; return; }
		fread(fileBuffer, size, 1, fontFile); 
		fclose(fontFile);

		//create pizels array
		pixels = (uint8_t*)malloc((long)m_ImageSize * (long)m_ImageSize);

		//begin pack
		stbtt_pack_context packContext;
		stbtt_PackBegin(&packContext, pixels, m_ImageSize, m_ImageSize, 0, 1, nullptr);
		//set the oversampling information
		//we want nice looking fonts!
		stbtt_PackSetOversampling(&packContext, 2, 2);
		//pack stuff!
		stbtt_PackFontRange(&packContext, fileBuffer, 0, (float)m_CharacterHeightPx, 0, 127, m_CharacterData);
		stbtt_PackEnd(&packContext);
		
		//free file buffer, no longer needed
		free(fileBuffer);

		//make an image
		m_Texture =  Texture2D::Create(pixels, m_ImageSize, m_ImageSize, 1, GetAssetName() + "_Texture");
		
		//get space width
		{
			float y = 0.0f;
			stbtt_aligned_quad q;
			stbtt_GetPackedQuad(m_CharacterData, m_ImageSize, m_ImageSize, ' ', &m_SpaceWidth, &y, &q, 1);
		}

		//free the pixels
		free(pixels);
	}


	std::string Font::FilterString(const std::string& instr)
	{
		std::string outstr = instr;
		size_t startPos = 0;
		size_t newlinePos = -1;
		while ((startPos = outstr.find("\b", startPos)) != std::string::npos) {
			if (outstr[startPos - 1] != '\n') {
				outstr.replace(startPos - 1, 2, "");
				startPos--;
			}
			else {
				outstr.replace(startPos, 1, "");
			}
		}
		for (size_t i = 0; i < outstr.size(); i++) {
			if (outstr[i] == '\n') {
				newlinePos = i;
			}
			if (outstr[i] == '\r') {
				size_t len = i - (newlinePos);
				outstr.replace(newlinePos + 1, len, "");
				i -= len;
			}
		}

		return outstr;
	}
}
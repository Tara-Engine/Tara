#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Texture.h"

namespace Tara {
	REFTYPE(Patch);

	/// <summary>
	/// A 9-Patch class. Basically, it is a single image 
	/// broken into 9 segments, so that it can be scaled 
	/// without the corners or edges stretching in the wrong way.
	/// </summary>
	class Patch : public Asset {
	public:
		/// <summary>
		/// Create a new Patch. Should use Patch::Create() instead
		/// </summary>
		/// <param name="texture">the texture to base it off of</param>
		/// <param name="name">the asset name</param>
		Patch(const Texture2DRef& texture, const std::string& name);

		/// <summary>
		/// Create a new Patch
		/// </summary>
		/// <param name="texture">the texture to base it off of</param>
		/// <param name="name">the asset name</param>
		/// <returns>Reference to the newly created patch</returns>
		static PatchRef Create(const Texture2DRef& texture, const std::string& name);

		virtual ~Patch() = default;

		/// <summary>
		/// Set the border of the non-stretching bits as a percentage (0 to 1). Each direction is measured from that edge.
		/// </summary>
		/// <param name="left">the left percent, measured from left edge</param>
		/// <param name="right">the right percent, measured from right edge</param>
		/// <param name="top">the top percent, measured from top edge</param>
		/// <param name="bottom">the bottom percent, measured from bottom edge</param>
		void SetBorderPercent(float left, float right, float top, float bottom);

		/// <summary>
		/// Set the border of the non-stretching bits by pixel counts from that border, 
		/// relative to current Texture2D. If a new Texture2D is used, the pixels will
		/// not be maintained, but the percent will be. (ie, setting a border of 1 px on a
		/// 16x16 texture, then changing to a 32x32 texture, the border will become 2 pixels)
		/// </summary>
		/// <param name="left">number of pixels from left border</param>
		/// <param name="right">number of pixels from right border</param>
		/// <param name="top">number of pixels from top border</param>
		/// <param name="bottom">number of pixels from bottom border</param>
		void SetBorderPixels(int32_t left, int32_t right, int32_t top, int32_t bottom);

		/// <summary>
		/// Get the current border percent.
		/// </summary>
		/// <returns>tuple of [left, right, top, bottom]</returns>
		std::tuple<float, float, float, float> GetBorderPercent() const;

		/// <summary>
		/// Get the current border pixels
		/// </summary>
		/// <returns>tuple of [left, right, top, bottom]</returns>
		std::tuple<int32_t, int32_t, int32_t, int32_t> GetBorderPixels() const;

		/// <summary>
		/// Get the UVs for the middle square in the 9-patch
		/// </summary>
		/// <returns>the minimum and maximum UVs normalized</returns>
		std::pair<glm::vec2, glm::vec2> GetMiddleUVs() const;

		/// <summary>
		/// Given a specific scale, the the min and max offsets from the origin that 
		/// the middle square's corners should be.
		/// Units are assumed to be pixels.
		/// </summary>
		/// <param name="scale">the scale in pixels</param>
		/// <returns>the minimum and maximum offsets in pixels</returns>
		std::pair<glm::vec2, glm::vec2> GetMiddleOffsets(glm::vec2 scale) const;

		/// <summary>
		/// Get the internal texture reference
		/// </summary>
		/// <returns></returns>
		const Texture2DRef& GetTexture() const { return m_Texture; }

		/// <summary>
		/// Set a new internal texture reference
		/// </summary>
		/// <param name="texture"></param>
		void SetTexture(const Texture2DRef& texture) { m_Texture = texture; }

	private:
		Texture2DRef m_Texture;
		float m_BorderLeft;
		float m_BorderRight;
		float m_BorderTop;
		float m_BorderBottom;
	};
}
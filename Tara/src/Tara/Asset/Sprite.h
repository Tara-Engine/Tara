#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Texture.h"

namespace Tara {

	/// <summary>
	/// A sprite class. It stores all the frame and animation information for a spritesheet.
	/// 
	/// </summary>
	class Sprite : public Asset {
		using SpriteRef = std::shared_ptr<Sprite>;
	public: 
		/// <summary>
		/// A struct to hold the information of a specific animation sequence:
		///   Start - the first frame index
		///   End - the last frame index
		///   FrameRate - the target frames per second
		/// Both Start and End are inclusive.
		/// </summary>
		struct AnimationSequence {
			/// <summary>
			/// The first frame, inclusive
			/// </summary>
			uint32_t Start;
			/// <summary>
			/// The last frame, inclusive
			/// </summary>
			uint32_t End;
			/// <summary>
			/// The inverse frames per second (inverse, so, time each frame takes)
			/// </summary>
			float IFrameRate;

			AnimationSequence()
				: Start(0), End(0), IFrameRate(1.0f)
			{}

			/// <summary>
			/// Construct an Animation sequence
			/// </summary>
			/// <param name="start">the first frame, inclusive</param>
			/// <param name="end">the last frame, inclusive</param>
			/// <param name="frameRate">the frames per second</param>
			AnimationSequence(uint32_t start, uint32_t end, float frameRate)
				: Start(start), End(end), IFrameRate(1/frameRate)
			{}

			/// <summary>
			/// Get the legth, in frames, of the animation
			/// </summary>
			/// <returns>the length</returns>
			inline uint32_t Length() { return End - Start + 1; }
		};
	
	public:
		
		/// <summary>
		/// Construct a new Sprite. Should not be called manually
		/// </summary>
		/// <param name="texture">the texture</param>
		/// <param name="xFrameCount">the horizontal frame count</param>
		/// <param name="yFrameCount">the vertical frame count</param>
		/// <param name="name">the asset name</param>
		Sprite(const Texture2DRef& texture, uint32_t xFrameCount, uint32_t yFrameCount, const std::string& name);
		
		/// <summary>
		/// Create a new Sprite.
		/// </summary>
		/// <param name="texture">the texture</param>
		/// <param name="xFrameCount">the horizontal frame count</param>
		/// <param name="yFrameCount">the vertical frame count</param>
		/// <param name="name">the asset name</param>
		/// <returns></returns>
		static SpriteRef Create(const Texture2DRef& texture, uint32_t xFrameCount, uint32_t yFrameCount, const std::string& name);

		virtual ~Sprite() {}

		/// <summary>
		/// Create a new animation sequence
		/// </summary>
		/// <param name="name">the sequence name</param>
		/// <param name="start">the start frame, inclusive</param>
		/// <param name="end">the end frame, inclusive</param>
		/// <param name="frameRate">the frames per second</param>
		/// <returns>true on success, false otherwise</returns>
		bool CreateAnimationSequence(const std::string& name, uint32_t start, uint32_t end, float frameRate);
		
		/// <summary>
		/// Create a new animation sequence
		/// </summary>
		/// <param name="name">the sequence name</param>
		/// <param name="sequence">the sequence</param>
		/// <returns>true on success, false otherwise</returns>
		bool CreateAnimationSequence(const std::string& name, const AnimationSequence& sequence);

		/// <summary>
		/// Get an animation sequence by name, if available.
		/// Returns a const pointer to that sequence in storage.
		/// </summary>
		/// <param name="name">the name to check for</param>
		/// <returns>a pointer to the sequence, or nullptr if not available</returns>
		const AnimationSequence* GetAnimationSequence(const std::string& name) const;

		/// <summary>
		/// Get the UV coordinates for a specific frame
		/// </summary>
		/// <param name="frame">the frame index</param>
		/// <returns>a pair of vec2 UV coordinates, (min, max)</returns>
		std::pair<glm::vec2, glm::vec2> GetUVsForFrame(uint32_t frame) const;

		/// <summary>
		/// Get the normalized size of a single frame
		/// </summary>
		/// <returns>a vec2 of the frame size in UV coordinates</returns>
		inline glm::vec2 GetFrameSizeUV() const { return glm::vec2(1.0f / (float)m_XFrameCount, 1.0f / (float)m_YFrameCount); }

		/// <summary>
		/// Get the size, in pixels, of a single frame
		/// </summary>
		/// <returns>a vec2 of the frame size in pixel count</returns>
		inline glm::vec2 GetFrameSizePixel() const { glm::vec2((float)m_Texture->GetWidth() / (float)m_XFrameCount, (float)m_Texture->GetHeight() / m_YFrameCount); }

		/// <summary>
		/// Get a reference to the internal texture
		/// </summary>
		/// <returns></returns>
		inline  const Texture2DRef& GetTexture() const { return m_Texture; }

		/// <summary>
		/// Get the last available frame
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetLastFrame() const { return m_XFrameCount * m_YFrameCount - 1; }

		/// <summary>
		/// Get the frame count
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetFrameCount() const { return m_XFrameCount * m_YFrameCount; }

		/// <summary>
		/// Check if a particular animation sequence is valid for this sprite.
		/// </summary>
		/// <param name="seq"></param>
		/// <returns></returns>
		inline bool ValidSequence(const AnimationSequence& seq) { return (seq.Start <= seq.End && seq.IFrameRate >= 0 && seq.Start >= 0 && seq.End < GetFrameCount()); }

	private:
		Texture2DRef m_Texture;
		uint32_t m_XFrameCount;
		uint32_t m_YFrameCount;
		std::unordered_map<std::string, AnimationSequence> m_Sequences;
	};

	using SpriteRef = std::shared_ptr<Sprite>;
}
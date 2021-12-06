#pragma once
#include "Tara/Math/Types.h"

namespace Tara {


	struct UIBox {
		float x1, y1, x2, y2;

		/// <summary>
		/// Construct a UI box from its bounding points
		/// </summary>
		/// <param name="x1">the x location of the first point</param>
		/// <param name="y1">the y location of the first point</param>
		/// <param name="x2">the x location of the second point</param>
		/// <param name="y2">the y location of the second point</param>
		UIBox(float x1, float y1, float x2, float y2)
			: x1(x1), y1(y1), x2(x2), y2(y2)
		{Rectify(); }

		/// <summary>
		/// Construct a UI box from its bounding points
		/// </summary>
		/// <param name="pos1">the first point</param>
		/// <param name="pos2">the second point</param>
		UIBox(glm::vec2 pos1, glm::vec2 pos2)
			:x1(pos1.x), y1(pos1.y), x2(pos2.x), y2(pos2.y)
		{Rectify(); }

		/// <summary>
		/// Construct a UI box from its bounding points
		/// </summary>
		/// <param name="pos1">the first point</param>
		/// <param name="pos2">the second point</param>
		UIBox(Vector pos1, Vector pos2)
			:x1(pos1.x), y1(pos1.y), x2(pos2.x), y2(pos2.y)
		{Rectify();}


		/// <summary>
		/// Get the UIBox height
		/// </summary>
		/// <returns></returns>
		inline float Width() const { return x2 - x1; }
		
		/// <summary>
		/// Get the UIBox width
		/// </summary>
		/// <returns></returns>
		inline float Height() const { return y2 - y1; }
		
		/// <summary>
		/// Get the dimentions of a UIBox
		/// </summary>
		/// <returns></returns>
		inline glm::vec2 Dimentions() const { return glm::vec2{x2-x1, y2-y1}; }

		/// <summary>
		/// Rectify the box, so that x1, y1 is less than x2, y2
		/// </summary>
		void Rectify();

		/// <summary>
		/// make a transform (for drawing) from the box
		/// </summary>
		/// <returns></returns>
		Transform ToTransform() const;

		/// <summary>
		/// Compress a UIBox and size information into a transform, to save memory
		/// </summary>
		/// <param name="box"></param>
		/// <param name="border"></param>
		/// <returns></returns>
		static Transform CompressBoxAndSize(const UIBox& box, const glm::vec2& size);

		/// <summary>
		/// Get the compressed UIBox and size from a compressed transform
		/// </summary>
		/// <param name="transform"></param>
		/// <returns></returns>
		static std::pair<UIBox, glm::vec2> DecompressBoxAndSize(const Transform& transform);
	};

}

inline std::ostream& operator<< (std::ostream& out, const Tara::UIBox& b)
{
	out << "{x1:" << b.x1 << ", y1:" << b.y1 << ", x2:" << b.x2 << ", y2:" << b.y2 << " (" << b.Dimentions() << ")}";
	return out;
}
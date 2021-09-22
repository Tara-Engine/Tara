#pragma once
#include "tarapch.h"
//#include <glm/glm.hpp>
#include "Tara/Math/Types.h"

namespace Tara {

	/// <summary>
	/// 3 dimentional axis-alligned bounding box
	/// </summary>
	union BoundingBox {
		struct {
			/// <summary>
			/// the origin of the box
			/// </summary>
			glm::vec3 Position;
			/// <summary>
			/// the size of the box
			/// </summary>
			glm::vec3 Extent;
		};
		struct {
			/// <summary>
			/// X coordinate of the box
			/// </summary>
			float x;
			/// <summary>
			/// Y coordinate of the box
			/// </summary>
			float y;
			/// <summary>
			/// Z coordinate of the box
			/// </summary>
			float z;
			/// <summary>
			/// Width of the box (X)
			/// </summary>
			float Width;
			/// <summary>
			/// Height of the box (Y)
			/// </summary>
			float Height;
			/// <summary>
			/// Depth of the box (Z)
			/// </summary>
			float Depth;
		};

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="src">the box to copy</param>
		BoundingBox(const BoundingBox& src)
			: Position(src.Position), Extent(src.Extent)
		{}

		/// <summary>
		/// Create a new BoundingBox from two vectors
		/// </summary>
		/// <param name="pos">the origin of the box</param>
		/// <param name="extent">the extent of the box</param>
		BoundingBox(Vector pos = { 0.0f,0.0f,0.0f }, Vector extent = {1.0f, 1.0f, 1.0f})
			: Position(pos), Extent(extent)
		{}

		/// <summary>
		/// Create a new BoundingBox from raw floats
		/// </summary>
		/// <param name="x">X coordinate of the box</param>
		/// <param name="y">Y coordinate of the box</param>
		/// <param name="z">Z coordinate of the box</param>
		/// <param name="width">Width of the box (X)</param>
		/// <param name="height">Height of the box (Y)</param>
		/// <param name="depth">Depth of the box (Z)</param>
		BoundingBox(float x, float y, float z, float width, float height, float depth)
			: Position(x, y, z), Extent(width, height, depth)
		{}

		/// <summary>
		/// Check if two BoundingBoxes are overlapping
		/// </summary>
		/// <param name="other">the other box</param>
		/// <returns>true if they are overlapping, false otherwise</returns>
		bool Overlaping(const BoundingBox& other) const;

		/// <summary>
		/// Fast check to if a BoundingBox is overlapping a sphere
		/// prone to inaccuracies near edges and points. 
		/// IE, it will return true on some occations when they are not overlapping
		/// </summary>
		/// <param name="origin">the origin of the sphere</param>
		/// <param name="radius">the radius of the sphere</param>
		/// <returns></returns>
		bool OverlappingSphere(const Vector origin, const float radius) const;

		/// <summary>
		/// Combine two bounding boxes. This returns a new box that encompases both.
		/// </summary>
		/// <param name="other">the other bounding box to add</param>
		/// <returns>the new bounding box that is larger than both</returns>
		BoundingBox operator+(const BoundingBox& other) const;

		/// <summary>
		/// Modify a bounding box by a transform.
		/// In essence, it takes the box, transforms it, and makes a new box around the old one.
		/// </summary>
		/// <param name="t"></param>
		/// <returns></returns>
		BoundingBox operator*(const Transform& t) const;

		/// <summary>
		/// Take a transform and apply it to a 1x1x1 bounding box to create a new bounding box
		/// Rotation is ignored
		/// </summary>
		/// <param name="transform">the transform to use</param>
		/// <returns>the new bounding box</returns>
		inline static BoundingBox FromTransform(const Transform& t) {
			return BoundingBox() * t;
		}
	};


}
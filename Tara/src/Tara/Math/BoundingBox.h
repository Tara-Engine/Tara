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
		/// Create a new BoundingBox from two vectors
		/// </summary>
		/// <param name="pos">the origin of the box</param>
		/// <param name="extent">the extent of the box</param>
		BoundingBox(Vector pos, Vector extent)
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
		inline bool Overlaping(const BoundingBox& other) const {
			return  !(
				(x + Width  < other.x || other.x + other.Width  < x) ||
				(y + Height < other.y || other.y + other.Height < y) ||
				(z + Depth  < other.z || other.z + other.Depth  < z)
			);
		}

		/// <summary>
		/// Fast check to if a BoundingBox is overlapping a sphere
		/// prone to inaccuracies near edges and points. 
		/// IE, it will return true on some occations when they are not overlapping
		/// </summary>
		/// <param name="origin">the origin of the sphere</param>
		/// <param name="radius">the radius of the sphere</param>
		/// <returns></returns>
		inline bool OverlappingSphere(const Vector origin, const float radius) {
			return (
				((x - radius < origin.x) && (x + Width  + radius > origin.x)) &&
				((y - radius < origin.y) && (y + Height + radius > origin.y)) &&
				((z - radius < origin.z) && (z + Depth  + radius > origin.z))
			);
		}

		/// <summary>
		/// Combine two bounding boxes. This returns a new box that encompases both.
		/// </summary>
		/// <param name="other">the other bounding box to add</param>
		/// <returns>the new bounding box that is larger than both</returns>
		inline BoundingBox operator+(const BoundingBox& other) {
			Vector minpos = {std::min(x, other.x),std::min(y, other.y), std::min(z, other.z)};
			Vector maxpos = { std::max(x + Width, other.x + other.Width), std::max(y + Height, other.y + other.Height) , std::max(z + Depth, other.z + other.Depth) };
			return BoundingBox(minpos, maxpos - minpos);
		}


	};


}
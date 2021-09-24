#include "tarapch.h"
#include "BoundingBox.h"

namespace Tara{

	

	bool BoundingBox::OverlappingSphere(const Vector origin, const float radius) const
	{
		//negative-size don't collide
		if (Width < 0 || Height < 0 || Depth < 0) { return false; }
		return (
			((x - radius < origin.x) && (x + Width + radius > origin.x)) &&
			((y - radius < origin.y) && (y + Height + radius > origin.y)) &&
			((z - radius < origin.z) && (z + Depth + radius > origin.z))
			);
	}

	BoundingBox BoundingBox::operator+(const BoundingBox& other) const
	{
		//negative-size don't combine
		if (Width < 0 || Height < 0 || Depth < 0) { return BoundingBox(other); }
		if (other.Width < 0 || other.Height < 0 || other.Depth < 0) { return BoundingBox(*this); }
		//combine
		Vector minpos = { std::min(x, other.x),std::min(y, other.y), std::min(z, other.z) };
		Vector maxpos = { std::max(x + Width, other.x + other.Width), std::max(y + Height, other.y + other.Height) , std::max(z + Depth, other.z + other.Depth) };
		return BoundingBox(minpos, maxpos - minpos);
	}

	BoundingBox BoundingBox::operator*(const Transform& t) const
	{
		//first, scale
		Vector newExtent = Extent * t.Scale;
		//then, rotate the extent, but do it component-wise (7 components)
		Vector extents[7] = {
			t.Rotation.RotateVector({newExtent.x,0,0}),
			t.Rotation.RotateVector({0,newExtent.y,0}),
			t.Rotation.RotateVector({0,0,newExtent.z}),

			t.Rotation.RotateVector({newExtent.x,newExtent.y,0}),
			t.Rotation.RotateVector({newExtent.x,0,newExtent.z}),
			t.Rotation.RotateVector({0,newExtent.y,newExtent.z}),

			t.Rotation.RotateVector(newExtent)
		};

		//then, recombine, and get a origin offset
		newExtent = { 0,0,0 };
		Vector newPosition = { 0,0,0 }; //functions as an offset till relocation of origin
		for (auto extent : extents) {
			if (extent.x > newExtent.x) { newExtent.x = extent.x; }
			if (extent.y > newExtent.y) { newExtent.y = extent.y; }
			if (extent.z > newExtent.z) { newExtent.z = extent.z; }

			if (extent.x < newPosition.x) { newPosition.x = extent.x; }
			if (extent.y < newPosition.y) { newPosition.y = extent.y; }
			if (extent.z < newPosition.z) { newPosition.z = extent.z; }
		}
		newExtent = newExtent - newPosition;

		//then, relocate origin
		newPosition = newPosition + Position + t.Position;
		//ensure no negativity
		//if (newExtent.x < 0) { newPosition.x += newExtent.x; newExtent.x *= -1; }
		//if (newExtent.y < 0) { newPosition.y += newExtent.y; newExtent.y *= -1; }
		//if (newExtent.z < 0) { newPosition.z += newExtent.z; newExtent.z *= -1; }
		return BoundingBox(newPosition, newExtent);
	}

	bool BoundingBox::operator==(const BoundingBox& other) const
	{
		return (Position == other.Position && Extent == other.Extent);
	}

	bool BoundingBox::operator<(const BoundingBox& other) const
	{
		return (x * x + y * y + z * z < other.x* other.x + other.y * other.y + other.z * other.z);
	}

}
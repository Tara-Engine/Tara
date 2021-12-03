#include "tarapch.h"
#include "UIBox.h"

namespace Tara{

	void UIBox::Rectify()
	{
		float t;
		if (x2 < x1) {
			t = x2;
			x2 = x1;
			x1 = t;
		}
		if (y2 < y1) {
			t = y2;
			y2 = y1;
			y1 = t;
		}
	}

	Transform UIBox::ToTransform() const
	{
		return Transform(Vector(x1, y1,0), Rotator(0,0,0), Vector(Width(), Height(), 1));
	}

	Transform UIBox::CompressBoxAndSize(const UIBox& box, const glm::vec2& size)
	{
		return Transform(
			Vector(box.x1, box.y1, box.x2),
			Rotator(0,0,0),
			Vector(box.y2, size.x, size.y)
		);
	}

	std::pair<UIBox, glm::vec2> UIBox::DecompressBoxAndSize(const Transform& t)
	{
		return std::make_pair(
			UIBox(t.Position.x, t.Position.y, t.Position.z, t.Scale.x),
			glm::vec2(t.Scale.y, t.Scale.z)
		);
	}

}

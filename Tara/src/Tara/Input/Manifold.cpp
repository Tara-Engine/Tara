#include "tarapch.h"
#include "Manifold.h"

#include "Tara/Core/Entity.h"
#include "Tara/Math/BoundingBox.h"
#include "Tara/Input/ApplicationEvents.h"

namespace Tara {

	Manifold::Manifold(EntityRef a, EntityRef b)
		: A(a), B(b), Penetration(0), Normal(0,0,0)
	{
		//create the actual manifold data here
		//NOTE: Z-overlaps are currently commented out due to ... problems.

		//everything is calculated from these boxes.
		BoundingBox boxA = A->GetSpecificBoundingBox();
		BoundingBox boxB = B->GetSpecificBoundingBox();

		//Get the overlap
		Vector overlap = { 0,0,0 };

		if (boxA.x < boxB.x) {
			overlap.x = (boxA.x + boxA.Width) - boxB.x;
		}
		else {
			overlap.x = (boxB.x + boxB.Width) - boxA.x;
		}

		if (boxA.y < boxB.y) {
			overlap.y = (boxA.y + boxA.Height) - boxB.y;
		}
		else {
			overlap.y = (boxB.y + boxB.Height) - boxA.y;
		}

		/*
		if (boxA.z < boxB.z) {
			overlap.z = (boxA.z + boxA.Depth) - boxB.z;
		}
		else {
			overlap.z = (boxB.z + boxB.Depth) - boxA.z;
		}
		*/

		//get the normal
		if (abs(overlap.x) < abs(overlap.y) /* && abs(overlap.x) < abs(overlap.z)*/) {
			//normal is in X
			if (overlap.x > 0) {
				Normal = { 1,0,0 };
				Penetration = overlap.x;
			}
			else {
				Normal = { -1,0,0 };
				Penetration = -overlap.x;
			}
		}
		else /*if (abs(overlap.y) < abs(overlap.x) && abs(overlap.y) < abs(overlap.z))*/ {
			//normal is in Y
			if (overlap.y > 0) {
				Normal = { 0,1,0 };
				Penetration = overlap.y;
			}
			else {
				Normal = { 0,-1,0 };
				Penetration = -overlap.y;
			}
		}
		/*
		else {
			//normal is in Z
			if (overlap.z > 0) {
				Normal = { 0,0,1 };
				Penetration = overlap.z;
			}
			else {
				Normal = { 0,0,-1 };
				Penetration = -overlap.z;
			}
		}
		*/

	}
	
	
	void Manifold::Resolve()
	{
		//if the entities have physics bodies, and they are solid, this is where collision should take place.
		//meanwhile, it just generates the Event
		
		if (Penetration > 0) {
			OverlapEvent orig = OverlapEvent(*this);
			A->ReceiveEvent(orig);
			OverlapEvent other = OverlapEvent(Invert());
			B->ReceiveEvent(other);
		}
		else if (Penetration == 0) {
			//generate Touch Events
		}
		

	}
}

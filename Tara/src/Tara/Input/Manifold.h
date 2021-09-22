#pragma once
#include "Tara/Math/Types.h"

namespace Tara {

	class Entity;
	using EntityRef = std::shared_ptr<Entity>;

	/// <summary>
	/// A Manifold holds all the information about an overlap
	/// </summary>
	struct Manifold {
		
		/// <summary>
		/// The first entity in the manifold
		/// </summary>
		EntityRef A;
		/// <summary>
		/// The second entity in the manifold
		/// </summary>
		EntityRef B;
		/// <summary>
		/// The depth of the overlap
		/// </summary>
		float Penetration;
		/// <summary>
		/// The direction from Entity A to Entity B
		/// </summary>
		Vector Normal;

		/// <summary>
		/// Construct a new manifold for an overlap between two objects
		/// </summary>
		/// <param name="a">The first object</param>
		/// <param name="b">The second object</param>
		Manifold(EntityRef a, EntityRef b);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="m">the manifold being moved</param>
		Manifold(const Manifold&& m) noexcept
			:A(std::move(m.A)),B(std::move(m.B)),Penetration(m.Penetration),Normal(std::move(m.Normal))
		{}
		
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="m">the manifold being copied</param>
		Manifold(const Manifold& m)
			:A(m.A),B(m.B),Penetration(m.Penetration),Normal(m.Normal)
		{}

		/// <summary>
		/// Resolve a overlap via its manifold.
		/// </summary>
		void Resolve();

	private:
		/// <summary>
		/// Invert a manifold (ie, from (A to B) become (B to A)
		/// 
		/// </summary>
		/// <returns></returns>
		inline Manifold Invert() {
			return Manifold(B, A, Penetration, Normal * -1.0f);
		}

		
		/// <summary>
		/// private constructor
		/// </summary>
		/// <param name="a">first object</param>
		/// <param name="b">second object</param>
		/// <param name="penetration">penetration</param>
		/// <param name="normal">normal</param>
		Manifold(EntityRef a, EntityRef b, float penetration, Vector normal)
			: A(a), B(b), Penetration(penetration), Normal(normal)
		{}

	};

}
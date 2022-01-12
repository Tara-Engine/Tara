#include "tarapch.h"
#include "MeshPart.h"
#include "Tara/Utility/MeshMaker.h"

namespace Tara{

	MeshPart& MeshPart::CalculateNormals()
	{
		//first, get the usage count
		std::vector<int32_t> usageCount(Vertices.size(), 0);
		for (auto& index : Indices) {
			if (index < usageCount.size()) { //in case there is an index that is logically outside of the mesh. Could happen
				usageCount[index]+=1;
			}
		}

		//figgure out if each vertex needs its normal calculated or not. Any that is the zero-vector is marked as "needed"
		std::vector<bool> needsNormal(Vertices.size(), false); //should be space-efficent from stl
		for (int i = 0; i < Vertices.size();i++) {
			auto& vert = Vertices[i];
			if (vert.Normal == Vector{0, 0, 0}) {
				needsNormal[i] = true;
			}
		}

		//now, loop through each triangle, and calculate its normal. Apply those to the normals of the vertecies on its edges (if initially 0)
		for (int i = 0; i < Indices.size(); i += 3) {
			auto& va = Vertices[Indices[i]];
			auto& vb = Vertices[Indices[i+1]];
			auto& vc = Vertices[Indices[i+2]];
			Vector norm = Vector::Cross(vb.Position-va.Position, vc.Position -va.Position).Normalize();
			for (int j = i; j < i + 3; j++) {
				if (needsNormal[Indices[j]]) {
					//using -= here because the calculation used is for CouterClockwise winding, but Tara uses Clockwise winding as front face
					Vertices[Indices[j]].Normal -= (norm / (float)usageCount[Indices[j]]);
				}
			}
		}

		return *this;
	}

	MeshPart& MeshPart::FlipNormals()
	{
		for (auto& vert : Vertices) {
			vert.Normal *= -1;
		}
		return *this;
	}

	MeshPart& MeshPart::FlipWindings()
	{
		for (int i = 0; i < Indices.size(); i += 3) {
			//for every triangle
			std::swap(Indices[i + 1], Indices[i + 2]);
		}
		return *this;
	}

	MeshPart& MeshPart::ClearNormals()
	{
		for (auto& vert : Vertices) {
			vert.Normal = { 0,0,0 };
		}
		return *this;
	}

	MeshPart& MeshPart::Transform(glm::mat4 matrix)
	{
		for (auto& vert : Vertices) {
			vert.Position = Tara::Vector(matrix * (glm::vec4)vert.Position);
		}
		return *this;
	}

	MeshPart MeshPart::UnitCube()
	{
		return MeshPart(
			{ 
				{0,0,1,  0, 0, 1, 1,1,1,1, 0,0}, {1,0,1,  0, 0, 1, 1,1,1,1, 1,0}, {1,1,1,  0, 0, 1, 1,1,1,1, 1,1}, {0,1,1,  0, 0, 1, 1,1,1,1, 0,1}, //Front (+Z)
				{0,0,0,  0, 0,-1, 1,1,1,1, 1,0}, {1,0,0,  0, 0,-1, 1,1,1,1, 0,0}, {1,1,0,  0, 0,-1, 1,1,1,1, 0,1}, {0,1,0,  0, 0,-1, 1,1,1,1, 1,1}, //Back (-Z)
				{0,0,0, -1, 0, 0, 1,1,1,1, 0,0}, {0,1,0, -1, 0, 0, 1,1,1,1, 0,1}, {0,1,1, -1, 0, 0, 1,1,1,1, 1,1}, {0,0,1, -1, 0, 0, 1,1,1,1, 1,0}, //Left (-X)
				{1,0,0,  1, 0, 0, 1,1,1,1, 1,0}, {1,1,0,  1, 0, 0, 1,1,1,1, 1,1}, {1,1,1,  1, 0, 0, 1,1,1,1, 0,1}, {1,0,1,  1, 0, 0, 1,1,1,1, 0,0}, //Right (+X)
				{0,1,0,  0, 1, 0, 1,1,1,1, 0,1}, {1,1,0,  0, 1, 0, 1,1,1,1, 1,1}, {1,1,1,  0, 1, 0, 1,1,1,1, 1,0}, {0,1,1,  0, 1, 0, 1,1,1,1, 0,0}, //Top(+Y)
				{0,0,0,  0,-1, 0, 1,1,1,1, 0,0}, {1,0,0,  0,-1, 0, 1,1,1,1, 1,0}, {1,0,1,  0,-1, 0, 1,1,1,1, 1,1}, {0,0,1,  0,-1, 0, 1,1,1,1, 0,1}  //Bottom(-Y)
			}, 
			{
				0,2,1,2,0,3,		//Front (+Z)
				4,5,6,6,7,4,		//Back (-Z) 
				8,9,10,10,11,8,		//Left (-X) 
				12,14,13,14,12,15,	//Right (+X)
				16,17,18,18,19,16,	//Top(+Y) 
				20,22,21,22,20,23	//Bottom(-Y) 
			});
	}

	MeshPart MeshPart::UnitSphere(int32_t strips_v, int32_t strips_h)
	{
		MeshMaker mm(MeshMaker::Mode::TRIANGLE_FAN);
		mm.SetCombineRule(MeshMaker::CombineRules::IF_SECOND_DEFAULT);
		float theta_v = glm::radians(180.0f / strips_v);
		float theta_h = glm::radians(360.0f / strips_h);
		float angle_v = glm::radians(-180.0f);
		for (int v = 0; v < strips_v; v++) {
			float angle_h = glm::radians(-180.0f);

			if (v == 0) {
				//start triangle fan
				mm.Vertex(Vector{ 0,-0.5,0 });

				mm.Vertex(Vector{
						sinf(angle_h) * sinf(angle_v + theta_v) * 0.5,
						cosf(angle_v + theta_v) * 0.5,
						cosf(angle_h) * sinf(angle_v + theta_v) * 0.5
					});

			}
			else if (v == strips_v - 1) {
				mm.Vertex(Vector{ 0,0.5,0 });

				mm.Vertex(Vector{
						sinf(angle_h + theta_h) * sinf(angle_v) * 0.5,
						cosf(angle_v) * 0.5,
						cosf(angle_h + theta_h) * sinf(angle_v) * 0.5
					});

				angle_h *= -1;

			}

			for (int h = 0; h < strips_h; h++) {

				if (v == 0) {
					//Draw upside down triangle fan
					mm.Vertex(Vector{
						sinf(angle_h + theta_h) * sinf(angle_v + theta_v) * 0.5,
						cosf(angle_v + theta_v) * 0.5,
						cosf(angle_h + theta_h) * sinf(angle_v + theta_v) * 0.5
						});

				}
				else if (v == strips_v - 1) {


					mm.Vertex(Vector{
						sinf(angle_h) * sinf(angle_v) * 0.5,
						cosf(angle_v) * 0.5,
						cosf(angle_h) * sinf(angle_v) * 0.5
						});

					angle_h -= 2 * theta_h;
				}
				else {
					mm.Vertex(Vector{
						sinf(angle_h) * sinf(angle_v) * 0.5,
						cosf(angle_v) * 0.5,
						cosf(angle_h) * sinf(angle_v) * 0.5
						});

					mm.Vertex(Vector{
						sinf(angle_h) * sinf(angle_v + theta_v) * 0.5,
						cosf(angle_v + theta_v) * 0.5,
						cosf(angle_h) * sinf(angle_v + theta_v) * 0.5
						});

					mm.Vertex(Vector{
						sinf(angle_h + theta_h) * sinf(angle_v + theta_v) * 0.5,
						cosf(angle_v + theta_v) * 0.5,
						cosf(angle_h + theta_h) * sinf(angle_v + theta_v) * 0.5
						});

					mm.Vertex(Vector{
						sinf(angle_h + theta_h) * sinf(angle_v) * 0.5,
						cosf(angle_v) * 0.5,
						cosf(angle_h + theta_h) * sinf(angle_v) * 0.5
						});
				}

				angle_h += theta_h;
			}
			if (v == 0) {
				mm.PushPrimitive();
				mm.SetMode(MeshMaker::Mode::QUADS);
			}
			else if (v == strips_v - 2) {
				mm.PushPrimitive();
				mm.SetMode(MeshMaker::Mode::TRIANGLE_FAN);
			}
			angle_v += theta_v;
		}
		Tara::MeshPart spherePart = mm.GetMeshPart();
		spherePart.CalculateNormals();
		return spherePart;
	}

	VertexArrayRef MeshPart::ToVertexArray() const
	{
		auto vertexArray = VertexArray::Create();

		auto vertexBuffer = VertexBuffer::Create((const float*)Vertices.data(), Vertices.size() * VERTEX_FLOATS);
		vertexBuffer->SetLayout(VERTEX_LAYOUT);
		vertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = IndexBuffer::Create(Indices.data(), Indices.size());
		vertexArray->SetIndexBuffer(indexBuffer);

		return vertexArray;
	}

}

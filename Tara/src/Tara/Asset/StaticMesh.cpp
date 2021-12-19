#include "tarapch.h"
#include "StaticMesh.h"
#include "Tara/Asset/AssetLibrary.h"

namespace Tara{
	StaticMesh::StaticMesh(const std::initializer_list<MeshPart>& parts, const std::string& name)
		: Asset(name)
	{
		m_VertexArrays.reserve(parts.size()); //guarantee only one memory operation
		for (auto& part : parts) {
			m_VertexArrays.push_back(part.ToVertexArray());
		}
	}

	StaticMesh::StaticMesh(const std::initializer_list<VertexArrayRef>& parts, const std::string& name)
		: Asset(name)
	{
		m_VertexArrays = parts;
	}

	StaticMeshRef StaticMesh::Create(const std::initializer_list<MeshPart>& parts, const std::string& name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<StaticMesh>(name);
		if (ref == nullptr) {
			ref = std::make_shared<StaticMesh>(parts, name);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}

	StaticMeshRef StaticMesh::Create(const std::initializer_list<VertexArrayRef>& parts, const std::string& name)
	{
		auto ref = AssetLibrary::Get()->GetAssetIf<StaticMesh>(name);
		if (ref == nullptr) {
			ref = std::make_shared<StaticMesh>(parts, name);
			AssetLibrary::Get()->RegisterAsset(ref);
		}
		return ref;
	}
}

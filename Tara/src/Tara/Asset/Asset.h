#pragma once
#include "tarapch.h"
namespace Tara {

	class Asset {
	public:
		Asset(const std::string& name)
			: m_Name(name) {}
		virtual ~Asset() {}
		
		const inline std::string& GetAssetName() { return m_Name; }

	private:
		const std::string m_Name;
	};

	using AssetRef = std::shared_ptr<Asset>;
}
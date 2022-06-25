#pragma once
#include "tarapch.h"
namespace Tara {

	class Asset {
	public:
		Asset(const std::string& name)
			: m_Name(name) 
		{
			LOG_S(8) << "Creating asset with name: " << name;
		}
		virtual ~Asset() {}
		
		const inline std::string& GetAssetName() { return m_Name; }

		virtual inline bool Save(const std::string& filePath) { LOG_S(WARNING) << "Unable to save asset: " << m_Name; return false; }

	private:
		const std::string m_Name;
	};

	using AssetRef = std::shared_ptr<Asset>;
}
#pragma once
#include "tarapch.h"
#include "Asset.h"
namespace Tara {

	/// <summary>
	/// AssetLibrary should hold a copy of every asset the engine manufactures, 
	/// under its unique name, for lookup and retrieval later
	/// </summary>
	class AssetLibrary {
	private:
		/// <summary>
		/// Asset Library Private Constructor
		/// </summary>
		AssetLibrary();

	public:
		/// <summary>
		/// Get the singleton AssetRef
		/// </summary>
		/// <returns>A pointer to the singleton</returns>
		static AssetLibrary* Get();
		
		/// <summary>
		/// Get the named asset, if available. Returns nullptr if not available.
		/// </summary>
		/// <param name="name">the name of the asset to get</param>
		/// <returns>the asset, as AssetRef, or nullptr</returns>
		AssetRef GetAsset(const std::string& name);

		/// <summary>
		/// Get the named asset, if it is both available and of the correct type. Returns nullptr otherwise.
		/// </summary>
		/// <typeparam name="t">Asset subclass. Not a ref type.</typeparam>
		/// <param name="name">the name of the asset to get</param>
		/// <returns>the asset, as SuppliedTypeNameRef, opr nullptr</returns>
		template <typename T> std::shared_ptr<T> GetAssetIf(const std::string& name);

		/// <summary>
		/// registers an asset to the library, under its builtin name.
		/// </summary>
		/// <param name="asset">The asset to register</param>
		/// <returns>boolean true if the asset is added. False if there was a collision.</returns>
		bool RegisterAsset(AssetRef asset);
		
		/// <summary>
		/// Get the next ID in an ID sequence, to help name assets uniquely.
		/// </summary>
		/// <returns></returns>
		std::string GetNextId();

		//singleton stuff
		AssetLibrary(AssetLibrary const&) = delete;
		void operator=(AssetLibrary const&) = delete;


	private:
		/// <summary>
		/// The map of all the assets
		/// </summary>
		std::unordered_map<std::string, AssetRef> m_Assets;

		/// <summary>
		/// the next id, as an uint64_t
		/// </summary>
		uint64_t m_NextID;
	};

	

	template<typename T>
	inline std::shared_ptr<T> AssetLibrary::GetAssetIf(const std::string& name)
	{
		//LOG_S(INFO) << "AssetLib::GetAssetIf(...) called";
		auto ast = m_Assets.find(name);
		if (ast != m_Assets.end()) {
			std::shared_ptr<T> ref = std::dynamic_pointer_cast<T>(ast->second);
			if (ref) {
				return ref;
			}
			else {
				return nullptr;
			}
		}
		else {
			return nullptr;
		}
	}

	/// <summary>
	///  Take a file path and turn it into a unique name for an asset.
	/// </summary>
	/// <param name="path">the path to take in</param>
	/// <returns>string ready for use as asset</returns>
	std::string GetAssetNameFromPath(const std::string& path);

}
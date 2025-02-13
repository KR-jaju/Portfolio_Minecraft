#pragma once

#include <unordered_map>
#include <memory>
#include "Asset.h"
#include "Graphics.h"

class AssetManager
{
public:
    AssetManager(Graphics& graphics);
	template <typename T>
	std::shared_ptr<T> load(std::wstring const& path)
	{
        std::shared_ptr<T> resource = nullptr;
        auto it = this->resources.find(path);

        if (it != this->resources.end())
        {
            resource = std::move(std::dynamic_pointer_cast<T>(it->second));
            if (resource == nullptr)
                throw std::runtime_error("Resource type mismatch for path");
            return resource;
        }
        constexpr AssetType type = T::getAssetType();
        if constexpr (type == AssetType::Texture2D ||
            type == AssetType::TextureArray ||
            type == AssetType::Cubemap)
        {
            resource = std::make_shared<T>(graphics, path);
        }
        else if constexpr (T::getAssetType() == AssetType::AnimationClip ||
            T::getAssetType() == AssetType::SkinnedMesh ||
            T::getAssetType() == AssetType::Blob)
        {
            resource = std::make_shared<T>(path);
        }
        else
        {
            //static_assert(_)
        }
        this->resources[path] = resource;
        return std::move(resource);
	}
    void    unload(std::wstring const& path);
private:
    Graphics& graphics;
	std::unordered_map<std::wstring, std::shared_ptr<Asset>> resources;
};

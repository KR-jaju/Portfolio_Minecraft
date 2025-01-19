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
	std::shared_ptr<T> load(std::string const& path)
	{
        auto it = this->resources.find(path);

        if (it != this->resources.end()) {
            std::shared_ptr<T> const& casted = std::dynamic_pointer_cast<T>(it->second);
            if (!casted)
                throw std::runtime_error("Resource type mismatch for path: " + path);
            return casted;
        }
        std::shared_ptr<T> resource = std::make_shared<T>(path);
        this->resources[path] = resource;
        return resource;
	}
    void    unload(std::string const& path);
private:
    Graphics& graphics;
	std::unordered_map<std::string, std::shared_ptr<Asset>> resources;
};

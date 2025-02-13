#include "pch.h"
#include "AssetManager.h"

AssetManager::AssetManager(Graphics& graphics) : graphics(graphics) {}

void	AssetManager::unload(std::wstring const& path)
{
    this->resources.erase(path);
}

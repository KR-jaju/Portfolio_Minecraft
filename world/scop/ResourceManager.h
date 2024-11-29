#pragma once

#include <memory>
#include <string>
#include <map>

class ResourceManager {
public:
    template <typename T>
    std::shared_ptr<T> GetResource(const std::string& filename) {
        // 이미 로드된 리소스인지 확인
        auto it = resources.find(filename);
        if (it != resources.end()) {
            // 캐시에서 리소스를 반환
            return std::static_pointer_cast<T>(it->second);
        }
        else {
            // 리소스를 로드하고 캐시에 저장
            std::shared_ptr<T> resource = std::make_shared<T>();
            resource->Load(filename);
            resources[filename] = resource;
            return resource;
        }
    }

    void Clear() {
        resources.clear();
    }

private:
    // 파일 이름을 키로 하고 리소스를 값으로 하는 맵
    std::map<std::string, std::shared_ptr<Resource>> resources;
};
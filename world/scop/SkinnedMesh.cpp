#include "pch.h"
#include "SkinnedMesh.h"
#include <fstream>
#include "json.hpp"

SkinnedMesh::SkinnedMesh(std::wstring const& path)
{
	std::ifstream ifs(path);
	nlohmann::json json;
	
	ifs >> json; // 파싱
	auto const& positions = json["positions"];
	auto const& normals = json["normals"];
	auto const& tangents = json["tangents"];
	auto const& uvs = json["uvs"];
	auto const& bone_weights = json["bone_weights"];
	auto const& bone_indices = json["bone_indices"];
	auto const& indices = json["indices"];
	auto const& bindposes = json["bindposes"];
	auto const& parent = json["bone_parent"]; // 본의 부모 본

	for (int i = 0; i < positions.size() / 4; ++i)
	{
		this->vertices.push_back(
			{
				vec3(positions[i * 3].get<float>(), positions[i * 3 + 1].get<float>(),positions[i * 3 + 2].get<float>()),
				vec3(normals[i * 3].get<float>(), normals[i * 3 + 1].get<float>(),normals[i * 3 + 2].get<float>()),
				vec3(tangents[i * 3].get<float>(), tangents[i * 3 + 1].get<float>(),tangents[i * 3 + 2].get<float>()),
				vec2(uvs[i * 2].get<float>(), uvs[i * 2 + 1].get<float>()),
				vec4(bone_weights[i * 4].get<float>(), bone_weights[i * 4 + 1].get<float>(), bone_weights[i * 4 + 2].get<float>(), bone_weights[i * 4 + 3].get<float>()),
				ivec4(bone_indices[i * 4].get<int>(), bone_indices[i * 4 + 1].get<int>(), bone_indices[i * 4 + 2].get<int>(), bone_indices[i * 4 + 3].get<int>()),
			}
		);
	}
	for (int i = 0; i < indices.size(); ++i)
	{
		this->indices.push_back(indices[i]);
	}
}

Mat const& SkinnedMesh::getBindpose(int bone_idx) const
{
	return (this->bindposes.matrix[bone_idx]);
}

std::vector<int> const& SkinnedMesh::getBoneParent() const
{
	return (this->bone_parent);
}

void	SkinnedMesh::render(Graphics& graphics)
{
	ComPtr<ID3D11DeviceContext> context = graphics.getContext();

	if (this->vertex_buffer == nullptr)
	{
		this->vertex_buffer = std::make_unique<Buffer<EntityVertex>>(graphics.getDevice(), this->vertices.data(), this->vertices.size(), D3D11_BIND_VERTEX_BUFFER);
		this->index_buffer = std::make_unique<Buffer<uint32>>(graphics.getDevice(), this->indices.data(), this->indices.size(), D3D11_BIND_VERTEX_BUFFER);
		this->bindposes_buffer = std::make_unique<ConstantBuffer>(graphics.getDevice(), context, this->bindposes);
	}
	context->IASetVertexBuffers(0, 1, this->vertex_buffer->getComPtr().GetAddressOf(), nullptr, nullptr);
	context->IASetIndexBuffer(this->index_buffer->getComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(3, 1, this->bindposes_buffer->getComPtr().GetAddressOf());
	context->DrawIndexed(this->indices.size(), 0, 0);
}

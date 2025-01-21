#include "pch.h"
#include "SkinnedMesh.h"
#include <fstream>

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
	auto const& parents = json["bone_parents"]; // 본의 부모 본

	for (int i = 0; i < positions.size() / 3; ++i)
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
	for (int i = 0; i < parents.size(); ++i) // 모든 본
	{
		this->bindposes.matrix[i] = {
			bindposes[i * 16 + 0].get<float>(),
			bindposes[i * 16 + 1].get<float>(),
			bindposes[i * 16 + 2].get<float>(),
			bindposes[i * 16 + 3].get<float>(),

			bindposes[i * 16 + 4].get<float>(),
			bindposes[i * 16 + 5].get<float>(),
			bindposes[i * 16 + 6].get<float>(),
			bindposes[i * 16 + 7].get<float>(),

			bindposes[i * 16 + 8].get<float>(),
			bindposes[i * 16 + 9].get<float>(),
			bindposes[i * 16 + 10].get<float>(),
			bindposes[i * 16 + 11].get<float>(),

			bindposes[i * 16 + 12].get<float>(),
			bindposes[i * 16 + 13].get<float>(),
			bindposes[i * 16 + 14].get<float>(),
			bindposes[i * 16 + 15].get<float>(),
		};
		this->bone_parents.push_back(parents[i].get<float>());
	}
}

BoneData const& SkinnedMesh::getBindposes() const
{
	return (this->bindposes);
}

std::vector<int> const& SkinnedMesh::getBoneParents() const
{
	return (this->bone_parents);
}

uint32 SkinnedMesh::getBoneCount() const
{
	return (this->bone_parents.size());
}

void	SkinnedMesh::render(Graphics& graphics)
{
	ComPtr<ID3D11DeviceContext> context = graphics.getContext();

	if (this->vertex_buffer == nullptr)
	{
		this->vertex_buffer = std::make_unique<Buffer<EntityVertex>>(graphics.getDevice(), this->vertices.data(), this->vertices.size(), D3D11_BIND_VERTEX_BUFFER);
		this->index_buffer = std::make_unique<Buffer<uint32>>(graphics.getDevice(), this->indices.data(), this->indices.size(), D3D11_BIND_VERTEX_BUFFER);
	}
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();

	context->IASetVertexBuffers(0, 1, this->vertex_buffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->index_buffer->getComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(this->indices.size(), 0, 0);
}

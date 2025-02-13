#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(RenderingContext& context)
	: context(context),
	model_cb(context.graphics.getDevice(), context.graphics.getContext(), Mat::Identity),
	bindpose_cb(context.graphics.getDevice(), context.graphics.getContext(), BoneData{}),
	pose_cb(context.graphics.getDevice(), context.graphics.getContext(), BoneData{})
{

}

void	Renderer::render(SubchunkMesh& data, ivec2 chunk_idx)
{
	Graphics& graphics = this->context.graphics;
	ComPtr<ID3D11DeviceContext> dc = graphics.getContext();

	this->model_cb.update(Mat::CreateTranslation(vec3(chunk_idx.x * 16, 0, chunk_idx.y * 16)).Transpose());
	dc->VSSetConstantBuffers(1, 1, this->model_cb.getComPtr().GetAddressOf());
	data.draw(context.graphics);
}

void	Renderer::render(Entity& entity)
{
	Graphics& graphics = this->context.graphics;
	ComPtr<ID3D11DeviceContext> dc = graphics.getContext();
	SkinnedMesh* mesh = entity.getMesh();

	if (mesh == nullptr) // TODO : 플레이어가 모델이 없어서 이럼
		return;

	vec3 const position = entity.getPosition();
	int const bone_count = mesh->getBoneCount();

	if (!entity.isVisible())
		return;
	this->model_cb.update(Mat::CreateTranslation(vec3(position.x, position.y, position.z)).Transpose());
	this->bindpose_cb.update(mesh->getBindposes());
	this->pose_cb.update(entity.getPose());
	dc->VSSetConstantBuffers(1, 1, this->model_cb.getComPtr().GetAddressOf());
	dc->VSSetConstantBuffers(2, 1, this->bindpose_cb.getComPtr().GetAddressOf());
	dc->VSSetConstantBuffers(3, 1, this->pose_cb.getComPtr().GetAddressOf());
	mesh->render(graphics);
}
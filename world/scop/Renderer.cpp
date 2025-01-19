#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(RenderingContext& context)
	: context(context),
	model_cb(context.graphics.getDevice(), context.graphics.getContext(), Mat::Identity),
	armature_cb(context.graphics.getDevice(), context.graphics.getContext(), BoneData{})
{

}

void	Renderer::render(SubchunkMeshData& data, ivec3 position)
{
	Graphics& graphics = this->context.graphics;
	ComPtr<ID3D11DeviceContext> dc = graphics.getContext();

	this->model_cb.update(Mat::CreateTranslation(vec3(position.x * 16, position.y * 16, position.z * 16)).Transpose());
	dc->VSSetConstantBuffers(1, 1, this->model_cb.getComPtr().GetAddressOf());
	data.draw(context.graphics);
}

void	Renderer::render(Entity& entity)
{
	Graphics& graphics = this->context.graphics;
	ComPtr<ID3D11DeviceContext> dc = graphics.getContext();
	SkinnedMesh* mesh = entity.getMesh();
	vec3 const position = entity.getPosition();

	if (!entity.isVisible())
		return;
	this->model_cb.update(Mat::CreateTranslation(vec3(position.x, position.y, position.z)).Transpose());
	dc->VSSetConstantBuffers(1, 1, this->model_cb.getComPtr().GetAddressOf());
	mesh->render(graphics);
}
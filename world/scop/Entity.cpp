#include "pch.h"
#include "Entity.h"
#include "EntityGeometry.h"
#include "EntityVertex.h"

Entity::Entity(shared_ptr<EntityGeometry> geometry, shared_ptr<Texture> texture)
	: position(),
	rotation(),
	scale(1, 1, 1),
	texture(texture),
	geometry(geometry)
{

}

void	Entity::render(ComPtr<ID3D11DeviceContext> context) const
{
	Buffer<EntityVertex> const& vbuffer = this->geometry->getVertexBuffer();
	Buffer<uint32> const& ibuffer = this->geometry->getIndexBuffer();

	context->PSGetShaderResources(0, 1, this->texture->getComPtr().GetAddressOf());
	uint32 stride = vbuffer.getStride();
	uint32 offset = vbuffer.getOffset();
	context->IASetVertexBuffers(0, 1,
		vbuffer.getComPtr().GetAddressOf(),
		&stride, &offset);
	context->IASetIndexBuffer(
		ibuffer.getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->DrawIndexed(ibuffer.getCount(), 0, 0);
}

BoneData& Entity::getBoneTransforms()
{
	return this->bone_transform;
}

BoneData const& Entity::getBoneTransforms() const
{
	return this->bone_transform;
}

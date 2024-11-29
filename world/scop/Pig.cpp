#include "pch.h"

#include "Pig.h"
#include "AnimationResource.h"
#include "AnimationUtil.h"
#include "EntityGeometry.h"
#include "SkinnedMesh.h"

Pig::Pig(shared_ptr<SkinnedMesh> mesh, shared_ptr<Texture> texture)
	: behavior_fsm({ {"Idle", &Pig::behaviorIdleState} }, "Idle"),
	animation_fsm({ {"Idle", &Pig::animationIdleState} }, "Idle"),
	texture(texture),
	mesh(mesh)
{
	this->armature = pig_model.default_pose;
	this->animation_time = 0.0f;
}

void	Pig::update()
{
	this->behavior_fsm.update(*this);
	this->updateBoneMatrix();
}


void	Pig::render(ComPtr<ID3D11DeviceContext> context) const
{
	Buffer<EntityVertex> const& vbuffer = this->mesh->getVertexBuffer();
	Buffer<uint32> const& ibuffer = this->mesh->getIndexBuffer();

	context->PSSetShaderResources(0, 1, this->texture->getComPtr().GetAddressOf());
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

void	Pig::updateBoneMatrix()
{
	AnimationClip	idle_clip = pig_animation.at("Scene");

	this->animation_fsm.update(*this);
	AnimationUtil::sample(this->armature.data(), idle_clip, this->animation_time);
	this->animation_time += 0.01f;
	if (this->animation_time > 1.0f)
	{
		this->animation_time -= 1.0f;
	}

	for (int idx : this->mesh->getBoneOrder())
	{
		Transform pose = this->armature[idx];
		int parent_idx = pig_model.parent[idx];
		vec4 rot = pose.rotation;
		Mat localToWorld = Mat::CreateScale(pose.scale) * Mat::CreateFromQuaternion(rot) * Mat::CreateTranslation(pose.position);
		Mat result;
		if (parent_idx != -1)
			result = this->bone_transform.matrix[parent_idx] * localToWorld.Transpose();
		else
			result = localToWorld.Transpose();
		this->bone_transform.matrix[idx] = result;
	}
	for (int idx : this->mesh->getBoneOrder())
	{
		this->bone_transform.bindpose[idx] = pig_model.bindposes[idx];
	}
}

BoneData const& Pig::getBoneTransform() const
{
	return this->bone_transform;
}

bool	Pig::behaviorIdleState()
{

	return (false);
}

bool	Pig::animationIdleState()
{

	return (false);
}
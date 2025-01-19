#include "pch.h"
#include "Pig.h"




Pig::Pig(shared_ptr<SkinnedMesh> geometry, shared_ptr<Texture> texture)
{

}




void	Pig::update(float dt)
{

}

void	Pig::render()
{
	
}


vec3	Pig::getPosition() const
{
	return (this->position);
}

vec3	Pig::getRotation() const
{
	return (this->rotation);
}

bool	Pig::shouldDespawn() const
{
	return (false);
}

void	Pig::setPosition(vec3 position)
{

}

ivec2	Pig::getChunkIndex() const
{
	int x = (int)(this->position.x + ((this->position.x < 0) ? -15 : 0)) / 16;
	int z = (int)(this->position.z + ((this->position.z < 0) ? -15 : 0)) / 16;

	return (ivec2(x, z));
}

vec3	Pig::getVelocity() const
{
	return (this->velocity);
}

void	Pig::setVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

int		Pig::getContactFaces() const
{
	return (this->contact_faces);
}

void	Pig::setContactFaces(int faces)
{
	this->contact_faces = faces;
}

AABB const& Pig::getBoundingBox() const
{
	return (this->bounding_box);
}












//
//#include "Pig.h"
//#include "AnimationResource.h"
//#include "AnimationUtil.h"
//#include "EntityGeometry.h"
//#include "SkinnedMesh.h"
//#include <algorithm>
//
//Pig::Pig(shared_ptr<SkinnedMesh> mesh, shared_ptr<Texture> texture)
//	: behavior_fsm({ {"Idle", &Pig::behaviorIdleState} }, "Idle"),
//	armature(pig_model.default_pose),
//	animator(this->armature.data(), pig_animation, "Scene"),
//	texture(texture),
//	mesh(mesh)
//{
//	for (int idx : this->mesh->getBoneOrder())
//	{
//		this->bone_transform.bindpose[idx] = pig_model.bindposes[idx];
//	}
//}
//
//void	Pig::update()
//{
//	this->behavior_fsm.update(*this);
//	this->animator.update();
//	this->updateBoneMatrix();
//}
//
//
//void	Pig::render(ComPtr<ID3D11DeviceContext> context) const
//{
//	Buffer<EntityVertex> const& vbuffer = this->mesh->getVertexBuffer();
//	Buffer<uint32> const& ibuffer = this->mesh->getIndexBuffer();
//
//	context->PSSetShaderResources(0, 1, this->texture->getComPtr().GetAddressOf());
//	uint32 stride = vbuffer.getStride();
//	uint32 offset = vbuffer.getOffset();
//	context->IASetVertexBuffers(0, 1,
//		vbuffer.getComPtr().GetAddressOf(),
//		&stride, &offset);
//	context->IASetIndexBuffer(
//		ibuffer.getComPtr().Get(),
//		DXGI_FORMAT_R32_UINT,
//		0
//	);
//	context->DrawIndexed(ibuffer.getCount(), 0, 0);
//}
//
//void	Pig::updateBoneMatrix()
//{
//	for (int idx : this->mesh->getBoneOrder())
//	{
//		Transform pose = this->armature[idx];
//		int parent_idx = pig_model.parent[idx];
//		vec4 rot = pose.rotation;
//		Mat localToWorld = Mat::CreateScale(pose.scale) * Mat::CreateFromQuaternion(rot) * Mat::CreateTranslation(pose.position);
//		Mat result;
//		if (parent_idx != -1)
//			result = this->bone_transform.matrix[parent_idx] * localToWorld.Transpose();
//		else
//			result = localToWorld.Transpose();
//		this->bone_transform.matrix[idx] = result;
//	}
//}
//
//BoneData const& Pig::getBoneTransform() const
//{
//	return this->bone_transform;
//}
//
//bool	Pig::behaviorIdleState()
//{
//
//	return (false);
//}
//
///*
//1. 상태 변화 중인지, 한 상태에 머물러 있는지
//2. 상태 변화 중이라면 변화에 대한 파라미터(머물러 있다면 의미 없음)
//3. 
//*/
//

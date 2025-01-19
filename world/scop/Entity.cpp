#include "pch.h"
#include "Entity.h"
//#include "SkinnedMesh.h"
//#include "MobAI.h"

//Entity::Entity()
//	: position(0, 0, 0),
//	armature(),
//	//behavior(nullptr),
//	animator(nullptr)
//{
//
//}

//void	Entity::update(vec3 position)
//{
//
//}

//void	Entity::render(ComPtr<ID3D11DeviceContext> context) const
//{
//
//}
//void	Entity::updateBoneMatrix()
//{
	//std::vector<int> const& parent = this->mesh->getBoneParent();
	//for (int idx : this->mesh->getBoneOrder())
	//{	
	//	Transform const& pose = this->armature[idx];
	//	int parent_idx = parent[idx];
	//	vec4 rot = pose.rotation; //quaternion
	//	Mat localToWorld = Mat::CreateScale(pose.scale) * Mat::CreateFromQuaternion(rot) * Mat::CreateTranslation(pose.position);
	//	Mat result = localToWorld.Transpose();

	//	if (parent_idx != -1)
	//		result = this->bone_transform[parent_idx] * result;
	//	this->bone_transform[idx] = result;
	//}
//}

//BoneData const& Entity::getBoneTransform() const
//{
//	
//}

//void	Entity::setMesh(std::shared_ptr<SkinnedMesh> mesh)
//{
//	this->mesh = mesh;
//}
//
//std::shared_ptr<SkinnedMesh>	Entity::getMesh() const
//{
//	return (this->mesh);
//}

//vec3	Entity::getPosition() const
//{
//	return (vec3::Zero);
//}
//
//vec3	Entity::getRotation() const
//{
//	return (this->rotation);
//}
//
//std::pair<int, int> Entity::getChunkIndex() const
//{
//	int x = (int)this->position.x / 16 - ((this->position.x < 0) ? 1 : 0);
//	int z = (int)this->position.z / 16 - ((this->position.z < 0) ? 1 : 0);
//
//	return (std::make_pair(x, z));
//}
//
//bool	Entity::shouldDespawn() const
//{
//	return false;
//}
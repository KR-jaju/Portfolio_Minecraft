#include "pch.h"
#include "Animator.h"


Animator::Animator(std::shared_ptr<SkinnedMesh> const& mesh)
	: mesh(mesh)
{

}

void	Animator::updatePose(Armature& armature)
{
	int const bone_count = this->mesh->getBoneCount();
	bool processed[32] = {};

	for (int i = 0; i < bone_count; ++i)
		this->calculateBoneHierarchy(armature, i, processed);
	for (int i = 0; i < bone_count; ++i)
		this->pose.matrix[i] = this->pose.matrix[i].Transpose();
}

BoneData const& Animator::getPose() const
{
	return (this->pose);
}

void	Animator::calculateBoneHierarchy(Armature& armature, int bone_idx, bool* processed)
{
	if (processed[bone_idx])
		return;
	int const parent = this->mesh->getBoneParents()[bone_idx];
	Transform const& transform = armature.bone_transform[bone_idx];

	this->calculateBoneHierarchy(armature, parent, processed);
	this->pose.matrix[bone_idx] = this->pose.matrix[bone_idx] * transform.toMatrix();
}


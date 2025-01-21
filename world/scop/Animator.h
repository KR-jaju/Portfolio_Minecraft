#pragma once

#include "SkinnedMesh.h"

template <typename T>
class Animator
{
public:
	Animator(std::shared_ptr<SkinnedMesh> const& mesh)
		: mesh(mesh)
	{}
	void	updatePose(Armature& armature)
	{
		int const bone_count = this->mesh->getBoneCount();
		bool processed[32] = {};

		for (int i = 0; i < bone_count; ++i)
			this->calculateBoneHierarchy(armature, i, processed);
		for (int i = 0; i < bone_count; ++i)
			this->pose.matrix[i] = this->pose.matrix[i].Transpose();
	}
	T& getContext() { return (this->transition_data); }
	BoneData const& getPose() const { return (this->pose); }
private:
	std::shared_ptr<SkinnedMesh> mesh;
	T transition_data;
	BoneData pose;

	void	calculateBoneHierarchy(Armature& armature, int bone_idx, bool* processed)
	{
		if (processed[bone_idx])
			return;
		int const parent = this->mesh->getBoneParents()[bone_idx];
		Transform const& transform = armature.bone_transform[bone_idx];

		if (parent != -1)
		{
			this->calculateBoneHierarchy(armature, parent, processed);
			this->pose.matrix[bone_idx] = transform.toMatrix() * this->pose.matrix[parent];
		}
		else
		{
			this->pose.matrix[bone_idx] = transform.toMatrix();
		}
		processed[bone_idx] = true;
	}
};

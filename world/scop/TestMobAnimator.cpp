#include "pch.h"
#include "TestMobAnimator.h"

TestMobAnimator::TestMobAnimator(AssetManager& asset_manager, std::shared_ptr<SkinnedMesh> const& mesh)
	: Animator(mesh),
	armature(mesh->getBoneCount()),
	base_layer(asset_manager, this->getContext())
{
	this->armature.bone_transform.resize(mesh->getBoneCount());
}

void	TestMobAnimator::update(float dt)
{
	this->base_layer.update(this->armature, dt);
	this->updatePose(this->armature);
}


#pragma once

#include "TestMobAnimationLayer.h"
#include "SkinnedMesh.h"
#include "Animator.h"
#include "AssetManager.h"
#include "TestMobAnimatorContext.h"

class TestMobAnimator : public Animator<TestMobAnimatorContext>
{
public:
	TestMobAnimator(AssetManager& asset_manager, std::shared_ptr<SkinnedMesh> const& mesh);
	void	update(float dt);
private:
	Armature armature;
	TestMobAnimationLayer base_layer;
};


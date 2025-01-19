#include "pch.h"
#include "TestMob.h"

TestMob::TestMob(AssetManager& asset_manager)
	: mesh(asset_manager.load<SkinnedMesh>(L"TestMob.json")),
	position(0.0, 35.0, 0.0),
	bounding_box({ vec3(0, 0.9f, 0), vec3(0.3f, 0.9f, 0.3f) })
{

}

void	TestMob::update(float dt)
{
	if (this->contact_faces & DIRECTION_DOWN_BIT)
		this->velocity.y = 10.0f;
	this->velocity.y -= 25 * dt;
	//this->animator.update(this->armature, dt); // armature update
}

vec3	TestMob::getPosition() const
{
	return (this->position);
}

vec3	TestMob::getRotation() const
{
	return (this->rotation);
}

bool	TestMob::shouldDespawn() const
{
	return (false); // TODO 만들어야함
}

void	TestMob::setPosition(vec3 position)
{
	this->position = position;
}

ivec2	TestMob::getChunkIndex() const
{
	return ivec2(0, 0); // TODO : 고쳐야함
}

vec3	TestMob::getVelocity() const
{
	return (this->velocity);
}

void	TestMob::setVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

int		TestMob::getContactFaces() const
{
	return (this->contact_faces);
}

void	TestMob::setContactFaces(int faces)
{
	this->contact_faces = faces;
}

AABB const& TestMob::getBoundingBox() const
{
	return (this->bounding_box);
}

bool	TestMob::isVisible() const
{
	return (true);
}

SkinnedMesh* TestMob::getMesh() const
{
	return (this->mesh.get());
}

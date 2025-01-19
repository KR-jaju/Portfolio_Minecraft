#include "pch.h"
#include "TestMob.h"

TestMob::TestMob()
{

}

void	TestMob::update(float dt)
{

	this->animator.update(this->armature, dt); // armature update




}

void	TestMob::render()
{

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

bool	TestMob::isVisible() const
{
	return (true);
}

SkinnedMesh* TestMob::getMesh() const
{
	return (nullptr);
}

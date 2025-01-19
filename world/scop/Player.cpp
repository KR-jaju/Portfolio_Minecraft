#include "pch.h"
#include "Player.h"

Player::Player(InputRegistry& input_registry, AssetManager& asset_manager)
	: 
	input_registry(input_registry),
	camera(),
	position(vec3(0, 30, 0)),
	rotation(vec3::Zero),
	owns_input(false),
	bounding_box({vec3(0, 0.9f, 0), vec3(0.3f, 0.9f, 0.3f)}),
	contact_faces(DIRECTION_CLEAR_BIT),
	bobbing_frequency(3.7f),
	bobbing_amplitude(0.04f),
	time(0)
{
}

/*
1. velocity라는 것이 존재함.
2. 충돌을 하면, velocity가 조정됨 (물리엔진이 velocity에 변화를 가함)
3. onGrounded는 중력을 먼저 적용하기 때문에 + 바닥이 항상 평평하기 때문에 항상 제대로 작동한다고 말할 수 있음
4. 박쥐같은 동물은 velocity를 조정해야할 것임. 하지만 velocity를 항상 위로 가도록 해놓으면, 윗면에 닿고 있다고도 감지할 수 있음! -> 모든 물리 현상을 통합할 수 있음

*/


void	Player::update(float dt)
{
	std::pair<float, float> mouse_delta = input_registry.getMouseDelta();

	this->rotation.y -= mouse_delta.first;
	this->rotation.x -= mouse_delta.second;
	if (this->contact_faces & DIRECTION_DOWN_BIT)
		this->groundMovement(this->input_registry, dt);
	else // in air
		this->airMovement(this->input_registry, dt);
}

void	Player::groundMovement(InputRegistry& input_registry, float dt)
{
	std::pair<float, float> movement = input_registry.getMovementInput();
	float	move_speed = 4.0f;
	float	cosine = std::cosf(this->rotation.y);
	float	sine = std::sinf(this->rotation.y);
	float	h_speed = std::sqrtf(this->velocity.x * this->velocity.x + this->velocity.z * this->velocity.z);

	this->time += dt * h_speed * this->bobbing_frequency;
	float offset_x = std::cosf(this->time * 0.5f) * this->bobbing_amplitude * 2.0f;
	float offset_y = std::sinf(this->time) * this->bobbing_amplitude;
	vec3 target = this->position + vec3(0, 1.6f, 0);
	float target_vx = (cosine * movement.first + sine * movement.second) * move_speed;
	float target_vz = (-sine * movement.first + cosine * movement.second) * move_speed;
	vec3 dvxz = vec3(target_vx - this->velocity.x, 0, target_vz - this->velocity.z); // 속도의 변화량

	if (h_speed > 0.1f) // 움직인다면
		target += vec3(cosine * offset_x, offset_y, -sine * offset_x);
	this->camera.setPosition(vec3::Lerp(camera.getPosition(), target, 0.4));
	this->camera.setRotation(this->rotation);
	if (dvxz.Length() < dt * 20.0f)
	{
		this->velocity.x = target_vx;
		this->velocity.z = target_vz;
	}
	else
	{
		dvxz.Normalize();
		this->velocity += dvxz * dt * 20.0f;
	}

	this->velocity.y -= 25.8f * dt;
	if (input_registry.getKeyPressed(' ')) // Jump
	{
		this->velocity.y = 7.5f;
	}
}

void	Player::airMovement(InputRegistry& input_registry, float dt)
{
	std::pair<float, float> movement = input_registry.getMovementInput();
	float	move_speed = 4.0f;
	float	cosine = std::cosf(this->rotation.y);
	float	sine = std::sinf(this->rotation.y);
	float	h_speed = std::sqrtf(this->velocity.x * this->velocity.x + this->velocity.z * this->velocity.z);

	vec3 target = this->position + vec3(0, 1.6f, 0);
	float target_vx = (cosine * movement.first + sine * movement.second) * move_speed;
	float target_vz = (-sine * movement.first + cosine * movement.second) * move_speed;
	vec3 dvxz = vec3(target_vx - this->velocity.x, 0, target_vz - this->velocity.z); // 속도의 변화량

	this->camera.setPosition(vec3::Lerp(camera.getPosition(), target, 0.4));
	this->camera.setRotation(this->rotation);
	if (dvxz.Length() < dt * 4.0f)
	{
		this->velocity.x = target_vx;
		this->velocity.z = target_vz;
	}
	else
	{
		dvxz.Normalize();
		this->velocity += dvxz * dt * 4.0f;
	}
	this->velocity.y -= 25.8f * dt;
}

Camera const&	Player::getCamera() const
{
	return (this->camera);
}

bool	Player::hasInputOwnership() const
{
	return (this->owns_input);
}

void	Player::setInputOwnership(bool has_ownership)
{
	this->owns_input = has_ownership;
}

vec3	Player::getPosition() const
{
	return (this->position);
}

vec3	Player::getRotation() const
{
	return (this->rotation);
}

ivec2	Player::getChunkIndex() const
{
	int x = (int)(this->position.x + ((this->position.x < 0) ? -15 : 0)) / 16;
	int z = (int)(this->position.z + ((this->position.z < 0) ? -15 : 0)) / 16;

	return (ivec2(x, z));
}

void	Player::setPosition(vec3 position)
{
	this->position = position;
}

bool	Player::shouldDespawn() const
{
	return false;
}

vec3	Player::getVelocity() const
{
	return (this->velocity);
}

void	Player::setVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

int	Player::getContactFaces() const
{
	return (this->contact_faces);
}

void	Player::setContactFaces(int faces)
{
	this->contact_faces = faces;
}

AABB const& Player::getBoundingBox() const
{
	return (this->bounding_box);
}

bool	Player::isVisible() const
{
	return (false);
}

SkinnedMesh* Player::getMesh() const
{
	return (nullptr);
}
#include "pch.h"
#include "CameraMove.h"
#include "Transform.h"
#include "TimeSystem.h"
#include "InputSystem.h"
#include "GameObject.h"
#include "Camera.h"

CameraMove::CameraMove(shared_ptr<InputSystem> input_system, shared_ptr<TimeSystem> time_system, shared_ptr<GameObject> camera)
	: input_system(input_system), time_system(time_system), camera(camera)
{

}

CameraMove::~CameraMove()
{
}

void CameraMove::Start()
{

}

void CameraMove::Update()
{
	vec2 mouse_cursor;

	mouse_cursor.x = (static_cast<float>(this->input_system->GetMousePos().x));
	mouse_cursor.y = (static_cast<float>(this->input_system->GetMousePos().y));
	float deltaX = mouse_cursor.x - 400;
	float deltaY = mouse_cursor.y - 325;
	this->camera_yaw += deltaX * this->rotation_sensitive;
	this->camera_pitch += deltaY * this->rotation_sensitive;

	if (camera_pitch > XM_PI / 2.0f)
		camera_pitch = XM_PI / 2.0f - 0.005f;
	if (camera_pitch < -XM_PI / 2.0f)
		camera_pitch = -XM_PI / 2.0f + 0.005f;

	vec3 camera_move_vec;
	camera_move_vec.x = camera_pitch;
	camera_move_vec.y = camera_yaw;
	camera_move_vec.z = 0;
	this->camera->GetTransform()->SetLocalRotation(camera_move_vec);
/*	this->camera->Update();*/	

	float dt = this->time_system->GetDeltaTime();
	vec3 pos = this->camera->GetTransform()->GetPosition();
	if (this->input_system->GetButton(KEY_TYPE::W))
		pos += this->camera->GetTransform()->GetLook() * this->speed * dt;
	if (this->input_system->GetButton(KEY_TYPE::S))
		pos -= this->camera->GetTransform()->GetLook() * this->speed * dt;
	if (this->input_system->GetButton(KEY_TYPE::A))
		pos -= this->camera->GetTransform()->GetRight() * this->speed * dt;
	if (this->input_system->GetButton(KEY_TYPE::D))
		pos += this->camera->GetTransform()->GetRight() * this->speed * dt;
	camera->GetTransform()->SetPosition(pos);
};

// ndc ±¸ÇÑ ½Ä
	//vec4 ndc_position;
	//ndc_position.x = (static_cast<float>(this->input_system->GetMousePos().x - 400) / 400);
	//ndc_position.y = (static_cast<float>(this->input_system->GetMousePos().y - 325) / 325);
	////cout << ndc_position.x << "X\n";
	////cout << ndc_position.y << "Y\n";
	//ndc_position.z = 0.f;
	//ndc_position.w = 1.f;
	//vec4 clip_position;
	//clip_position.x = ndc_position.x * this->camera->GetCamera()->GetNear();
	//clip_position.y = ndc_position.y * this->camera->GetCamera()->GetNear();
	//clip_position.z = ndc_position.z * this->camera->GetCamera()->GetNear();
	//clip_position.w = ndc_position.w * this->camera->GetCamera()->GetNear();
	//Mat mat_projection_reverse = this->camera->GetCamera()->GetProjectionMatrix().Invert();

	//XMVECTOR xm_clip_position = XMLoadFloat4(&clip_position);
	//XMMATRIX xm_projection = XMLoadFloat4x4(&mat_projection_reverse);
	//XMVECTOR xm_view_position = XMVector4Transform(xm_clip_position, xm_projection);
	//Mat mat_view_reverse = this->camera->GetCamera()->GetViewMatrix().Invert();
	//XMMATRIX xm_view = XMLoadFloat4x4(&mat_view_reverse);
	//XMVECTOR xm_world_position = XMVector4Transform(xm_view_position, xm_view);
	//vec4 world_position;
	//XMStoreFloat4(&world_position, xm_world_position);
	//vec3 world_position_vec3;
	//world_position_vec3.x = world_position.x;
	//world_position_vec3.y = world_position.y;
	//world_position_vec3.z = 0;
	//cout << world_position_vec3.x << "X\n";
	//cout << world_position_vec3.y << "Y\n";

	//this->camera->GetCamera()->Update();
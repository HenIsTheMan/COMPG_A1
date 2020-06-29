#include "Camera.h"
#include "Application.h"

Camera::Camera(){} //Default constructor

void Camera::Init(const Vector3 &pos, const Vector3 &target, const Vector3 &up){ //Init cam
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset(){ //Reset cam
	position.Set(0, 0, 80);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt){ //Update cam
	static const float CAMERA_SPEED = 20.f;
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A')){
		position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D')){
		position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W')){
		position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S')){
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
}
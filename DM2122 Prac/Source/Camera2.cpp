#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

double modeBounceTime = 0.0;
extern double elapsedTime = 0.0;
extern float FOV;

Camera2::Camera2(const Vector3 &pos, const Vector3 &target, const Vector3 &up){ //Parameterised constructor for initialising the Camera2 obj
	mode = MODE::LOCK;
	leftMouse = rightMouse = 0;
	yawAngle = pitchAngle = 0.0f;
	spd = 100.0f;
	position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 front = target - pos, right = front.Cross(up).Normalized();
	right.y = 0;
	this->up = defaultUp = right.Cross(front).Normalized();
}

void Camera2::UpdateCamVectors(float yaw, float pitch){ //For cam to respond to mouse movement
	if(mode == MODE::LOCK){
		Mtx44 r1;
		r1.SetToRotation(-yaw, 0, 1, 0);
		position = r1 * position;
		up = r1 * up;

		Vector3 front = target - position, right = front.Cross(up).Normalized();
		right.y = 0;
		up = right.Cross(front).Normalized();
		Mtx44 r2;
		r2.SetToRotation(-pitch, right.x, right.y, right.z);
		position = r2 * position;
	} else if(mode == MODE::FREE){
		yawAngle -= yaw / 2.5f;
		pitchAngle -= -pitch / 2.5f;
		Vector3 front, right;
		front.x = cos(Math::DegreeToRadian(-pitchAngle)) * cos(Math::DegreeToRadian(-yawAngle - 90.f));
		front.y = sin(Math::DegreeToRadian(-pitchAngle));
		front.z = cos(Math::DegreeToRadian(-pitchAngle)) * sin(Math::DegreeToRadian(-yawAngle - 90.f));
		front = front.Normalized();
		right = front.Cross(up).Normalize();
		right.y = 0;
		up = right.Cross(front).Normalize();
		target = position + front;
	}
}

void Camera2::Update(double dt){ //Update cam
	if(Application::IsKeyPressed('B') && modeBounceTime <= elapsedTime){ //Change cam mode
		Reset();
		mode = MODE(!bool(mode));
		modeBounceTime = elapsedTime + 0.4;
	}
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A')){ //Move cam towards the left
		if(mode == MODE::LOCK){
			float yaw = float(-spd * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position = rotation * position;
			up = rotation * up;
		} else if(mode == MODE::FREE){
			Vector3 front = target - position, right = front.Cross(up).Normalized();
			right.y = 0;
			position += float(-spd * dt) * (FOV / 45.f) * right;
			target += float(-spd * dt) * (FOV / 45.f) * right;
		}

	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D')){ //Move cam towards the right
		if(mode == MODE::LOCK){
			float yaw = float(spd * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position = rotation * position;
			up = rotation * up;
		} else if(mode == MODE::FREE){
			Vector3 front = target - position, right = front.Cross(up).Normalized();
			right.y = 0;
			position += float(spd * dt) * (FOV / 45.f) * right;
			target += float(spd * dt) * (FOV / 45.f) * right;
		}
	}
	if(Application::IsKeyPressed('Q')){ //Move cam upwards
		if(mode == MODE::LOCK){
			float pitch = float(-spd * dt);
			Vector3 front = target - position, right = front.Cross(up).Normalized();
			right.y = 0;
			up = right.Cross(front).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position = rotation * position;
		} else if(mode == MODE::FREE){
			position += float(spd * dt) * (FOV / 45.f) * up.Normalized();
			target += float(spd * dt) * (FOV / 45.f) * up.Normalized();
		}
	}
	if(Application::IsKeyPressed('E')){ //Move cam downwards
		if(mode == MODE::LOCK){
			float pitch = float(spd * dt);
			Vector3 front = target - position, right = front.Cross(up).Normalized();
			right.y = 0;
			up = right.Cross(front).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position = rotation * position;
		} else if(mode == MODE::FREE){
			position += float(-spd * dt) * (FOV / 45.f) * up.Normalized();
			target += float(-spd * dt) * (FOV / 45.f) * up.Normalized();
		}
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W')){ //Move cam forward
		Vector3 dir = target - position, front = dir.Normalized();
		if(mode == MODE::LOCK){
			position += front * float(spd / 2 * dt) * (dir.Length() > 5);
		} else if(mode == MODE::FREE){
			front.y = 0;
			position += float(spd * dt) * (FOV / 45.f) * front.Normalized();
			target += float(spd * dt) * (FOV / 45.f) * front.Normalized();
		}
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S')){ //Move cam backward
		Vector3 dir = target - position, front = dir.Normalized();
		if(mode == MODE::LOCK){
			position += front * float(-spd / 2 * dt) * (dir.Length() < 200);
		} else if(mode == MODE::FREE){
			front.y = 0;
			position += float(-spd * dt) * (FOV / 45.f) * front.Normalized();
			target += float(-spd * dt) * (FOV / 45.f) * front.Normalized();
		}
	}
	if(mode == MODE::FREE){ //Move cam in the direction of target
		Vector3 front = (target - position).Normalized();
		if(leftMouse && !rightMouse){
			position += float(spd * dt) * (FOV / 45.f) * front.Normalized();
			target += float(spd * dt) * (FOV / 45.f) * front.Normalized();
		}
		if(rightMouse && !leftMouse){
			position += float(-spd * dt) * (FOV / 45.f) * front.Normalized();
			target += float(-spd * dt) * (FOV / 45.f) * front.Normalized();
		}
	}
	if(Application::IsKeyPressed(VK_END)){ //Reset cam
		mode = MODE::LOCK;
		position = defaultPosition;
		target = defaultTarget;
		up = defaultUp;
		FOV = 45.0f;
	}
}
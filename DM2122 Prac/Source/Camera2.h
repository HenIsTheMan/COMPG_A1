#pragma once
#include "Camera.h"

class Camera2: public Camera{
	public:
		enum class MODE{
			LOCK, FREE
		};
		Vector3 defaultPosition, defaultTarget, defaultUp;
		Camera2(const Vector3&, const Vector3&, const Vector3&);
		bool leftMouse, rightMouse;
		float spd, yawAngle, pitchAngle;
		MODE mode;
		void UpdateCamVectors(float, float);
		void Update(double);
};
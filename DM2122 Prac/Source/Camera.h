#pragma once
#include "Vector3.h"

class Camera{
	public:
		Vector3 position, target, up;
		Camera();
		void Init(const Vector3&, const Vector3&, const Vector3&), Update(double), Reset();
};
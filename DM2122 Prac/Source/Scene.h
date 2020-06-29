#pragma once
#include "Camera2.h"

class Scene{
	public:
		Scene(){}
		virtual ~Scene(){}
		virtual void Update(double, float) = 0, Render(Camera2&) = 0;
};
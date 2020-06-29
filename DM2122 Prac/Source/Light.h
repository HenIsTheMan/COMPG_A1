#pragma once
#include "Vertex.h"

struct Light{
	Light(){
		type = Light::LIGHT_TYPE::LIGHT_SPOT;
		position.Set(0, 25, 0);
		color.Set(1, 1, 1);
		power = 1.f;
		kC = 1.f;
		kL = 0.01f;
		kQ = 0.001f;
		cosCutoff = cos(Math::DegreeToRadian(45));
		cosInner = cos(Math::DegreeToRadian(30));
		exponent = 3.f;
		spotDirection.Set(0.f, 1.f, 0.f);
	}
	enum class LIGHT_TYPE{
		LIGHT_POINT, LIGHT_DIRECTIONAL, LIGHT_SPOT
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	Position position;
	Color color;
	float power, kC, kL, kQ, cosCutoff, cosInner, exponent;
};
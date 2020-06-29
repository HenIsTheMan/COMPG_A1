#include "Vector3.h"

bool Vector3::IsEqual(float a, float b) const{
	return a - b <= Math::EPSILON && b - a <= Math::EPSILON;
}

Vector3::Vector3(float a, float b, float c): x(a), y(b), z(c){} //Default constructor
Vector3::Vector3(const Vector3 &RHS): x(RHS.x), y(RHS.y), z(RHS.z){} //Copy constructor
Vector3::~Vector3(){}

void Vector3::Set(float a, float b, float c){
	x = a;
	y = b;
	z = c;
}

void Vector3::SetZero(){
	x = y = z = 0.0f;
}

bool Vector3::IsZero() const{
	return IsEqual(x, 0.f) && IsEqual(y, 0.f) && IsEqual(z, 0.f);
}

Vector3 Vector3::operator+(const Vector3 &RHS) const{
	return Vector3(x + RHS.x, y + RHS.y, z + RHS.z);
}

Vector3 &Vector3::operator+=(const Vector3 &RHS){
	x += RHS.x;
	y += RHS.y;
	z += RHS.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3 &RHS) const{
	return Vector3(x - RHS.x, y - RHS.y, z - RHS.z);
}

Vector3 &Vector3::operator-=(const Vector3& RHS){
	x -= RHS.x;
	y -= RHS.y;
	z -= RHS.z;
	return *this;
}

Vector3 Vector3::operator-() const{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(float scalar) const{
	return Vector3(scalar * x, scalar * y, scalar * z);
}

Vector3 &Vector3::operator*=(float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

bool Vector3::operator==(const Vector3 &RHS) const{ //Using Math::EPSILON
	return IsEqual(x, RHS.x) && IsEqual(y, RHS.y) && IsEqual(z, RHS.z);
}

bool Vector3::operator!=(const Vector3 &RHS) const{ //Using Math::EPSILON
	return !IsEqual(x, RHS.x) || !IsEqual(y, RHS.y) || !IsEqual(z, RHS.z);
}

Vector3 &Vector3::operator=(const Vector3 &RHS){
	x = RHS.x;
	y = RHS.y;
	z = RHS.z;
	return *this;
}

float Vector3::Length() const{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::LengthSquared() const{
	return x * x + y * y + z * z;
}

float Vector3::Dot(const Vector3 &RHS) const{
	return x * RHS.x + y * RHS.y + z * RHS.z;
}

Vector3 Vector3::Cross(const Vector3 &RHS) const{
	return Vector3(y * RHS.z - z * RHS.y, z * RHS.x - x * RHS.z, x * RHS.y - y * RHS.x);
}

Vector3 Vector3::Normalized() const throw(DivideByZero){
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON){
		throw DivideByZero();
	}
	return Vector3(x / d, y / d, z / d);
}

Vector3 &Vector3::Normalize() throw(DivideByZero){
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON){
		throw DivideByZero();
	}
	x /= d;
	y /= d;
	z /= d;
	return *this;
}

std::ostream& operator<<(std::ostream &os, Vector3 &RHS){
	os << "[ " << RHS.x << ", " << RHS.y << ", " << RHS.z << " ]";
	return os;
}

Vector3 operator*(float scalar, const Vector3 &RHS){ //Friend overloaded operator for scalar multiplication
	return RHS * scalar;
}
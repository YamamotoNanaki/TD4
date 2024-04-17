#include "Float4.h"
#include <assert.h>

using namespace IFE;

Float4::Float4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}

void IFE::Float4::Set(float _x, float _y, float _z, float _w)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->w = _w;
}

Float4 Float4::operator+() const
{
	return *this;
}

Float4 Float4::operator-() const
{
	return Float4(-x, -y, -z, -w);
}

Float4 Float4::operator*() const
{
	return *this;
}

Float4& Float4::operator+=(const Float4& f)
{
	x += f.x;
	y += f.y;
	z += f.z;
	w += f.w;
	return *this;
}

Float4& Float4::operator-=(const Float4& f)
{
	x -= f.x;
	y -= f.y;
	z -= f.z;
	w -= f.w;
	return *this;
}

Float4& Float4::operator*=(const Float4& f)
{
	x *= f.x;
	y *= f.y;
	z *= f.z;
	w *= f.w;
	return *this;
}

Float4& Float4::operator/=(const Float4& f)
{
	assert(f.x == 0 && "—ëœZ");
	assert(f.y == 0 && "—ëœZ");
	assert(f.z == 0 && "—ëœZ");
	assert(f.w == 0 && "—ëœZ");
	x /= f.x;
	y /= f.y;
	z /= f.z;
	w /= f.w;
	return *this;
}

Float4& Float4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Float4& Float4::operator/=(float f)
{
	assert(f == 0 && "—ëœZ");
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

Float4 IFE::Float4::operator=(const Float4& f)
{
	x = f.x;
	y = f.y;
	z = f.z;
	w = f.w;
	return *this;
}

Float4 IFE::Float4::operator=(float* f)
{
	x = f[0];
	y = f[1];
	z = f[2];
	w = f[3];
	return *this;
}

Float4 IFE::Float4::operator=(float f)
{
	x = f;
	y = f;
	z = f;
	w = f;
	return *this;
}

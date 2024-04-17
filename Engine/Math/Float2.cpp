#include "Float2.h"
#include <assert.h>

using namespace IFE;

Float2::Float2(float x, float y) :x(x), y(y) {}

void IFE::Float2::Set(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

Float2 Float2::operator+() const
{
	return *this;
}

Float2 Float2::operator-() const
{
	return Float2(-x, -y);
}

Float2 Float2::operator*() const
{
	return *this;
}

Float2& Float2::operator+=(const Float2& f)
{
	x += f.x;
	y += f.y;
	return *this;
}

Float2& Float2::operator-=(const Float2& f)
{
	x -= f.x;
	y -= f.y;
	return *this;
}

Float2& Float2::operator*=(const Float2& f)
{
	x *= f.x;
	y *= f.y;
	return *this;
}

Float2& Float2::operator/=(const Float2& f)
{
	assert(f.x == 0 && "—ëœZ");
	assert(f.y == 0 && "—ëœZ");
	x /= f.x;
	y /= f.y;
	return *this;
}

Float2& Float2::operator*=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

Float2& Float2::operator/=(float f)
{
	x /= f;
	y /= f;
	return *this;
}

bool IFE::Float2::operator==(const Float2& f)const
{
	return x == f.x && y == f.y;
}

bool IFE::Float2::operator!=(const Float2& f) const
{
	return !(*this == f);
}

#ifndef _VEC2F_H_
#define _VEC2F_H_

#include <cmath>

class Vec2f
{
public:
	float x;
	float y;

public:
	Vec2f() = default;
	~Vec2f() = default;

	Vec2f(float var_x, float var_y)
		:x(var_x), y(var_y) {}

	Vec2f operator+(Vec2f vec2f) const {
		return Vec2f(x + vec2f.x, y + vec2f.y);
	}

	Vec2f operator-(Vec2f vec2f) const {
		return Vec2f(x - vec2f.x, y - vec2f.y);
	}

	void operator+=(Vec2f vec2f) {
		x += vec2f.x;
		y += vec2f.y;
	}

	void operator-=(Vec2f vec2f) {
		x -= vec2f.x;
		y -= vec2f.y;
	}

	Vec2f operator*(float var) const {
		return Vec2f(var * x, var * y);
	}

	void operator*=(float var) {
		x = var * x;
		y = var * y;
	}

	float operator*(Vec2f vec2f) const {
		return x * vec2f.x + y * vec2f.y;
	}

	float length() const {
		return sqrtf(x * x + y * y);
	}

	Vec2f nomalized() const {
		float len = length();

		if (len == 0)
			return Vec2f(0, 0);
		else
			return Vec2f(x / len, y / len);
	}

	float angle() const {
		return atan2f(y, x);
	}
};

const Vec2f FUP = { 0,-1 };
const Vec2f FDOWN = { 0,1 };
const Vec2f FLEFT = { -1,0 };
const Vec2f FRIGHT = { 1,0 };

#endif // !_VEC2F_H_
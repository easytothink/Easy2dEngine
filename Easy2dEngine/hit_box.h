#ifndef _HIT_BOX_H_
#define _HIT_BOX_H_

#include <functional>

#include "vec2f.h"

class HitBoxManager;

class HitBox
{
	friend class HitBoxManager;

public:
	enum class HitLayer
	{
		NONE = 0,
		USER_MOUSE,
		BUTTON
	};

public:
	void SetEnabled(bool flag) { 
		enabled = flag; 
	}

	void SetOnHit(std::function<void()> func) { 
		on_hit = func; 
	}

	void SetSource(HitLayer hit_layer) { 
		source = hit_layer; 
	}

	void SetTarget(HitLayer hit_layer) { 
		target = hit_layer; 
	}

	void SetRect(float left, float top, float right, float bottom) {
		left_top = Vec2f(left, top);
		right_bottom = Vec2f(right, bottom);
	}

private:
	Vec2f left_top;
	Vec2f right_bottom;
	bool enabled = true;
	std::function<void()> on_hit;
	HitLayer source = HitLayer::NONE;
	HitLayer target = HitLayer::NONE;

private:
	HitBox(Vec2f left_top, Vec2f right_bottom)
		:left_top(left_top), right_bottom(right_bottom) {}
	HitBox(float left, float top, float right, float bottom)
		:left_top(left, top), right_bottom(right, bottom) {}
	~HitBox() = default;
};

#endif // !_HIT_BOX_H_
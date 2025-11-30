#ifndef _SLIDER_H_
#define _SLIDER_H_

#include <functional>
#include "SDL3/SDL.h"
#include "camera.h"

class Slider
{
public:
	Slider(SDL_FRect rect, int* target, int min, int max, int initial) {
		this->rect = rect;
		this->target = target;
		this->min = min;
		this->max = max;
		this->ratio = 1.0f * (initial - min) / (max - min);
	};
	~Slider() = default;

private:
	bool CheckHit(float x,float y) {
		return true;//TODO
	}

public:
	void SetIntial(int initial) {
		this->ratio = 1.0f * (initial - min) / (max - min);
	}

	void SetCalcDeltaRatio(std::function<float(SDL_Event*)> function) {
		this->calc_delta_ratio = function;
	}

	void OnEvent(SDL_Event* event) {
		switch (event->type)
		{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (!selected && CheckHit(event -> motion.x,event -> motion.y))
				selected = true;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (selected)
				selected = false;
			break;
		case SDL_EVENT_MOUSE_MOTION:
			if (selected && calc_delta_ratio) {
				ratio += calc_delta_ratio(event);
				if (ratio > 1.0f) ratio = 1.0f;
				else if (ratio < 0.0f) ratio = 0.0f;
			}
			break;
		}
	}

	void OnUpdate(float ticks) {
		*target = (int)(ratio * (max - min) + min);
	}

	void OnRender(Camera* camera) {

	}

private:
	SDL_FRect rect;
	bool selected = false;
	int* target;
	int min, max;
	float ratio;
	std::function<float(SDL_Event*)> calc_delta_ratio;
};

#endif // !_SLIDER_H_
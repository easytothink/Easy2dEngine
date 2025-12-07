#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>
#include <functional>
#include "SDL3/SDL.h"
#include "resources_manager.h"
#include "vec2f.h"
#include "camera.h"

class Button
{
public:
	enum class Status
	{
		Idle = 0,
		Hovered,
		Pushed
	};

public:
	Button(SDL_FRect rect, SDL_Texture* img_idle, SDL_Texture* img_hovered, SDL_Texture* img_pushed) {
		this->rect = rect;
		this->img_idle = img_idle;
		this->img_hovered = img_hovered;
		this->img_pushed = img_pushed;
	};
	Button(SDL_FRect rect,std::string idle,std::string hovered,std::string pushed) {
		this->rect = rect;
		img_idle = ResourcesManager::instance()->FindTexture(idle);
		img_hovered = ResourcesManager::instance()->FindTexture(hovered);
		img_pushed = ResourcesManager::instance()->FindTexture(pushed);
	};
	~Button() = default;

private:
	bool CheckHit(float x, float y) const {
		return x>rect.x && x<rect.x+rect.w && y>rect.y && y<rect.y+rect.h;
	}

public:
	void SetStatus(Status status) {
		this->status = status;
	}

	void SetClick(std::function<void()> on_click) {
		this->on_click = on_click;
	}

	void OnEvent(SDL_Event* event, Vec2f camera_offset) {
		switch (event->motion.type)
		{
		case SDL_EVENT_MOUSE_MOTION:
			if (status == Status::Idle && CheckHit(event->motion.x + camera_offset.x, event->motion.y + camera_offset.y))
				status = Status::Hovered;
			else if (status == Status::Hovered && !CheckHit(event->motion.x + camera_offset.x, event->motion.y + camera_offset.y))
				status = Status::Idle;
			break;
		}
		switch (event->button.type)
		{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (CheckHit(event->motion.x + camera_offset.x, event->motion.y + camera_offset.y))
				status = Status::Pushed;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (status == Status::Pushed && on_click)
				on_click();

			if(CheckHit(event->motion.x + camera_offset.x, event->motion.y + camera_offset.y))
				status = Status::Hovered;
			else
				status = Status::Idle;
			break;
		}
	}
	void OnUpdate(float ticks) {

	}
	void OnRender(Camera* camera) {
		switch (status)
		{
		case Button::Status::Idle:
			camera->RenderTexture(img_idle, &rect);
			break;
		case Button::Status::Hovered:
			camera->RenderTexture(img_hovered, &rect);
			break;
		case Button::Status::Pushed:
			camera->RenderTexture(img_pushed, &rect);
			break;
		}
	}

private:
	SDL_FRect rect;
	SDL_Texture* img_idle = nullptr;
	SDL_Texture* img_hovered = nullptr;
	SDL_Texture* img_pushed = nullptr;
	Status status = Status::Idle;
	std::function<void()> on_click;
};

#endif // !_BUTTON_H_
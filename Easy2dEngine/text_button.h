#ifndef _TEXT_BUTTON_H_
#define _TEXT_BUTTON_H_

#include "button.h"

class TextButton
{
public:
	TextButton() = default;
	~TextButton() = default;

	void Create(SDL_Renderer* renderer, SDL_FRect rect, std::string font, const char* text) {
		SDL_Surface* temp_idle = nullptr;
		SDL_Surface* temp_hovered = nullptr;
		SDL_Surface* temp_pushed = nullptr;
		SDL_Texture* img_idle = nullptr;
		SDL_Texture* img_hovered = nullptr;
		SDL_Texture* img_pushed = nullptr;

		temp_idle = TTF_RenderText_Blended(
			ResourcesManager::instance()->FindFont(font), text, NULL, {127,127,127,255});
		if (!temp_idle) {
			SDL_Log(u8"¼ÓÔØ×Ö·û´®±íÃæÊ§°Ü£º%s", SDL_GetError());
			return;
		}

		img_idle = SDL_CreateTextureFromSurface(renderer, temp_idle);
		if (!img_idle) {
			SDL_Log(u8"´´½¨×Ö·û´®²ÄÖÊ´íÎó£º%s", SDL_GetError());
			return;
		}

		temp_hovered = TTF_RenderText_Blended(
			ResourcesManager::instance()->FindFont(font), text, NULL, { 95,95,95,255 });
		if (!temp_hovered) {
			SDL_Log(u8"¼ÓÔØ×Ö·û´®±íÃæÊ§°Ü£º%s", SDL_GetError());
			return;
		}

		img_hovered = SDL_CreateTextureFromSurface(renderer, temp_hovered);
		if (!img_hovered) {
			SDL_Log(u8"´´½¨×Ö·û´®²ÄÖÊ´íÎó£º%s", SDL_GetError());
			return;
		}

		temp_pushed = TTF_RenderText_Blended(
			ResourcesManager::instance()->FindFont(font), text, NULL, { 63,63,63,255 });
		if (!temp_pushed) {
			SDL_Log(u8"¼ÓÔØ×Ö·û´®±íÃæÊ§°Ü£º%s", SDL_GetError());
			return;
		}

		img_pushed = SDL_CreateTextureFromSurface(renderer, temp_pushed);
		if (!img_pushed) {
			SDL_Log(u8"´´½¨×Ö·û´®²ÄÖÊ´íÎó£º%s", SDL_GetError());
			return;
		}

		button = new Button(rect, img_idle, img_hovered, img_pushed);

		SDL_DestroySurface(temp_idle);
		SDL_DestroySurface(temp_hovered);
		SDL_DestroySurface(temp_pushed);
	}

	void SetStatus(Button::Status status) {
		button->SetStatus(status);
	}
	void SetClick(std::function<void()> on_click) {
		button->SetClick(on_click);
	}
	void OnEvent(SDL_Event* event, Vec2f camera_offset) {
		button->OnEvent(event, camera_offset);
	}
	void OnUpdate(float ticks) {
		button->OnUpdate(ticks);
	}
	void OnRender(Camera* camera) {
		button->OnRender(camera);
	}

private:
	Button* button;
};

#endif // !_TEXT_BUTTON_H_
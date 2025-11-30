#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <string>
#include <SDL3/SDL.h>
#include "camera.h"
#include "resources_manager.h"

class TextBox
{
public:
	TextBox(SDL_FRect rect, std::string font, const char* text, SDL_Color fg) {
		this->rect = rect;
		this->font = font;
		this->text = text;
		this->fg = fg;
	};
	~TextBox() = default;

	void OnEvent(SDL_Event* event) {
		
	}
	void OnUpdate(float ticks) {

	}
	void OnRender(Camera* camera) {
		camera->OnRenderTexture(img_text, &rect);
	}

	void Init(SDL_Renderer* renderer) {
		SDL_Surface* surface = nullptr;
		surface = TTF_RenderText_Blended(ResourcesManager::instance()->FindFont(font), text, NULL, fg);
		if (!surface) {
			SDL_Log(u8"加载字符串表面失败：%s", SDL_GetError());
			return;
		}

		img_text = SDL_CreateTextureFromSurface(renderer, surface);
		if (!img_text) {
			SDL_Log(u8"创建字符串材质错误：%s", SDL_GetError());
			return;
		}
		SDL_DestroySurface(surface);
	}

private:
	std::string font;
	const char* text;
	SDL_Color fg;
	SDL_FRect rect;
	SDL_Texture* img_text = nullptr;
};

#endif // !_TEXTBOX_H_
#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <string>
#include <SDL3/SDL.h>
#include "camera.h"
#include "resources_manager.h"
#include "text.h"

class TextBox
{
public:
	TextBox(SDL_FRect rect, const char* plain_text, TTF_Font* font) {
		this->rect = rect;
		this->text = new Text(plain_text, font, { 255,255,255,0 });
	};
	TextBox(SDL_FRect rect, const char* plain_text, const char* font_id) {
		this->rect = rect;
		this->text = new Text(plain_text, ResourcesManager::instance()->FindFont(font_id), {255,255,255,0});
	}
	~TextBox() {
		delete text;
	};

public:
	template<typename T>
	void SetProperties(Text::FontProperty prop, T t) {
		text->SetProperties(prop, t);
	}

	void ApplyProperties(int index) {
		text->ApplyProperties(index);
	}

	void ApplyProperties(std::vector<int> indexes) {
		text->ApplyProperties(indexes);
	}

	void SetColor(int index, SDL_Color fg) {
		text->SetColor(index, fg);
	}

	void OnEvent(SDL_Event* event) {
		
	}

	void OnUpdate(float ticks) {

	}

	void OnRender(Camera* camera) {
		if (rect.w == 0 || rect.h == 0) {
			rect.w = img_text->w;
			rect.h = img_text->h;
		}

		camera->RenderTexture(img_text, &rect);
	}

	void Load(Camera* camera) {
		std::vector<SDL_Surface*> _chr_suf_group;
		text->Text2Surface(_chr_suf_group);

		//加载字符数据，转换为小图片
		std::vector<SDL_Texture*> _chr_tex_group;
		int w = 0, h = 0;

		for (auto& surface : _chr_suf_group)
		{
			SDL_Texture* texture = nullptr;
			texture = camera->CreateTextureFromSurface(surface);
			if (!texture) {
				SDL_Log("材质转换失败，原因：%s", SDL_GetError());
				return;
			}
			
			_chr_tex_group.push_back(texture);
			w += texture->w;
			h = (h > texture->h) ? h : texture->h;
			SDL_DestroySurface(surface);
		}

		//合并字符图片 TODO
		SDL_FPoint prev_pos = { 0,0 };
		SDL_FRect rect{};
		img_text = camera->CreateTexture(SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, w, h);
		camera->SetRenderTarget(img_text);
		for (auto& img : _chr_tex_group)
		{
			prev_pos.x = img->w;
			rect.w = img->w;
			rect.h = img->h;

			camera->RenderTexture(img, &rect);
			SDL_DestroyTexture(img);

			rect.x += prev_pos.x;
			rect.y += prev_pos.y;
		}
		_chr_tex_group.clear();
		camera->SetRenderTarget(NULL);
	}

private:
	SDL_FRect rect;
	Text* text;
	SDL_Texture* img_text = nullptr;
};

#endif // !_TEXTBOX_H_
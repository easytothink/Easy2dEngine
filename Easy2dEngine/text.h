#ifndef _TEXT_H_
#define _TEXT_H_

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <functional>
#include <unordered_map>

class Text
{
public:
	enum class AlignStyle
	{
		TOP = 0,
		BOTTOM,
		CENTER
	};

public:
    Text(SDL_FRect rect, const char* plain_text, TTF_Font* font, SDL_Color fg, AlignStyle style) {
		this->rect = rect;
		this->style = style;
		
		text.clear();
		on_handle_all.clear();
        if (!plain_text)
            return;

        size_t length = strlen(plain_text);
        for (size_t i = 0, len = 0; i < length; i += len)
		{
			unsigned char byte = (unsigned)(*(plain_text + i));
			if (byte >= 0xF0)
				len = 4;
			else if (byte >= 0xE0)
				len = 3;
			else if (byte >= 0xC0)
				len = 2;
			else
				len = 1;
            std::string chr;
            chr.assign(plain_text + i, len);
            text.emplace_back(chr, font, fg);
		}
	};
	Text(SDL_FRect rect, const char* plain_text, std::string font, SDL_Color fg, AlignStyle style)
		:Text(rect, plain_text, ResourcesManager::instance()->FindFont(font), fg, style) {
	};
	Text(SDL_FRect rect, const char* plain_text, std::string font, SDL_Color fg)
		:Text(rect, plain_text, font, fg, AlignStyle::BOTTOM) {
	};
	Text(SDL_FRect rect, const char* plain_text, std::string font)
		:Text(rect, plain_text, font, { 255,255,255,0 }) {
	};

    ~Text() {
        text.clear();
        on_handle_all.clear();
        if (img_text)
            SDL_DestroyTexture(img_text);
    };

public:
	enum class FontProperty
	{
		SIZE = 0,
		STYLE
	};

	size_t GetLength() {
		return text.size();
	}

	template<typename T>
	void SetProperties(FontProperty prop, T t) {
		switch (prop)
		{
		case FontProperty::SIZE:
			on_handle_all.push_back([=](TTF_Font* base) {
					TTF_SetFontSize(base, (float)t);
				});
			break;
		case FontProperty::STYLE:
			on_handle_all.push_back([=](TTF_Font* base) {
					TTF_SetFontStyle(base,(TTF_FontStyleFlags)t);
				});
			break;
		default:
			break;
		}
	}

	void ApplyProperties(int index) {
		if (index >= 0 && index < GetLength()) {
			TTF_Font* new_font = ResourcesManager::instance()->
				RequestDynamicFont(text[index].font, on_handle_all);
			text[index].font = new_font;
			on_handle_all.clear();
		}
	}

	void ApplyProperties(std::vector<int> indexes) {
		for (auto& index : indexes) {
			if (index >= 0 && index < GetLength()) {
				TTF_Font* new_font = ResourcesManager::instance()->
					RequestDynamicFont(text[index].font, on_handle_all);
				text[index].font = new_font;
			}
		}
		on_handle_all.clear();
	}

	void SetColor(int index, SDL_Color fg) {
		if (index >= 0 && index < GetLength()) {
			text[index].fg = fg;
		}
	}

	void SetColor(std::unordered_map<int, SDL_Color> pairs) {
		for (auto& pair : pairs)
		{
			text[pair.first].fg = pair.second;
		}
	}

	void Load(Camera* camera) {
		//加载字符数据，转换为小图片
        std::vector<SDL_Texture*> _chr_tex_group;
        int w = 0, h = 0;
        for (auto& chr : text)
        {
            SDL_Surface* surface = nullptr;
            surface = TTF_RenderText_Blended(chr.font, chr.bytes.c_str(), NULL, chr.fg);
			if (!surface) {
				SDL_Log("表面加载失败，原因：%s", SDL_GetError());
				return;
			}

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

		//合并字符图片
		SDL_FRect rect{};
		rect.x = 0;
		rect.y = (h - _chr_tex_group[0]->h) / 2.0f;
		img_text = camera->CreateTexture(SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, w, h);
		if (!img_text) {
			SDL_Log("字符图像创建失败");
			return;
		}
		camera->SetRenderTarget(img_text);
		int size = _chr_tex_group.size();
		for (int i = 0; i < size; i++)
		{
			rect.w = _chr_tex_group[i]->w;
			rect.h = _chr_tex_group[i]->h;

			camera->RenderTexture(_chr_tex_group[i], &rect);
			SDL_DestroyTexture(_chr_tex_group[i]);

			if (i >= size - 1)
				break;

			rect.x += _chr_tex_group[i]->w;
			switch (style)
			{
			case AlignStyle::TOP:
				rect.y = 0;
				break;
			case AlignStyle::BOTTOM:
				rect.y = h - _chr_tex_group[i + 1]->h;
				break;
			case AlignStyle::CENTER:
				rect.y = (h - _chr_tex_group[i + 1]->h) / 2.0f;
				break;
			}
		}
		_chr_tex_group.clear();
		camera->SetRenderTarget(NULL);
	}

	void OnEvent(SDL_Event* event) {

	}

	void OnUpdate(float ticks) {

	}

	void OnRender(Camera* camera) {
		if (img_text && (rect.w == 0 || rect.h == 0)) {
			SDL_GetTextureSize(img_text, &rect.w, &rect.h);
		}

		camera->RenderTexture(img_text, &rect);
	}

private:
	struct CharElement
	{
		std::string bytes;
		TTF_Font* font;
		SDL_Color fg;

		CharElement() = default;
		~CharElement() = default;
		CharElement(const std::string& bytes, TTF_Font* font, SDL_Color fg)
			:bytes(bytes), font(font) ,fg(fg) {};
	};

private:
	SDL_FRect rect;
	AlignStyle style;
	std::vector<std::function<void(TTF_Font*)>> on_handle_all;
	std::vector<CharElement> text;
	SDL_Texture* img_text = nullptr;
};

#endif // !_TEXT_H_
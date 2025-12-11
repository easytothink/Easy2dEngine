#ifndef _TEXT_H_
#define _TEXT_H_

#include <string>
#include <vector>

class Text
{
public:
	Text(const char* plain_text, TTF_Font* font, SDL_Color fg) {
		text.clear();
		on_handle_all.clear();
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
			char* chr = nullptr;
			chr = (char*)malloc((len + 1) * sizeof(char));
			if (!chr)
				return;//TODO
			strncpy_s(chr, len + 1, plain_text + i, len);
			chr[len] = '\0';
			text.emplace_back(chr, font, fg);
		}
	};

	~Text() {
		for (auto& chr : text) {
			free(chr.bytes);
		}
		text.clear();
	};

public:
	enum class FontProperty
	{
		SIZE = 0,
		SYTLE
	};

	template<typename T>
	void SetProperties(FontProperty prop, T t) {
		switch (prop)
		{
		case FontProperty::SIZE:
			on_handle_all.push_back([=](TTF_Font* base) {
					TTF_SetFontSize(base, (float)t);
				});
			break;
		case FontProperty::SYTLE:
			on_handle_all.push_back([=](TTF_Font* base) {
					TTF_SetFontStyle(base,(TTF_FontStyleFlags)t);
				});
			break;
		default:
			break;
		}
	}

	void ApplyProperties(int index) {
		TTF_Font* new_font = ResourcesManager::instance()->
			RequestDynamicFont(text[index].font, on_handle_all);
		text[index].font = new_font;
		on_handle_all.clear();
	}

	void ApplyProperties(std::vector<int> indexes) {
		for (auto& index : indexes) {
			TTF_Font* new_font = ResourcesManager::instance()->
				RequestDynamicFont(text[index].font, on_handle_all);
			text[index].font = new_font;
		}
		on_handle_all.clear();
	}

	void SetColor(int index, SDL_Color fg) {
		text[index].fg = fg;
	}

	void SetColor(std::unordered_map<int, SDL_Color> pairs) {
		for (auto& pair : pairs)
		{
			text[pair.first].fg = pair.second;
		}
	}

	void Text2Surface(std::vector<SDL_Surface*>& output) {
		for (auto& chr : text)
		{
			SDL_Surface* surface = nullptr;
			surface = TTF_RenderText_Blended(chr.font, chr.bytes, NULL, chr.fg);
			if (!surface) {
				SDL_Log("表面加载失败，原因：%s", SDL_GetError());
				return;
			}
			output.push_back(surface);
		}
	}

private:
	struct CharElement
	{
		char* bytes;
		TTF_Font* font;
		SDL_Color fg;

		CharElement() = default;
		~CharElement() = default;
		CharElement(char* bytes, TTF_Font* font, SDL_Color fg)
			:bytes(bytes), font(font) ,fg(fg) {};
	};

private:
	std::vector<std::function<void(TTF_Font*)>> on_handle_all;
	std::vector<CharElement> text;
};

#endif // !_TEXT_H_
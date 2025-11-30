#ifndef _ALTAS_H_
#define _ALTAS_H_

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#include <vector>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() {
		for (SDL_Texture* texture : tex_list)
			SDL_DestroyTexture(texture);
	}

	void LoadFiles(SDL_Renderer* renderer, const char* path_template, int num) {
		for (int i = 0; i < num; i++) {
			char file_path[256]{};
			sprintf_s(file_path, path_template, i + 1);
			SDL_Texture* texture = IMG_LoadTexture(renderer, file_path);
			tex_list.push_back(texture);
		}
	}

	void Clear() {
		for (SDL_Texture* texture : tex_list)
			SDL_DestroyTexture(texture);

		tex_list.clear();
	}

	int Size() const {
		return (int)tex_list.size();
	}

	SDL_Texture* GetTexture(int idx) {
		if (idx<0 || idx>(int)tex_list.size())
			return nullptr;

		return tex_list[idx];
	}

	void AddTexture(SDL_Texture* texture) {
		tex_list.push_back(texture);
	}

private:
	std::vector<SDL_Texture*> tex_list;
};

#endif // !_ALTAS_H_
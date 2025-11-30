#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <string>
#include <tchar.h>
#include <unordered_map>
#include <Windows.h>
#include <fstream>

#include "nlohmann/json.hpp"

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "atlas.h"
#include "IO_exception.h"

class ResourcesManager
{
public:
	static ResourcesManager* instance();

	void Load(SDL_Renderer* renderer);
	void Load(SDL_Renderer* renderer, const char* filename);

	Atlas* FindAtlas(const std::string& id) const;
	SDL_Texture* FindTexture(const std::string& id) const;
	TTF_Font* FindFont(const std::string& id) const;

private:
	static ResourcesManager* manager;

	std::unordered_map<std::string, Atlas*> atlas_pool;
	std::unordered_map<std::string, SDL_Texture*> texture_pool;
	std::unordered_map<std::string, TTF_Font*> font_pool;

private:
	ResourcesManager();
	~ResourcesManager();
};

#endif // !RESOURCES_MANAGER_H_
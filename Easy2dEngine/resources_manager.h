#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <string>
#include <tchar.h>
#include <unordered_map>
#include <Windows.h>
#include <fstream>

#include "nlohmann/json.hpp"
#include "md5.h"
#include "hex.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "atlas.h"
#include "IO_exception.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#define MD5_CRYPT_LEN 16  // 16´ú±í32×Ö½Ú

class ResourcesManager
{
public:
	static ResourcesManager* instance();

	void Load(SDL_Renderer* renderer);
	void Load(SDL_Renderer* renderer, const char* filename);
	void RemoveAll();
	TTF_Font* RequestDynamicFont(TTF_Font* base, const std::vector<std::function<void(TTF_Font*)>>& on_handle_all);
	void RemoveDynamicFont(const std::string& id);

	Atlas* FindAtlas(const std::string& id) const;
	SDL_Texture* FindTexture(const std::string& id) const;
	TTF_Font* FindFont(const std::string& id) const;

private:
	static ResourcesManager* manager;

	std::unordered_map<std::string, Atlas*> atlas_pool;
	std::unordered_map<std::string, SDL_Texture*> texture_pool;
	std::unordered_map<std::string, TTF_Font*> font_pool;

	std::unordered_map<std::string, TTF_Font*> dynamic_font_pool;

private:
	ResourcesManager();
	~ResourcesManager();
};

#endif // !RESOURCES_MANAGER_H_
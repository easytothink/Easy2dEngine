#include "resources_manager.h"

struct TextureResInfo
{
	std::string id;
	char path[64];
};

struct AtlasResInfo
{
	std::string id;
	char path[64];
	int num = 0;
};

struct FontResInfo
{
	std::string id;
	char path[64];
	float size;
};

//资源文件 名字-路径对

static const std::vector<TextureResInfo> texture_info_list = {
};

static const std::vector<AtlasResInfo> altas_info_list = {
};

static const std::vector<FontResInfo> font_info_list = {
};

//CLASS ResourcesManager

ResourcesManager* ResourcesManager::manager = nullptr;

ResourcesManager* ResourcesManager::instance() {
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

void ResourcesManager::Load(SDL_Renderer* renderer) {
	for (const auto& info : texture_info_list) {
		SDL_Texture* texture = nullptr;
		texture = IMG_LoadTexture(renderer, info.path);
		if (!texture)
			throw info.path;
		texture_pool[info.id] = texture;
	}
	for (const auto& info : altas_info_list) {
		Atlas* atlas = new Atlas();
		atlas->LoadFiles(renderer, info.path, info.num);
		for (int i = 0; i < atlas->Size(); i++) {
			SDL_Texture* texture = nullptr;
			texture = atlas->GetTexture(i);
			if (!texture)
				throw info.path;
		}
		atlas_pool[info.id] = atlas;
	}
	for (const auto& info : font_info_list) {
		TTF_Font* font = nullptr;
		font = TTF_OpenFont(info.path, info.size);
		if (!font)
			throw info.path;
		font_pool[info.id] = font;
	}
}

void ResourcesManager::Load(SDL_Renderer* renderer, const char* root) {
	using json = nlohmann::json;

	std::string root_dir = root;
	std::string metadata = root_dir + "\\resources.json";
	std::ifstream file(metadata);
	if (!file.good())
		throw IOException(metadata.c_str());

	json j = json::parse(file);
	for (const auto& info : j["textures"])
	{
		std::string id = info["id"].get<std::string>();
		std::string path = root_dir + "\\" + info["path"].get<std::string>();
		SDL_Texture* texture = nullptr;
		texture = IMG_LoadTexture(renderer, path.c_str());
		if (!texture)
			throw IOException(path.c_str());;
		texture_pool[id] = texture;
	}
	for (const auto& info : j["altas"])
	{
		std::string id = info["id"].get<std::string>();
		std::string path = root_dir + "\\" + info["path"].get<std::string>();
		int num = info["num"].get<int>();
		Atlas* atlas = new Atlas();
		atlas->LoadFiles(renderer, path.c_str(), num);
		for (int i = 0; i < atlas->Size(); i++) {
			SDL_Texture* texture = nullptr;
			texture = atlas->GetTexture(i);
			if (!texture)
				throw IOException(path.c_str());;
		}
		atlas_pool[id] = atlas;
	}
	for (const auto& info : j["fonts"])
	{
		std::string id = info["id"].get<std::string>();
		std::string path = root_dir + "\\" + info["path"].get<std::string>();
		float size = info["size"].get<float>();
		TTF_Font* font = nullptr;
		font = TTF_OpenFont(path.c_str(), size);
		if (!font)
			throw IOException(path.c_str());;
		font_pool[id] = font;
	}
}

Atlas* ResourcesManager::FindAtlas(const std::string& id) const
{
	const auto& iter = atlas_pool.find(id);
	if (iter == atlas_pool.end())
		return nullptr;

	return iter->second;
}

SDL_Texture* ResourcesManager::FindTexture(const std::string& id) const
{
	const auto& iter = texture_pool.find(id);
	if (iter == texture_pool.end())
		return nullptr;

	return iter->second;
}

TTF_Font* ResourcesManager::FindFont(const std::string& id) const
{
	const auto& iter = font_pool.find(id);
	if (iter == font_pool.end())
		return nullptr;

	return iter->second;
}

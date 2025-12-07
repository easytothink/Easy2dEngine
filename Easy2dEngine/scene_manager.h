#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <string>
#include <unordered_map>
#include "scene.h"
#include "test_scene.h"

class SceneManager
{
public:
	static SceneManager* instance();

	void LoadAll(SDL_Renderer* renderer);
	void RemoveAll();
	void OnEvent(SDL_Event* event);
	void OnUpdate(float ticks);
	void OnRender();

private:
	static SceneManager* manager;

	std::unordered_map<std::string, Scene*> scene_pool;
	Scene* current_scene = nullptr;

private:
	SceneManager();
	~SceneManager();
};

#endif // !_SCENE_MANAGER_H_
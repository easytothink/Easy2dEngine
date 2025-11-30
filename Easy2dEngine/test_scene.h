#ifndef _TEST_SCENE_
#define _TEST_SCENE_

#include "scene.h"
#include "resources_manager.h"

class TestScene : public Scene
{
public:
	TestScene(SDL_Renderer* renderer) {
		main_camera = new Camera(renderer);
	};
	~TestScene() = default;

	void OnEnter()
	{
		test_texture = ResourcesManager::instance()->FindTexture("img_head");
	}

	void OnExit() {}
	void OnEvent(SDL_Event* event) {}
	void OnUpdate(float ticks) {}
	void OnRender()
	{
		SDL_FRect rect = { 0,0,100,100 };
		main_camera->OnRenderTexture(test_texture, &rect);
	}

private:
	SDL_Texture* test_texture = nullptr;
};

#endif // !_TEST_SCENE_
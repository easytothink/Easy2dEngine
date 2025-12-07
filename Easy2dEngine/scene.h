#ifndef _SCENE_H_
#define _SCENE_H_

#include <stdio.h>
#include <string>
#include "SDL3/SDL.h"
#include "camera.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual void OnEvent(SDL_Event* event) {};
	virtual void OnUpdate(float ticks) {};
	virtual void OnRender() {};

	bool IsQuitting() const {
		return quitting;
	}
	std::string GetTargetScene() {
		return target_scene;
	}

protected:
	Camera* main_camera = nullptr;
	bool quitting = false;
	std::string target_scene;
};

#endif // !_SCENE_H_
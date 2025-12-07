#include "scene_manager.h"

SceneManager* SceneManager::manager = nullptr;

SceneManager* SceneManager::instance()
{
    if (!manager)
        manager = new SceneManager();

    return manager;
}

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::LoadAll(SDL_Renderer* renderer)
{
    scene_pool["test_scene"] = new TestScene(renderer);

    current_scene = scene_pool.find("test_scene")->second;
    current_scene->OnEnter();
}

void SceneManager::RemoveAll()
{
    for (auto& scene : scene_pool) {
        delete scene.second;
    }
    scene_pool.clear();
}

void SceneManager::OnEvent(SDL_Event* event)
{
    current_scene->OnEvent(event);
}

void SceneManager::OnUpdate(float ticks)
{
    if (!current_scene->IsQuitting())
        current_scene->OnUpdate(ticks);
    else {
        current_scene->OnExit();
        std::string scene = current_scene->GetTargetScene();
        current_scene = scene_pool.find(scene)->second;
        current_scene->OnEnter();
    }
}

void SceneManager::OnRender()
{
    current_scene->OnRender();
}

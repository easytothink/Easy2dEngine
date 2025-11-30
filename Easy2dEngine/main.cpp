#include <chrono>
#include <thread>
#include <Windows.h>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "resources_manager.h"
#include "scene_manager.h"

bool running = true;
const char WINDOW_TITLE[] = u8"DEMO";
const int WINDOW_HEIGHT = 720;
const int WINDOW_WDITH = 1280;

int main(int argc, char* argv[]) {
    //图形界面创建
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log(u8"SDL初始化失败: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        WINDOW_TITLE,
        WINDOW_WDITH,
        WINDOW_HEIGHT,
        SDL_WINDOW_MAXIMIZED);

    if (!window) {
        SDL_Log(u8"创建窗口失败: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    //资源加载
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log(u8"创建渲染器失败: %s\n", SDL_GetError());
        return -1;
    }

    if (!TTF_Init()) {
        SDL_Log(u8"SDL_ttf初始化失败: %s\n", SDL_GetError());
        return -1;
    }

    try {
        //ResourcesManager::instance()->Load(renderer);//内置
        ResourcesManager::instance()->Load(renderer,"test\\assets");//本地存储
        SceneManager::instance()->LoadAllScenes(renderer);
    }
    catch (IOException& e) {
        SDL_Log(u8"无法加载资源文件：%s", e.what());
        return -1;
    }

    //事件
    SDL_Event event;

    //时间控制初始化
    using namespace std::chrono;

    const nanoseconds frame_duration(1000000000 / 60);
    steady_clock::time_point last_tick = steady_clock::now();

    //主循环
    while (running) {
        //事件处理
        while (SDL_PollEvent(&event)) {
            SceneManager::instance()->OnEvent(&event);

            if(event.type == SDL_EVENT_QUIT)
                running = false;
        }

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> ticks = duration<float>(frame_start - last_tick);

        //逻辑处理
        SceneManager::instance()->OnUpdate(ticks.count());

        //渲染
        SDL_RenderClear(renderer);
        SceneManager::instance()->OnRender();
        SDL_RenderPresent(renderer);

        //动态延时
        last_tick = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if (sleep_duration > nanoseconds(0))
            std::this_thread::sleep_for(sleep_duration);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
#ifndef _TEST_SCENE_
#define _TEST_SCENE_

#include "scene.h"
#include "resources_manager.h"
#include "animation.h"
#include "button.h"
#include "textbox.h"

class TestScene : public Scene
{
public:
	TestScene(SDL_Renderer* renderer) {
		main_camera = new Camera(renderer);

		test_textbox.SetProperties(Text::FontProperty::SIZE, 72.0f);
		test_textbox.SetProperties(Text::FontProperty::SYTLE, TTF_STYLE_ITALIC);
		test_textbox.ApplyProperties(0);
		test_textbox.SetProperties(Text::FontProperty::SIZE, 36.0f);
		test_textbox.SetProperties(Text::FontProperty::SYTLE, TTF_STYLE_UNDERLINE);
		test_textbox.ApplyProperties({ 1,3,5,6 });
		test_textbox.SetProperties(Text::FontProperty::SYTLE, TTF_STYLE_STRIKETHROUGH);
		test_textbox.ApplyProperties({ 5,6 });
		test_textbox.SetColor({
			{ 3,{127,0,255,0} },
			{ 4,{255,255,0,0} },
			{ 6,{255,0,255,0} },
			{ 7,{0,255,255,0} }
			});
		test_textbox.Load(main_camera);
	};
	~TestScene() {
		delete main_camera;
	};

	void OnEnter()
	{
		test_texture = ResourcesManager::instance()->FindTexture("img_head");

		test_animation.SetAnchorMode(Animation::AnchorMode::BOTTOM_CENTER);
		test_animation.SetPosition(200, 100);
		test_animation.SetInterval(0.1f);//tick
		test_animation.SetLoop(true);
		test_animation.AddFrame(ResourcesManager::instance()->FindTexture("img_test_animaiton"), 16);
		/*test_animation.SetOnFinish([]() {
				SDL_Log(u8"动画播放完毕");
			});*/

		test_button.SetStatus(Button::Status::Idle);
		test_button.SetClick([]() {
				SDL_Log(u8"你点击了按钮");
			});
	}

	void OnExit() {}
	void OnEvent(SDL_Event* event) {
		test_button.OnEvent(event, main_camera->GetPosition());
	}

	void OnUpdate(float ticks) {
		test_animation.OnUpdate(ticks);
		test_button.OnUpdate(ticks);
		test_textbox.OnUpdate(ticks);
	}

	void OnRender()
	{
		SDL_FRect rect = { 0,0,100,100 };
		main_camera->RenderTexture(test_texture, &rect);

		test_animation.OnRender(main_camera);
		test_button.OnRender(main_camera);
		test_textbox.OnRender(main_camera);
	}

private:
	SDL_Texture* test_texture = nullptr;
	Animation test_animation;
	Button test_button = {
		{400,100,64,64},
		"img_btn_level_idle",
		"img_btn_level_hovered",
		"img_btn_level_pushed" 
	};
	TextBox test_textbox = {
		{500,100,0,0},
		u8"测试-我的世界ABCabc123%&*&@",
		"font_lyusung_32px"
	};
};

#endif // !_TEST_SCENE_
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SDL3/SDL.h"
#include "vec2f.h"
#include "atlas.h"
#include "timer.h"
#include "camera.h"

class Animation
{
public:
	Animation() {
		timer.SetOneShot(false);
		timer.SetOnTimeout([&]() {
			idx_frame++;
			if (idx_frame >= frame_list.size()) {
				idx_frame = is_loop ? 0 : frame_list.size() - 1;
				if (!is_loop && on_finish)
					on_finish();
			}
		});
	};
	~Animation() = default;

public:
	enum class AnchorMode
	{
		CENTER = 0,
		BOTTOM_CENTER
	};

public:
	void Reset() {
		timer.Restart();
		idx_frame = 0;
	}

	void SetPosition(const Vec2f& pos) {
		this->pos = pos;
	}

	void SetPosition(float x, float y) {
		this->pos.x = x;
		this->pos.y = y;
	}

	void SetRotation(double angle) {
		this->angle = angle;
	}

	void SetCenter(const SDL_FPoint& center) {
		this->center = center;
	}

	void SetCenter(float x, float y) {
		this->center.x = x;
		this->center.y = y;
	}

	void SetLoop(bool loop) {
		this->is_loop = loop;
	}

	void SetInterval(float interval) {
		timer.SetWaitTime(interval);
	}

	void SetOnFinish(std::function<void()> on_finish) {
		this->on_finish = on_finish;
	}

	void SetAnchorMode(AnchorMode mode) {
		this->anchor = mode;
	}

	void AddFrame(SDL_Texture* texture, int num) {
		int width_frame = texture->w / num;
		for (int i = 0; i < num; i++)
		{
			SDL_FRect rect_src{};
			rect_src.x = i * width_frame * 1.0f;
			rect_src.y = 0.0f;
			rect_src.h = texture->h * 1.0f;
			rect_src.w = width_frame * 1.0f;

			frame_list.emplace_back(rect_src, texture);
		}
	}

	void AddFrame(Atlas* atlas) {
		for (int i = 0; i < atlas->Size(); i++) {
			SDL_Texture* texture = atlas->GetTexture(i);

			SDL_FRect rect_src{};
			rect_src.x = 0.0f;
			rect_src.y = 0.0f;
			rect_src.h = texture->h * 1.0f;
			rect_src.w = texture->w * 1.0f;

			frame_list.emplace_back(rect_src, texture);
		}
	}

	void OnEvent(SDL_Event* event) {

	}
	void OnUpdate(float ticks) {
		timer.OnUpdate(ticks);
	}
	void OnRender(Camera* camera) {
		const Frame& frame = frame_list[idx_frame];
		const Vec2f& pos_camera = camera->GetPosition();

		SDL_FRect rect_dst{};
		float anchor_offset = (anchor == AnchorMode::CENTER) ? frame.rect_src.h / 2 : frame.rect_src.h;
		rect_dst.x = pos.x - frame.rect_src.w / 2 - pos_camera.x;
		rect_dst.y = pos.y - anchor_offset - pos_camera.y;
		rect_dst.w = frame.rect_src.w;
		rect_dst.h = frame.rect_src.h;

		camera->RenderTexture(frame.texture, &frame.rect_src, &rect_dst, angle, &center);
	}

private:
	struct Frame
	{
		SDL_FRect rect_src;
		SDL_Texture* texture = nullptr;

		Frame() = default;
		Frame(const SDL_FRect& rect_src, SDL_Texture* texture)
			:rect_src(rect_src), texture(texture) {};
		~Frame() = default;
	};

private:
	Vec2f pos = { 0,0 };//坐标
	double angle = 0;//逆时针旋转角度
	SDL_FPoint center = { 0 };//旋转中心

	bool is_loop = true;
	size_t idx_frame = 0;
	Timer timer;
	std::vector<Frame> frame_list;
	std::function<void()> on_finish;
	AnchorMode anchor = AnchorMode::CENTER;
};

#endif // !_ANIMATION_H_
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SDL3/SDL.h"
#include "vec2f.h"

class Camera
{
public:
	Camera(SDL_Renderer* renderer) {
		this->position = { 0,0 };
		this->renderer = renderer;
	}
	~Camera() = default;

	Vec2f GetPosition() const {
		return position;
	}

	void SetPosition(Vec2f vec2f) {
		this->position = vec2f;
	}

	void OnUpdate(float ticks) {

	}

	void RenderTexture(SDL_Texture* texture, const SDL_FRect* src_rect,
			const SDL_FRect* dst_rect, double angle, const SDL_FPoint* center) {
		SDL_FRect dst_rect_win = *dst_rect;
		dst_rect_win.x -= position.x;
		dst_rect_win.y -= position.y;

		SDL_RenderTextureRotated(
			renderer, texture, src_rect, 
			&dst_rect_win, angle, center, SDL_FlipMode::SDL_FLIP_NONE);
	}

	void RenderTexture(SDL_Texture* texture, const SDL_FRect* src_rect, const SDL_FRect* dst_rect)
	{
		RenderTexture(texture, src_rect, dst_rect, 0, { 0 });
	}

	void RenderTexture(SDL_Texture* texture, const SDL_FRect* dst_rect){
		RenderTexture(texture, NULL, dst_rect);
	}

	void SetRenderTarget(SDL_Texture* texture) {
		SDL_SetRenderTarget(renderer, texture);
	}

	SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface) {
		return SDL_CreateTextureFromSurface(renderer, surface);
	}

	SDL_Texture* CreateTexture(SDL_PixelFormat format, SDL_TextureAccess access, 
			int w, int h) {
		return SDL_CreateTexture(renderer, format, access, w, h);
	}

	void SetColor(Uint8 r, Uint8 g, Uint8 b,Uint8 a) {
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void RenderRect(SDL_FRect* rect, SDL_Color color) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderRect(renderer, rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}

	void RenderFillRect(SDL_FRect* rect, SDL_Color color) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}

private:
	Vec2f position;
	SDL_Renderer* renderer = nullptr;
};

#endif // !_CAMERA_H_
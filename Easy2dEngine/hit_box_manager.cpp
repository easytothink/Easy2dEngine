#include "hit_box_manager.h"

HitBoxManager* HitBoxManager::manager = nullptr;

HitBoxManager* HitBoxManager::instance()
{
    if (!manager)
        manager = new HitBoxManager();
    return manager;
}

HitBoxManager::HitBoxManager() = default;
HitBoxManager::~HitBoxManager() = default;

HitBox* HitBoxManager::CreateHitBox(const Vec2f& left_top, const Vec2f& right_bottom)
{
    HitBox* hit_box = new HitBox(left_top, right_bottom);
	hit_box_list.push_back(hit_box);
	return hit_box;
}

HitBox* HitBoxManager::CreateHitBox(float left, float top, float right, float bottom)
{
	HitBox* hit_box = new HitBox(left, top, right, bottom);
	hit_box_list.push_back(hit_box);
	return hit_box;
}

void HitBoxManager::DestroyHitBox(HitBox* hit_box)
{
	hit_box_list.erase(std::remove(hit_box_list.begin(), hit_box_list.end(), hit_box), hit_box_list.end());
	delete hit_box;
}

void HitBoxManager::OnEvent(SDL_Event* event, HitBox* mouse_hit_box)
{
	if (event->motion.type == SDL_EVENT_MOUSE_MOTION)
		mouse_hit_box->SetRect(event->motion.x, event->motion.y, event->motion.x, event->motion.y);
}

void HitBoxManager::OnUpdate(int delta)
{
	for (auto& hit_box_src : hit_box_list)
	{
		if (!hit_box_src->enabled || hit_box_src->target == HitBox::HitLayer::NONE)
			continue;

		for (auto& hit_box_dst : hit_box_list) {
			if (!hit_box_dst->enabled || hit_box_dst == hit_box_dst
				|| hit_box_src->target != hit_box_dst->source)
				continue;

			bool is_hit = hit_box_src->left_top.x < hit_box_dst->right_bottom.x
				&& hit_box_src->right_bottom.x > hit_box_dst->left_top.x
				&& hit_box_src->left_top.y < hit_box_dst->right_bottom.y
				&& hit_box_src->right_bottom.y > hit_box_dst->left_top.y;

			if (is_hit && hit_box_dst->on_hit)
				hit_box_dst->on_hit();
		}
	}
}

HitBox* HitBoxManager::CheckHit(HitBox* hit_box_src)//TODO
{
	HitBox* hit_box_dst = nullptr;
	return nullptr;
}

void HitBoxManager::OnRender(Camera* camera)
{
	for (auto& hit_box : hit_box_list)
	{
		SDL_FRect rect;
		rect.x = hit_box->left_top.x;
		rect.y = hit_box->left_top.y;
		rect.w = hit_box->right_bottom.x - hit_box->left_top.x;
		rect.h = hit_box->right_bottom.y - hit_box->left_top.y;

		camera->RenderRect(&rect, { 220,20,60,255 });
	}
}

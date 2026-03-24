#ifndef _HIT_BOX_MANAGER_H_
#define _HIT_BOX_MANAGER_H_

#include <vector>

#include "hit_box.h"
#include "camera.h"

class HitBoxManager
{
public:
	static HitBoxManager* instance();

	HitBox* CreateHitBox(const Vec2f& left_top, const Vec2f& right_bottom);
	HitBox* CreateHitBox(float left, float top, float right, float bottom);
	void DestroyHitBox(HitBox* hit_box);

	void OnEvent(SDL_Event* event, HitBox* mouse_hit_box);
	void OnUpdate(int delta);
	HitBox* CheckHit(HitBox* hit_box_src);
	void OnRender(Camera* camera);

private:
	static HitBoxManager* manager;

	std::vector<HitBox*> hit_box_list;

private:
	HitBoxManager();
	~HitBoxManager();
};

#endif // !_HIT_BOX_MANAGER_H_
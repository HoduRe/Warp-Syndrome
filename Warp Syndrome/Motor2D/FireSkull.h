#ifndef __FIRESKULL_H__
#define __FIRESKULL_H__

#include "Enemy.h"
#include "Elemental.h"

class Enemy_FireSkull : public Enemy
{
public:

	Enemy_FireSkull(int x, int y);
	Enemy_FireSkull(int x, int y, enemy_states startingstate, float aHealth);
	~Enemy_FireSkull();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void CheckAnimation(enemy_states currentstate, enemy_states laststate);
	void ChangeAnimation(Elemental_Anim_List animations);
	float pathfinding_update_rate = 0.0f;
	bool Save(pugi::xml_node& data) const;

};

#endif // __FIRESKULL_H__

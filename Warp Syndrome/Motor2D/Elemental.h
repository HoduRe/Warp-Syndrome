#ifndef __ELEMENTAL_H__
#define __ELEMENTAL_H__

#include "Enemy.h"

enum Elemental_Anim_List
{
	EL_ANIM_IDLE,
	EL_ANIM_MOVING,
	EL_ANIM_ATTACKING,
	EL_ANIM_DIE,
	EL_ANIM_UNKNOWN
};

class Enemy_Elemental : public Enemy
{
public:

	Enemy_Elemental(int x, int y);
	Enemy_Elemental(int x, int y,enemy_states startingstate,float aHealth);
	~Enemy_Elemental();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void CheckAnimation(enemy_states currentstate, enemy_states laststate);
	void ChangeAnimation(Elemental_Anim_List animations);
	float pathfinding_update_rate=0.0f;
	bool Save(pugi::xml_node& data) const;

};

#endif // __ELEMENTAL_H__

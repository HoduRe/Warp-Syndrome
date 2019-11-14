#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "j1Collision.h"

struct SDL_Texture;

class Enemy
{
public:
	p2List<Animations> animations;//list of animations that the enemy will use when active (this has to be a copy of the correct p2List member of the EnemyAnimations Struct)
	collision_type collider = LAST_COLLISION;
	iPoint position;
	bool enabled;

public:
	Enemy();
	Enemy(int x, int y);
	virtual ~Enemy();

	virtual void Move();
	virtual void Draw(SDL_Texture* sprites);
};

#endif // __ENEMY_H__
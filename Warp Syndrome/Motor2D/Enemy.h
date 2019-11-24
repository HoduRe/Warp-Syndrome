#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "p2DynArray.h"
#include "j1Collision.h"
#include "Entity.h"
#include "Character.h"
#include "p2DynArray.h"

struct SDL_Texture;

enum enemy_states
{
	E_STATE_DEFAULT,//normal enemy behaviour when doesn't see the player
	E_STATE_CHASING,
	E_STATE_ATTACKING,
	E_STATE_DIE,
	E_STATE_UNKNOWN
};

class Enemy:public Character
{
public:
	enemy_states state;
	enemy_states last_state;
	collider_type collider;
	p2DynArray<iPoint> path;

public:
	Enemy(EntityType atype= E_TYPE_ENEMY);
	Enemy(int x, int y, enemy_states startingstate=E_STATE_DEFAULT, EntityType atype = E_TYPE_ENEMY);
	virtual ~Enemy();
	virtual void Move();
	virtual void Draw();
	// Checks if the animation has to be changed
	virtual void CheckAnimation(enemy_states currentstate, enemy_states laststate);
	virtual void ChangeAnimation();
	int CheckDistance(float x, float y);
};

#endif // __ENEMY_H__
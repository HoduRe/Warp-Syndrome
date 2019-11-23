#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "p2DynArray.h"
#include "j1Collision.h"
#include "Entity.h"
#include "Character.h"

struct SDL_Texture;

enum enemy_states
{
	E_STATE_DEFAULT,//normal enemy behaviour when doesn't see the player
	E_STATE_CHASING,
	E_STATE_ATTACKING,
	E_STATE_UNKNOWN
};

class Enemy:public Character
{
public:
	enemy_states state;
public:
	Enemy(EntityType atype= E_TYPE_ENEMY);
	Enemy(int x, int y, EntityType atype = E_TYPE_ENEMY);
	virtual ~Enemy();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual void Move();
	virtual void Draw(SDL_Texture* sprites);
	void GeneralMove(int* x, int* y, p2DynArray<iPoint>& path);
	int CheckDistance(int x, int y);
};

#endif // __ENEMY_H__
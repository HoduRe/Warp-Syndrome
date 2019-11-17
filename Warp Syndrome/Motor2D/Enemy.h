#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "p2DynArray.h"
#include "j1Collision.h"

struct SDL_Texture;

class Enemy
{
protected:
	Animations* animation = nullptr;
	collision_type collider = LAST_COLLISION;

public:
	iPoint position;
	bool enabled;
	p2DynArray<iPoint> path;

public:
	Enemy();
	Enemy(int x, int y);
	virtual ~Enemy();

	virtual void Move() {};
	void GeneralMove(int* x, int* y, p2DynArray<iPoint>& path);
};

#endif // __ENEMY_H__
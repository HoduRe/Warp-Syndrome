#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "j1Collision.h"

struct SDL_Texture;
struct Collider;

class Enemy
{
protected:
	Animations* animation = nullptr;
	collision_type collider = LAST_COLLISION;

public:
	iPoint position;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const collision_type GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
};

#endif // __ENEMY_H__
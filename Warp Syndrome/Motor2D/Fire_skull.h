#ifndef __FIRE_SKULL_H__
#define __FIRE_SKULL_H__

#include "Enemy.h"

class Enemy_FireSkull : public Enemy
{
private:

public:

	Enemy_FireSkull(int x, int y);
	~Enemy_FireSkull();

	void Move();
};

#endif // __FIRE_SKULL_H__

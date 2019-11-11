#ifndef __FIRE_SKULL_H__
#define __FIRE_SKULL_H__

#include "Enemy.h"

class Enemy_FireSkull : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animations fly;

public:

	Enemy_FireSkull(int x, int y);

	void Move();
};

#endif // __FIRE_SKULL_H__

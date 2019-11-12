#ifndef __HELL_HORSE_H__
#define __HELL_HORSE_H__

#include "Enemy.h"

class Enemy_HellHorse : public Enemy
{
private:
	int original_y = 0;
	Animations move;

public:

	Enemy_HellHorse(int x, int y);

	void Move();
};

#endif // __HELL_HORSE_H__

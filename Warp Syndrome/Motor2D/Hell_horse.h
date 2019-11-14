#ifndef __HELL_HORSE_H__
#define __HELL_HORSE_H__

#include "Enemy.h"

class Enemy_HellHorse : public Enemy
{
private:

public:

	Enemy_HellHorse();
	Enemy_HellHorse(int x, int y);
	~Enemy_HellHorse();

	void Move();
};

#endif // __HELL_HORSE_H__

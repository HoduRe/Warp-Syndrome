#ifndef __ELEMENTAL_H__
#define __ELEMENTAL_H__

#include "Enemy.h"

class Enemy_Elemental : public Enemy
{
public:

	Enemy_Elemental(int x, int y);
	~Enemy_Elemental();

	void Move();
};

#endif // __ELEMENTAL_H__

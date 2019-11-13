#include "j1App.h"
#include "Hell_Horse.h"
#include "j1Collision.h"

Enemy_HellHorse::Enemy_HellHorse() : Enemy() {}

Enemy_HellHorse::Enemy_HellHorse(int x, int y) : Enemy(x, y)
{
	//	fly.PushBack({ 5,6,24,24 });

	animation = &move;

	original_y = y;
}

Enemy_HellHorse::~Enemy_HellHorse() {}

void Enemy_HellHorse::Move() {

}

#include "j1App.h"
#include "Hell_Horse.h"
#include "j1Collision.h"

Enemy_HellHorse::Enemy_HellHorse(int x, int y) : Enemy(x, y)
{
	//	fly.PushBack({ 5,6,24,24 });

	animation = &fly;

	original_y = y;
}

void Enemy_HellHorse::Move() {

}

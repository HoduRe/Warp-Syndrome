#include "j1App.h"
#include "Elemental.h"
#include "j1Collision.h"

Enemy_Elemental::Enemy_Elemental(int x, int y) : Enemy(x, y)
{

	animation = &move;

	original_y = y;
}

Enemy_Elemental::~Enemy_Elemental() {}

void Enemy_Elemental::Move() {
	GeneralMove(&position.x, &position.y, path);
}

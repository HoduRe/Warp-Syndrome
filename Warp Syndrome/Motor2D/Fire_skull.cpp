#include "j1App.h"
#include "Fire_Skull.h"
#include "j1Collision.h"

Enemy_FireSkull::Enemy_FireSkull(int x, int y) : Enemy(x, y)
{
//	fly.PushBack({ 5,6,24,24 });
	
	animation = &fly;

	original_y = y;
}

void Enemy_FireSkull::Move() {

}

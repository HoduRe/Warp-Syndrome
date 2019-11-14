#include "j1App.h"
#include "j1EnemyManager.h"
#include "Elemental.h"
#include "j1Collision.h"

Enemy_Elemental::Enemy_Elemental(int x, int y) : Enemy(x, y)
{
	animations = *App->enemies->anim_item;
	//FERRAN note that you will have to enter a new parameter to this constructor (int x, int y, the animation list): Enemy(x,y, animationlist). you will have to do this the 3 child classes
}

Enemy_Elemental::~Enemy_Elemental() {}

void Enemy_Elemental::Move() {

}

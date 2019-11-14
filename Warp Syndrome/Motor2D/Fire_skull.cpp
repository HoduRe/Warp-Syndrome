#include "j1App.h"
#include "j1EnemyManager.h"
#include "Fire_Skull.h"
#include "j1Collision.h"

Enemy_FireSkull::Enemy_FireSkull() : Enemy(){}

Enemy_FireSkull::Enemy_FireSkull(int x, int y) : Enemy(x, y) {
	animations = *App->enemies->anim_item;
}

Enemy_FireSkull::~Enemy_FireSkull() {}

void Enemy_FireSkull::Move() {

}

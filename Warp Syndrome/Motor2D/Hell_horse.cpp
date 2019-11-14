#include "j1App.h"
#include "Hell_Horse.h"
#include "j1EnemyManager.h"
#include "j1Collision.h"

Enemy_HellHorse::Enemy_HellHorse() : Enemy() {}

Enemy_HellHorse::Enemy_HellHorse(int x, int y) : Enemy(x, y) {
	animations = *App->enemies->anim_item;
}

Enemy_HellHorse::~Enemy_HellHorse() {}

void Enemy_HellHorse::Move() {

}

#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"

Enemy::Enemy(){}

Enemy::Enemy(int x, int y) : position(x, y){
	enabled = false;
}

Enemy::~Enemy()
{}

void Enemy::Draw(SDL_Texture* sprites) {
//	if (animation != nullptr)
//		App->render->Blit(sprites, position.x, position.y, &(animation->GetFrame(0)));
}

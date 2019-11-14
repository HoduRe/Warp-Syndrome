#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"

Enemy::Enemy(){}

Enemy::Enemy(int x, int y){
	position.x = x;
	position.y = y;
	//FERRAN the animation is in the parent class, so you don't need to change all the constructors, only this one adding the animation. The animation list variable is in the .h
	enabled = false;
}

Enemy::~Enemy()
{}

void Enemy::Draw(SDL_Texture* sprites) {
//	if (animation != nullptr)
//		App->render->Blit(sprites, position.x, position.y, &(animation->GetFrame(0)));
}

void Enemy::Move() {

}

#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "j1Map.h"

Enemy::Enemy(EntityType atype): Character(atype)
{
	state = E_STATE_UNKNOWN;
}

Enemy::Enemy(int x, int y,enemy_states startingstate, EntityType atype):Character(atype)
{
	state = startingstate;
	last_state = E_STATE_UNKNOWN;
	pos.x = x;
	pos.y = y;
	enabled = false;
}

Enemy::~Enemy()
{}

void Enemy::Move(){}
void Enemy::Draw(){}
int Enemy::CheckDistance(float x, float y)
{
	iPoint mapCoords = App->map->WorldToMap(x, y, App->map->data);//convets the position to map coordinates (tile coordinates)

	return sqrt((App->entity_m->player->pos.x - mapCoords.x) * (App->entity_m->player->pos.x - mapCoords.x) +
		(App->entity_m->player->pos.y - mapCoords.y) * (App->entity_m->player->pos.y - mapCoords.y));
}

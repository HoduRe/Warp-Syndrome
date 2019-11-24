#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "j1Map.h"

Enemy::Enemy(int x, int y,enemy_states startingstate, EntityType atype):Character(atype)
{
	state = startingstate;
	anim_state = AS_UNKNOWN;
	last_state = E_STATE_UNKNOWN;
	pos.x = x;
	pos.y = y;
	enabled = false;
	player_distance = -1;
	chase_distance = -1;


	collider_type coll= collider_type::unknown_collider;
	switch (atype)
	{
	case E_TYPE_ELEMENTAL:
		coll = collider_type::enemy_elemental;
		break;
	case E_TYPE_FIRE_SKULL:
		coll = collider_type::enemy_skull;
		break;
	case E_TYPE_HELL_HORSE:
		coll = collider_type::enemy_horse;
		break;
	}
	collider = coll;
}

Enemy::~Enemy()
{}

void Enemy::Move(){}

void Enemy::Draw()
{
	SDL_Rect rect = currentframe->animationRect;
	App->render->Blit(texture, pos.x, pos.y, &rect, fliped);
}

int Enemy::CheckDistance(float x, float y)
{
	iPoint mapCoords = App->map->WorldToMap(x, y, App->map->data);//convets the position to map coordinates (tile coordinates)

	return sqrt((App->entity_m->player->pos.x - mapCoords.x) * (App->entity_m->player->pos.x - mapCoords.x) +
		(App->entity_m->player->pos.y - mapCoords.y) * (App->entity_m->player->pos.y - mapCoords.y));
}

void Enemy::CheckAnimation(enemy_states currentstate, enemy_states laststate){}
void Enemy::ChangeAnimation(){}

//enables the entity if its iside the camera rectangle
void Enemy::DoEnable()
{
	SDL_Rect cameraR = App->render->camera;
	cameraR.x *= -1;
	cameraR.y *= -1;
	iPoint tile_measures;
	tile_measures.x = App->map->data.tile_width;
	tile_measures.y = App->map->data.tile_height;


	
	if (pos.x >= cameraR.x - tile_measures.x &&//one tile margin for now
		pos.x <= cameraR.x + cameraR.w + tile_measures.x &&
		pos.y >= cameraR.y - tile_measures.y &&
		pos.y <= cameraR.y + cameraR.h + tile_measures.y
		)
	{
		enabled = true;
	}
	else enabled = false;
}

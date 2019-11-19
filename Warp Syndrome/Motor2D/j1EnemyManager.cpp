#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1EnemyManager.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Enemy.h"
#include "Hell_horse.h"
#include "Fire_skull.h"
#include "j1EntityManager.h"
#include "Player.h"

#define SPAWN_DISTANCE 25

j1EnemyManager::j1EnemyManager() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemy_list[i] = nullptr;
	}
}

j1EnemyManager::~j1EnemyManager() {}

bool j1EnemyManager::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->tex->Load("rtype/enemies.png");

	return true;
}

bool j1EnemyManager::PreUpdate()
{
	// Enables enemies
	iPoint player_pos;
	player_pos.x = App->entity_m->player->pos.x / App->map->data.tile_width;
	player_pos.y = App->entity_m->player->pos.y / App->map->data.tile_height;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (CheckDistance(enemy_list[i]->position.x, enemy_list[i]->position.y) <= SPAWN_DISTANCE) {
			enemy_list[i]->enabled = true;
			enemy_list[i]->path = App->pathfinding->CreatePath(player_pos, enemy_list[i]->position);
		}
	}

	return true;
}

// Called before render is available
bool j1EnemyManager::Update(float dt)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemy_list[i] != nullptr && enemy_list[i]->enabled == true) enemy_list[i]->Move();

	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemy_list[i] != nullptr && enemy_list[i]->enabled == true) enemy_list[i]->Draw(sprites);
	
	return true;
}

bool j1EnemyManager::PostUpdate()
{
	// Disables dead enemies
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (CheckDistance(enemy_list[i]->position.x, enemy_list[i]->position.y) > SPAWN_DISTANCE) {
			enemy_list[i]->enabled = false;
		}
	}

	return true;
}

// Called before quitting
bool j1EnemyManager::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);
	
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemy_list[i] != nullptr)
		{
			enemy_list[i]->path.Clear();
			delete enemy_list[i];
			enemy_list[i] = nullptr;
		}
	}
	
	return true;
}

void j1EnemyManager::AddEnemy(collider_type type, int x, int y) {
	switch (type) {
	case enemy_elemental:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr) {
				enemy_list[i] = new Enemy_Elemental(x, y);
			}
		}
		break;
	case enemy_horse:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr) {
				enemy_list[i] = new Enemy_HellHorse(x, y);
			}
		}
		break;
	case enemy_skull:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr) {
				enemy_list[i] = new Enemy_FireSkull(x, y);
			}
		}
		break;
	default:
		break;
	}
}

int j1EnemyManager::CheckDistance(int x, int y) {
	return sqrt((App->entity_m->player->pos.x - x) * (App->entity_m->player->pos.x - x) +
		(App->entity_m->player->pos.y - y) * (App->entity_m->player->pos.y - y));
}
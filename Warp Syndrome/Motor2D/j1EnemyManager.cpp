#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1EnemyManager.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "p2DynArray.h"
#include "Enemy.h"
#include "Hell_horse.h"
#include "Fire_skull.h"

#define SPAWN_DISTANCE 25

j1EnemyManager::j1EnemyManager() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemy_list[i] = nullptr;
	}
}

j1EnemyManager::~j1EnemyManager() {}

bool j1EnemyManager::Start() {
	return true;
}

bool j1EnemyManager::PreUpdate() {
	// Enables enemies
	iPoint player_pos;
	iPoint enemy_pos;
	int distance;
	p2DynArray<iPoint> aux_array;
	player_pos.x = App->player->GetPosition().x / App->map->data.tile_width;
	player_pos.y = App->player->GetPosition().y / App->map->data.tile_height;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy_list[i] != nullptr) {
			enemy_pos.x = enemy_list[i]->position.x / App->map->data.tile_width;
			enemy_pos.y = enemy_list[i]->position.y / App->map->data.tile_height;
			distance = CheckDistance(enemy_pos.x, enemy_pos.y);
			if (distance <= SPAWN_DISTANCE) {
				enemy_list[i]->enabled = true;
				App->pathfinding->CreatePath(player_pos, enemy_pos, enemy_list[i]);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1EnemyManager::Update(float dt) {
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemy_list[i] != nullptr && enemy_list[i]->enabled == true)
			enemy_list[i]->Move();

//	for(uint i = 0; i < MAX_ENEMIES; ++i)
//BLIT	
	return true;
}

bool j1EnemyManager::PostUpdate() {
	// Disables dead enemies
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy_list[i] != nullptr && CheckDistance(enemy_list[i]->position.x / App->map->data.tile_width,
			enemy_list[i]->position.y / App->map->data.tile_width) > SPAWN_DISTANCE) {
			enemy_list[i]->enabled = false;
		}
	}
	BlitEnemies();

	return true;
}

// Called before quitting
bool j1EnemyManager::CleanUp() {
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
				i = MAX_ENEMIES;
			}
		}
		break;
	case enemy_horse:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr) {
				enemy_list[i] = new Enemy_HellHorse(x, y);
				i = MAX_ENEMIES;
			}
		}
		break;
	case enemy_skull:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr) {
				enemy_list[i] = new Enemy_FireSkull(x, y);
				i = MAX_ENEMIES;
			}
		}
		break;
	default:
		break;
	}
}

int j1EnemyManager::CheckDistance(int x, int y) {
	iPoint playerpos;
	playerpos.x = App->player->GetPosition().x / App->map->data.tile_width - x;
	playerpos.y = App->player->GetPosition().y / App->map->data.tile_height - y;
	if (playerpos.x < 0) { playerpos.x = -playerpos.x; }
	if (playerpos.y < 0) { playerpos.y = -playerpos.y; }
	return playerpos.x + playerpos.y;
}

void j1EnemyManager::SetBlitEnemies() { blit = !blit; }

void j1EnemyManager::BlitEnemies() {
	SDL_Rect rect;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy_list[i] != nullptr && enemy_list[i]->enabled == true) {
			int j = 0;
			rect.x = enemy_list[i]->position.x;
			rect.y = enemy_list[i]->position.y;
			rect.w = 20;
			rect.h = 20;
			App->render->DrawQuad(rect, 75, 75, 75);
			while (enemy_list[i]->path.At(j) != NULL) {
				rect.w = App->map->data.tile_width;
				rect.h = App->map->data.tile_height;
				rect.x = enemy_list[i]->path[j].x * rect.w;
				rect.y = enemy_list[i]->path[j].y * rect.h;
				App->render->DrawQuad(rect, 175, 175, 175, 60);
				j++;
			}
		}
	}
}

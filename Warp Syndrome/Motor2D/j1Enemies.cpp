#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Enemies.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Enemy.h"


#define SPAWN_MARGIN 50

j1Enemies::j1Enemies()
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->tex->Load("rtype/enemies.png");

	return true;
}

bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if(queue[i].x < App->render->camera.x + App->render->camera.w + SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Enemies::Update(float dt)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemies[i] != nullptr) enemies[i]->Move();

	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemies[i] != nullptr) enemies[i]->Draw(sprites);

	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			if(enemies[i]->position.x < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}

	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for(; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if(i != MAX_ENEMIES)
	{
		switch(info.type)
		{
		default:
			break;
		}
	}
}
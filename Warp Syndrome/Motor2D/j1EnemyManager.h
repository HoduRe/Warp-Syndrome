#ifndef __J1ENEMYMANAGER_H__
#define __J1ENEMYMANAGER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "Hell_horse.h"
#include "Fire_skull.h"
#include "Elemental.h"

#define MAX_ENEMIES 40

class Enemy;

class j1EnemyManager : public j1Module
{
public:

	j1EnemyManager();
	~j1EnemyManager();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Adds an enemy to the array
	void AddEnemy(collider_type type, int x, int y);

	// Checks distance between player and enemy
	int CheckDistance(int x, int y);

private:

	Enemy *enemy_list[MAX_ENEMIES];
	SDL_Texture* sprites;
};

#endif // __J1ENEMYMANAGER_H__
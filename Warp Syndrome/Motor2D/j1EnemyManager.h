#ifndef __J1ENEMYMANAGER_H__
#define __J1ENEMYMANAGER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "Hell_horse.h"
#include "Fire_skull.h"
#include "Elemental.h"

#define MAX_ENEMIES 40

class Enemy;

struct EnemyAnimations
{
	int enemy_type; //when comparing use the enum collider_type
	p2List<Animations> enemy_animations;//FERRAN this is the list with all the animations, feel free to change any variable to pointer or to normal variable if you think its necessary
};

class j1EnemyManager : public j1Module
{
public:

	j1EnemyManager();
	~j1EnemyManager();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Adds an enemy to the array
	void AddEnemy(collider_type type, int x, int y);

	// Checks distance between player and enemy
	int CheckDistance(int x, int y);

	//Loads aniamtions from node
	bool LoadAnimations(pugi::xml_node& rootnode, p2List_item<EnemyAnimations*>* animlist);

private:
	p2List<EnemyAnimations*> enemies_animation_list; //FERRAN this is the list whith all the data, there's one item for enemy, each containing the enemy id and a list of animations for that enemy, it is loaded when starting the program. Loads Perfectly
	Enemy *enemy_list[MAX_ENEMIES];
	SDL_Texture* enemies_texture;
	p2SString filename;
	pugi::xml_document enemiesdoc;
	pugi::xml_node enemiesnode;
};

#endif // __J1ENEMYMANAGER_H__
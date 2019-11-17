#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__
#include "j1App.h"
#include "j1Module.h"
#include "p2List.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "SDL/include/SDL.h"
#include "j1Textures.h"
#include "Entity.h"
#include "Player.h"


//Manages all entities in the game
class j1EntityManager :public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);

public:
	Player* player;
private:
	p2List<Entity*> entity_list;
	//Player* player; //TODO when created the player class assign this pointer to the player
};



#endif // !__ENTITY_MANAGER_H__


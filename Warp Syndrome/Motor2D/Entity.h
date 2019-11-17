#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "SDL/include/SDL.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "j1Textures.h"

enum EntityType
{
	E_TYPE_PLAYER,
	E_TYPE_PARTICLE,
	E_TYPE_ANIMATED_PARTICLE,
	E_TYPE_GRENEDE,
	E_TYPE_ELEMENTAL,
	E_TYPE_FIRE_SKULL,
	E_TYPE_HELL_HORSE,
	E_TYPE_UNKNOWN
};


class Entity
{
public:
	Entity(EntityType eType);
	virtual ~Entity();

	virtual bool Awake();
	virtual bool Start();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	virtual bool CleanUp();
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;
	void Enable();
	void Disable();

public:

	fPoint pos;
	fPoint speed;
	EntityType type;
	SDL_Texture* texture;
	SDL_Rect texture_section; //rectangle which will be displayed from the texture

	bool fliped;
	bool destroy;//if true, the entity manager will know it has to destroy the entity
	bool enabled; //certain functions will only be done if the entity is enabled
	float health;//entity health/lifespan
};
#endif // !__ENTITY_H__

#include "j1EntityManager.h"
#include "Entity.h"
#include "Player.h"

//Entity Manager=============================================

j1EntityManager::j1EntityManager()
{
	name.create("player");//TODO it now takes the playernode from the entity manager, we won't be doing that in the future
}
j1EntityManager::~j1EntityManager()
{
	entity_list.clear();
}

bool j1EntityManager::Awake(pugi::xml_node& node)
{
	player = (Player*)CreateEntity(EntityType::E_TYPE_PLAYER);
	player->Awake(node);
	return true;
}
bool j1EntityManager::Start()
{
	return true;
}
bool j1EntityManager::PreUpdate(float dt)
{
	return true;
}
bool j1EntityManager::Update(float dt)
{
	return true;
}
bool j1EntityManager::PostUpdate(float dt)
{
	return true;
}
bool j1EntityManager::CleanUp()
{
	return true;
}
bool j1EntityManager::Load(pugi::xml_node&)
{
	return true;
}
bool j1EntityManager::Save(pugi::xml_node&) const
{
	return true;
}

Entity* j1EntityManager::CreateEntity(EntityType type)
{
	static_assert(EntityType::E_TYPE_UNKNOWN == 7, "code needs update");
	Entity* ret = nullptr;

	
	switch (type)
	{
	case EntityType::E_TYPE_PLAYER:
		ret = new Player();
		break;
		//TODO uncomment this once all the entity types are created
	/*case EntityType::E_TYPE_PARTICLE:
		ret = new Particle();
		break;
	case EntityType::E_TYPE_ANIMATED_PARTICLE:
		ret = new AnimatedParticle();
		break;
	case EntityType::E_TYPE_GRENEDE:
		ret = new Grenade();
		break;
	case EntityType::E_TYPE_ELEMENTAL:
		ret = new Elemental();
		break;
	case EntityType::E_TYPE_FIRE_SKULL:
		ret = new FireSkull();
		break;
	case EntityType::E_TYPE_HELL_HORSE:
		ret = new HellHorse();
		break;*/
	}


	if (ret != nullptr)
		entity_list.add(ret);
	return ret;
}
//destroys an entity from the list
void j1EntityManager::DestroyEntity(Entity* entity)
{
	p2List_item<Entity*>*item=nullptr;
	int index = entity_list.find(entity);//if the entity is in the list
	if (index != -1)
	{
		item = entity_list.At(index);
		entity_list.del(item);
	}
}

#include "j1EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Particles.h"
#include "j1Grenade.h"

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
	grenade = nullptr;
	player->Awake(node);
	return true;
}
bool j1EntityManager::Start()
{
	gravity = { 0.0f,490.0f };//TODO ASSIGN THIS FROM AN XML
	player->Start();
	return true;
}
bool j1EntityManager::Start(fPoint aGravity)
{
	gravity = aGravity;
	return true;
}
bool j1EntityManager::PreUpdate()
{
	p2List_item<Entity*>* item;
	//deletes all the death entities=================================================
	if (entity_list.count() > 0)//if there are entities in the list
	{
		item = entity_list.start;
		while (item != NULL)
		{
			if (item->data->destroy == true)
			{
				entity_list.del(item);
			}

			item = item->next;
		}

	}

	//updates all the entities=======================================================
	item = entity_list.start;
	while (item != NULL)
	{
		item->data->PreUpdate();
		item = item->next;
	}
	return true;
}
bool j1EntityManager::Update(float dt)
{
	//updates all the entities=======================================================
	p2List_item<Entity*>* item = entity_list.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		item = item->next;
	}
	return true;
}
bool j1EntityManager::PostUpdate()
{
	//updates all the entities=======================================================

	p2List_item<Entity*>* item = entity_list.start;
	while (item != NULL)
	{
		item->data->PostUpdate();
		item = item->next;
	}
	return true;
	return true;
}
bool j1EntityManager::CleanUp()
{
	entity_list.clear();
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

//creates entity from type and adds it to the list (this doesn't work for particles due to their customizable nature):
//to add a particle to the entity list create the particle and use the entity manager method AddEntity().
Entity* j1EntityManager::CreateEntity(EntityType type)
{
	static_assert(EntityType::E_TYPE_UNKNOWN == 9, "code needs update");
	Entity* ret = nullptr;


	switch (type)
	{
	case EntityType::E_TYPE_PLAYER:
		ret = new Player();
		break;
		
	case EntityType::E_TYPE_PARTICLE:
		assert("tried to create a particle from scratch"); //TODO delete this when going to do the release(including the case)
		break;
	case EntityType::E_TYPE_ANIMATED_PARTICLE:
		assert("tried to create a particle from scratch"); //TODO delete this when going to do the release(including the case)
		break;
	case EntityType::E_TYPE_GRENADE:
		ret = new Grenade();
		break;
	/*
	case EntityType::E_TYPE_ELEMENTAL:
		ret = new Elemental();
		break;
	case EntityType::E_TYPE_FIRE_SKULL:
		ret = new FireSkull();
		break;
	case EntityType::E_TYPE_HELL_HORSE:
		ret = new HellHorse();
		break;*/
		//TODO uncomment this once all the entity types are created
	}


	if (ret != nullptr)
		entity_list.add(ret);
	return ret;

}

//adds an existing entity to the list (mainly used for particles due to their customizable nature)
bool j1EntityManager::AddEntity(Entity* newEntity)
{
	bool ret = false;
	static_assert(EntityType::E_TYPE_UNKNOWN == 9, "code needs update");
	entity_list.add(newEntity);
	return ret;
}

//destroys a certain entity from the list (returns false if the entity in not in the list)
bool j1EntityManager::DestroyEntity(Entity* entity)
{
	bool ret = false;
	p2List_item<Entity*>* item = entity_list.start;
	while (item!=NULL)
	{
		if (item->data==entity)
		{
			entity_list.del(item);
			ret = true;
			break;
		}
		item = item->next;
	}
	return ret;
}

bool  j1EntityManager::ClearEntitiesOfType(EntityType eType)
{
	bool ret = false;
	p2List_item<Entity*>* item = entity_list.start;
	while (item != NULL)
	{
		if (item->data->type == eType)
		{
			entity_list.del(item);
			ret = true;
			break;
		}
		item = item->next;
	}
	return ret;
}

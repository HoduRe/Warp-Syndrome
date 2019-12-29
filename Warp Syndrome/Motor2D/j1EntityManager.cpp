#include "j1EntityManager.h"
#include "Entity.h"
#include "Elemental.h"
#include "FireSkull.h"
#include "Player.h"
#include "Particles.h"
#include "j1Grenade.h"
#include "Coin_G.h"
#include "j1Scene.h"

//Entity Manager=============================================

j1EntityManager::j1EntityManager()
{
	name.create("entity_m");//TODO it now takes the playernode from the entity manager, we won't be doing that in the future

}
j1EntityManager::~j1EntityManager()
{
	CleanUp();
}

bool j1EntityManager::Awake(pugi::xml_node& node)
{
	player = (Player*)CreateEntity(EntityType::E_TYPE_PLAYER);
	grenade = nullptr;
	kill = false;
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
	player->Start();

	return true;
}
bool j1EntityManager::PreUpdate()
{
	if (App->dt != 0.0f) {
		p2List_item<Entity*>* item;
		p2List_item<Entity*>* p_item;
		//deletes all the death entities=================================================
		if (entity_list.count() > 0)//if there are entities in the list
		{
			item = entity_list.start;
			while (item != nullptr)
			{
				if (item->data->destroy == true)
				{
					item->data->CleanUp();
					RELEASE(item->data);
					entity_list.del(item);
					item = item->prev;
				}
				else item = item->next;
			}

		}

		//updates all the entities=======================================================
		item = entity_list.start;
		while (item != NULL)
		{
			item->data->PreUpdate();
			item = item->next;
		}
	}
	return true;
}
bool j1EntityManager::Update(float dt)
{
	if (dt != 0.0f) {
		//updates all the entities=======================================================
		p2List_item<Entity*>* item = entity_list.start;
		while (item != NULL)
		{
			item->data->Update(dt);
			item = item->next;
		}
	}
	return true;
}
bool j1EntityManager::PostUpdate()
{

	//updates all the entities=======================================================

	p2List_item<Entity*>* item = entity_list.start;
	while (item != NULL)
	{
		if(App->scene->draw)item->data->PostUpdate();
		item = item->next;
	}

	return true;
}
bool j1EntityManager::CleanUp()
{
	p2List_item<Entity*>* item = entity_list.start;
	while (item != NULL)
	{
		if (item->data->type != EntityType::E_TYPE_PLAYER)
		{
			item->data->CleanUp();
			RELEASE(item->data);
			entity_list.del(item);
			item = item->prev;
		}
		else item = item->next;

	}
	//deletes the player
	item = entity_list.start;
	item->data->CleanUp();
	RELEASE(item->data);
	entity_list.del(item);

	entity_list.clear();
	player = nullptr;
	grenade = nullptr;
	return true;
}
bool j1EntityManager::CleanAllEntites()//except for the player
{
	p2List_item<Entity*>* item = entity_list.start;
	while (item != nullptr)
	{
		if (item->data->type != E_TYPE_PLAYER)
		{
			item->data->CleanUp();
			RELEASE(item->data);
			entity_list.del(item);
			item = item->prev;
		}
		else item = item->next;
	}
	return true;
}
bool j1EntityManager::Load(pugi::xml_node& ldata)
{
	//clears all the enemies before adding more
	CleanAllEntites();

	pugi::xml_node node = ldata.child("Entity");
	while (node != NULL)
	{
		//constructing an enemy from the xml
		Entity* entity = nullptr;
		iPoint entitypos;
		float entityhealth = 0.0f;
		int entitystartingstate = E_STATE_UNKNOWN;

		entitypos.x = node.child("pos").attribute("x").as_float();
		entitypos.y = node.child("pos").attribute("y").as_float();
		entityhealth = node.child("properties").attribute("health").as_float();
		entitystartingstate = node.child("properties").attribute("state").as_int();

		//------------------------------------------
		//adding the enemy
		int type = node.attribute("type").as_int();
		switch (type)
		{
		case E_TYPE_PLAYER:
			player->pos.x = node.child("pos").attribute("x").as_float();
			player->pos.y = node.child("pos").attribute("y").as_float();
			player->current_state = state_list(entitystartingstate);
			player->Load(node.child("properties"));
			break;
		case E_TYPE_ELEMENTAL:
			entity = new Enemy_Elemental(entitypos.x, entitypos.y, enemy_states(entitystartingstate), entityhealth);
			AddEntity(entity);//TODO: entity

			break;
		case E_TYPE_FIRE_SKULL:
			entity = new Enemy_FireSkull(entitypos.x, entitypos.y, enemy_states(entitystartingstate), entityhealth);
			//entity.last_state = enemy_states(node.child("properties").attribute("last_state").as_int()); //TODO ??
			AddEntity(entity);//TODO: entity
			break;
		case E_TYPE_HELL_HORSE:
			//TODO load
			break;

		case E_TYPE_COIN_G:
			entity = new Coin_G({ (float)entitypos.x, (float)entitypos.y });
			AddEntity(entity);
			break;
		}
		node = node.next_sibling("Entity");
	}
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& ldata) const //saves all the entities to the saves doc
{
	for (pugi::xml_node n = ldata.first_child(); n; n.next_sibling())//removes all the saved entites before oerwritting them
	{
		ldata.remove_child("Entity");
	}

	p2List_item<Entity*>* entity = entity_list.start;
	while (entity != NULL)
	{
		SaveEntity(ldata, entity->data);
		entity = entity->next;
	}


	return true;
}

bool j1EntityManager::SaveEntity(pugi::xml_node& ldata, Entity* enty) const//saves a single entity
{
	pugi::xml_node entity_n;
	pugi::xml_node pos_n;
	pugi::xml_node props_n;


	entity_n = ldata.append_child("Entity");
	pos_n = entity_n.append_child("pos");
	props_n = entity_n.append_child("properties");

	entity_n.append_attribute("type") = enty->type; //saves the type
	pos_n.append_attribute("x") = enty->pos.x; //saves posX
	pos_n.append_attribute("y") = enty->pos.y; //saves posY
	props_n.append_attribute("health") = enty->health;
	enty->Save(props_n);
	return true;
}

//creates entity from type and adds it to the list (this doesn't work for particles due to their customizable nature):
//to add a particle to the entity list create the particle and use the entity manager method AddEntity().
Entity* j1EntityManager::CreateEntity(EntityType type)
{
	static_assert(EntityType::E_TYPE_UNKNOWN == 10, "code needs update");
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
	static_assert(EntityType::E_TYPE_UNKNOWN == 10, "code needs update");
	entity_list.add(newEntity);
	return ret;
}

//destroys a certain entity from the list (returns false if the entity in not in the list)
bool j1EntityManager::DestroyEntity(Entity* entity)
{
	bool ret = false;
	p2List_item<Entity*>* item = entity_list.start;
	while (item != NULL)
	{
		if (item->data == entity)
		{
			RELEASE(item->data);
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
		}
		item = item->next;
	}
	return ret;
}

bool  j1EntityManager::RespawnEntitiesOfType(EntityType eType)
{
	p2List_item<ObjectGroup*>* objectgroupitem = App->map->data.objgroups.start;

	while (objectgroupitem != NULL)//double while that iterates between every objectgroup and every object in them
	{
		p2List_item<Object*>* objectlistitem = objectgroupitem->data->objlist.start;

		while (objectlistitem != NULL)
		{
			if (objectlistitem->data->type == eType)
			{
				Entity* entity = nullptr;
				SDL_Rect rect = objectlistitem->data->boundingbox;
				switch (objectlistitem->data->type)
				{
				case 6:
					entity = new Enemy_Elemental(rect.x + (rect.w * 0.5f), rect.y + (rect.h * 0.5f));
					AddEntity(entity); //TODO: entity
					//delete enemy;
					break;
				case 7:
					//App->enemies->AddEnemy(enemy_horse, set->boundingbox.x, set->boundingbox.y);//TODO add enemie horse here
					break;
				case 8:
					entity = new Enemy_FireSkull(rect.x + (rect.w * 0.5f), rect.y + (rect.h * 0.5f));
					AddEntity(entity); //TODO: entity
					break;
				case 9:
					entity = new Coin_G({ (float)rect.x + (rect.w * 0.5f), (float)rect.y + (rect.h * 0.5f) });
					AddEntity(entity);
					break;
				default:
					break;
				}
			}
			objectlistitem = objectlistitem->next;
		}

		objectgroupitem = objectgroupitem->next;
	}
	return true;
}


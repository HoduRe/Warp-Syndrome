#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1EnemyManager.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Enemy.h"
#include "Hell_horse.h"
#include "Fire_skull.h"

#define SPAWN_DISTANCE 25

j1EnemyManager::j1EnemyManager() {
	//creates the name of the class for usage to target the right node of coniguration in the awake function
	name.create("enemies");

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemy_list[i] = nullptr;
	}
}

j1EnemyManager::~j1EnemyManager() {}

bool j1EnemyManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	//gets the file name of the enemies document from the config document
	filename.create(config.child("load").attribute("docname").as_string());
	//loads the enemies document
	pugi::xml_parse_result result = enemiesdoc.load_file(filename.GetString());

	if (result == NULL)
	{
		LOG("Could not load enemies document. pugi error: %s", result.description());
		ret = false;
	}

	//initializes enemiesnode to be the root node of the doc
	enemiesnode = enemiesdoc.child("enemies");

	//loads the spritesheet texture
	pugi::xml_node texturenode = enemiesnode.child("texture");
	enemies_texture = App->tex->Load(PATH(texturenode.child("folder").text().as_string(), texturenode.child("load").attribute("texturename").as_string()));

	pugi::xml_node single_enemy_node = texturenode.next_sibling().first_child();
	while (single_enemy_node != NULL)
	{
		EnemyAnimations* item = new EnemyAnimations();

		item->enemy_type = single_enemy_node.attribute("enemy_type").as_int();
		p2List_item<EnemyAnimations*>* enemy_item = enemies_animation_list.add(item);
		LoadAnimations(single_enemy_node, enemy_item);

		single_enemy_node = single_enemy_node.next_sibling();
	}
	return ret;
}
bool j1EnemyManager::Start()
{
	bool ret = true;

	return ret;
}

bool j1EnemyManager::PreUpdate()
{
	// Enables enemies
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy_list[i]!=nullptr&&CheckDistance(enemy_list[i]->position.x, enemy_list[i]->position.y) <= SPAWN_DISTANCE) {
			enemy_list[i]->enabled = true;
		}
	}

	return true;
}

// Called before render is available
bool j1EnemyManager::Update(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemy_list[i] != nullptr && enemy_list[i]->enabled == true) enemy_list[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemy_list[i] != nullptr && enemy_list[i]->enabled == true) enemy_list[i]->Draw(enemies_texture);

	//for (uint i = 0; i < MAX_ENEMIES; ++i)
		//if (enemy_list[i] != nullptr && enemy_list[i]->enabled == false) enemy_list[1]->animations.start->data.AddFrame(1, { 60,12,21,4 }, {0,0}); //FERRAN I added those 2 lines of code for debugging purposes, it adds a frame to every animation, if you put a debug pointer here you can see if only the first animation on the list changes or every enemy animation does

	return true;
}

bool j1EnemyManager::PostUpdate()
{
	// Disables dead enemies
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemy_list[i]!=nullptr&&CheckDistance(enemy_list[i]->position.x, enemy_list[i]->position.y) > SPAWN_DISTANCE) {
			enemy_list[i]->enabled = false;
		}
	}

	return true;
}

// Called before quitting
bool j1EnemyManager::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(enemies_texture);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemy_list[i] != nullptr)
		{
			delete enemy_list[i];
			enemy_list[i] = nullptr;
		}
	}

	return true;
}

void j1EnemyManager::AddEnemy(collider_type type, int x, int y) {


	//first loads the correct animation from the list
	p2List_item<EnemyAnimations*>* item = enemies_animation_list.start;
	anim_item = nullptr;
	while (item != NULL)
	{
		if (item->data->enemy_type == type)//copies the animation list for this current enemy
			anim_item = &item->data->enemy_animations;//FERRAN anim_item is the correct p2_list that we are looking for (i tried to put this as a parameter when constructing the object, didn't work :C)
		item = item->next;
	}
	switch (type) {
	case enemy_elemental:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr && anim_item != NULL) {
				enemy_list[i] = new Enemy_Elemental(x, y);//FERRAN Enter a new parameter to the constructor, with the correct animation list
				break;

			}
		}
		break;
	case enemy_horse:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr && anim_item != NULL) {
				enemy_list[i] = new Enemy_HellHorse(x, y);//FERRAN Enter a new parameter to the constructor, with the correct animation list
				break;

			}
		}
		break;
	case enemy_skull:
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemy_list[i] == nullptr && anim_item != NULL) {
				enemy_list[i] = new Enemy_FireSkull(x, y);//FERRAN Enter a new parameter to the constructor, with the correct animation list
				break;

			}
		}
		break;
	default:
		break;
	}


}

int j1EnemyManager::CheckDistance(int x, int y) {
	return sqrt((App->player->GetPosition().x / App->map->data.tile_height - x / App->map->data.tile_height) * (App->player->GetPosition().x / App->map->data.tile_height - x / App->map->data.tile_height) +
		(App->player->GetPosition().y / App->map->data.tile_height - y / App->map->data.tile_height) * (App->player->GetPosition().y / App->map->data.tile_height - y / App->map->data.tile_height));
}

//imput the enemy node and the list you 
bool j1EnemyManager::LoadAnimations(pugi::xml_node& rootnode, p2List_item<EnemyAnimations*>* animlist)
{
	bool ret = false;
	//Load all the animations from a single enemy
	for (pugi::xml_node animationnode = rootnode.child("animation"); animationnode; animationnode = animationnode.next_sibling("animation"))
	{
		Animations* anim = new Animations;
		ret = anim->LoadAnim(animationnode);

		animlist->data->enemy_animations.add(*anim);//TODO charge the animations directly into each enemy instead of a list
	}
	return ret;
}

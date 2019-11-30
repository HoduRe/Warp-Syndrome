#include "j1App.h"
#include "p2Defs.h"

#include "p2Log.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1PathFinding.h"
#include "Entity.h"
#include "j1Map.h"


Enemy::Enemy(int x, int y, enemy_states startingstate, EntityType atype) :Character(atype)
{
	state = startingstate;
	anim_state = AS_UNKNOWN;
	last_state = E_STATE_UNKNOWN;
	pos.x = x;
	pos.y = y;
	enabled = false;
	player_distance = -1;
	chase_distance = -1;
	ground_distance = App->map->data.height;
	jump = false;

	//Loads the animations and properties
	filename = "enemies.xml";//TODO load this from config
	pugi::xml_parse_result result = enemiesdoc.load_file(filename.GetString());

	if (result == NULL)
		LOG("Could not load player documant. pugi error: %s", result.description());

	enemiesnode = enemiesdoc.child("enemies");



	pugi::xml_node current_enemy_node;

	collider_type coll = collider_type::unknown_collider;
	switch (atype)
	{
	case E_TYPE_ELEMENTAL:
		coll = collider_type::enemy_elemental;
		current_enemy_node = enemiesnode.child("elemental");
		break;
	case E_TYPE_FIRE_SKULL:
		coll = collider_type::enemy_skull;
		current_enemy_node = enemiesnode.child("fire_skull");

		break;
	case E_TYPE_HELL_HORSE:
		coll = collider_type::enemy_horse;
		current_enemy_node = enemiesnode.child("hell_horse");

		break;
	}
	collider = coll;
	LoadAnimations(current_enemy_node);
	pugi::xml_node tmp = enemiesnode.child("texture");
	const pugi::char_t* folder = tmp.child("folder").text().as_string();
	const pugi::char_t* texturename = tmp.child("load").append_attribute("texturename").as_string();
	texture = App->tex->Load(PATH(tmp.child("folder").text().as_string(), tmp.child("load").attribute("texturename").as_string()));

	p2List_item<Animations*>* defaultanim = animations_list.start->data->GetAnimFromName("idle", &animations_list);
	currentAnim = defaultanim;
	enemiesdoc.reset();
}

Enemy::~Enemy()
{
	this->CleanUp();

}

bool Enemy::CleanUp()
{
	App->tex->UnLoad(texture);
	enemiesdoc.reset();
	return true;
}

void Enemy::Move(float dt)
{
	//TODO enemy movement logic here
	int width = App->map->data.tile_width;
	int height = App->map->data.tile_height;
	int current_pos = pos.y / height;

	if (path.At(1) != nullptr) {
		if (path.At(0)->x != path.At(1)->x) {
			int current_pos = pos.x / App->map->data.tile_width;
			if (path.At(0)->x < path.At(1)->x && path.At(path.Count() - 1)->x != current_pos) {
				(pos.x) += (width)* dt;
			}
			else if (path.At(0)->x > path.At(1)->x && path.At(path.Count() - 1)->x != current_pos) {
				(pos.x) -= (width)* dt;
			}
		}
		else if (path.At(0)->y != path.At(1)->y) {
			switch (type) {
			case E_TYPE_ELEMENTAL:
			case E_TYPE_HELL_HORSE:
				if (jump == false && ground_distance != path.At(1)->y && path.At(path.Count() - 1)->y != current_pos) {
					(pos.y) += (height)* dt;
				}
				else if (path.At(0)->y > path.At(1)->y && path.At(path.Count() - 1)->y != current_pos) {
					(pos.y) -= (height)* dt;
					jump = true;
				}
				break;
			default:
				if (path.At(0)->y < path.At(1)->y && path.At(path.Count() - 1)->y != current_pos) {
					(pos.y) += (height)* dt;
				}
				else if (path.At(0)->y > path.At(1)->y && path.At(path.Count() - 1)->y != current_pos) {
					(pos.y) -= (height)* dt;
				}
				break;
			}
		}
		if (pos.y > App->entity_m->player->pos.y && current_pos == ground_distance - 1) { jump = true; }
		if (ground_distance - 5 == current_pos || (current_pos != ground_distance - 1 && path.At(0)->x != path.At(1)->x)) { jump = false; }
	}
	if (jump == false && ground_distance - 1 > current_pos) { (pos.y) += (height)* dt; }
}

void Enemy::Draw()
{
	SDL_Rect rect = currentframe->animationRect;
	App->render->Blit(texture, pos.x, pos.y, &rect, fliped);
	if (App->entity_m->player->GetBlitColliders() == true) { BlitEnemiesLogic(); }
}

int Enemy::CheckDistance(float x, float y)
{
	iPoint mapCoords = App->map->WorldToMap(x, y, App->map->data);//convets the position to map coordinates (tile coordinates)
	iPoint playerCoords = App->map->WorldToMap(App->entity_m->player->pos.x, App->entity_m->player->pos.y, App->map->data);
	return sqrt((playerCoords.x - mapCoords.x) * (playerCoords.x - mapCoords.x) +
		(playerCoords.y - mapCoords.y) * (playerCoords.y - mapCoords.y));
}

void Enemy::CheckAnimation(enemy_states currentstate, enemy_states laststate) {}
void Enemy::ChangeAnimation() {}

//enables the entity if its iside the camera rectangle
void Enemy::DoEnable()
{
	SDL_Rect cameraR = App->render->camera;
	cameraR.x *= -1;
	cameraR.y *= -1;
	iPoint tile_measures;
	tile_measures.x = App->map->data.tile_width;
	tile_measures.y = App->map->data.tile_height;



	if (pos.x >= cameraR.x - tile_measures.x &&//one tile margin for now
		pos.x <= cameraR.x + cameraR.w + tile_measures.x &&
		pos.y >= cameraR.y - tile_measures.y &&
		pos.y <= cameraR.y + cameraR.h + tile_measures.y
		)
	{
		enabled = true;
	}
	else enabled = false;
}

void Enemy::DoPathFinding() {
	iPoint player_pos;
	iPoint enemy_pos;
	player_pos.x = App->entity_m->player->pos.x / App->map->data.tile_width;
	player_pos.y = App->entity_m->player->pos.y / App->map->data.tile_height;
	enemy_pos.x = pos.x / App->map->data.tile_width;
	enemy_pos.y = pos.y / App->map->data.tile_height;
	App->pathfinding->CreatePath(enemy_pos, player_pos, this);
}

void Enemy::BlitEnemiesLogic() {
	SDL_Rect rect = texture_section;
	int j = 0;
	App->render->DrawQuad(rect, 75, 75, 75);
	while (path.At(j) != NULL) {
		rect.w = App->map->data.tile_width;
		rect.h = App->map->data.tile_height;
		rect.x = path[j].x * rect.w;
		rect.y = path[j].y * rect.h;
		App->render->DrawQuad(rect, 175, 175, 175, 60);
		j++;
	}
}

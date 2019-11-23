#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "Entity.h"

Enemy::Enemy(EntityType atype): Character(atype)
{
	state = E_STATE_UNKNOWN;
}

Enemy::Enemy(int x, int y, EntityType atype):Character(atype)
{
	state = E_STATE_DEFAULT;
	pos.x = x;
	pos.y = y;
	enabled = false;
}

Enemy::~Enemy()
{}

void Enemy::GeneralMove(int *x, int *y, p2DynArray<iPoint>& path) {

	int width = App->map->data.tile_width;
	int height = App->map->data.tile_height;

	if (path.At(path.Count())->x != path.At(path.Count() - 1)->x) {
		if (path.At(path.Count())->x < path.At(path.Count() - 1)->x) {
			(*x) += width / 4;
		}
		else if (path.At(path.Count())->x > path.At(path.Count() - 1)->x) {
			(*x) -= width / 4;
		}
	}
	else if (path.At(path.Count())->x != path.At(path.Count() - 2)->x) {
		if (path.At(path.Count())->x < path.At(path.Count() - 2)->x) {
			(*x) += width / 4;
		}
		else if (path.At(path.Count())->x > path.At(path.Count() - 2)->x) {
			(*x) -= width / 4;
		}
	}	// This second comprovation is done in order to make movement more dynamic, and less rigid

	if (path.At(path.Count())->y != path.At(path.Count() - 1)->y) {
		if (path.At(path.Count())->y < path.At(path.Count() - 1)->y) {
			(*y) += height / 4;
		}
		else if (path.At(path.Count())->y > path.At(path.Count() - 1)->y) {
			(*y) -= height / 4;
		}
	}
	else if (path.At(path.Count())->y != path.At(path.Count() - 2)->y) {
		if (path.At(path.Count())->y > path.At(path.Count() - 2)->y) {
			(*y) -= height / 4;
		}
	}	// This second comprovation is done in order to make movement more dynamic, and less rigid
}
int Enemy::CheckDistance(int x, int y)
{
	return sqrt((App->entity_m->player->pos.x - x) * (App->entity_m->player->pos.x - x) +
		(App->entity_m->player->pos.y - y) * (App->entity_m->player->pos.y - y));
}

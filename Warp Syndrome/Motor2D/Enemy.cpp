#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"

Enemy::Enemy(){}

Enemy::Enemy(int x, int y) : position(x, y){
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

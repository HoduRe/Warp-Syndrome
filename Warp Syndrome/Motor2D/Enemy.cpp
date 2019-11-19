#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "Particles.h"
#include "j1Render.h"
#include "p2Log.h"

Enemy::Enemy(){}

Enemy::Enemy(int x, int y) : position(x, y){
	enabled = false;
}

Enemy::~Enemy()
{}

void Enemy::GeneralMove(int *x, int *y, p2DynArray<iPoint>& path) {

	int i = 0;
	while (path.At(i) != NULL) {
		iPoint a;
		a.x = path.At(i)->x;
		a.y = path.At(i)->y;
		LOG("%i: %i %i", i, a.x, a.y);
		i++;
	}

	int width = App->map->data.tile_width;
	int height = App->map->data.tile_height;
	int count = path.Count()-1;

	if (path.At(count)->x != path.At(count-1)->x) {
		if (path.At(count)->x < path.At(count-1)->x) {
			(*x) += width / 4;
		}
		else if (path.At(count)->x > path.At(count-1)->x) {
			(*x) -= width / 4;
		}
	}
	else if (path.At(count)->x != path.At(count-2)->x) {
		if (path.At(count)->x < path.At(count-2)->x) {
			(*x) += width / 4;
		}
		else if (path.At(count)->x > path.At(count-2)->x) {
			(*x) -= width / 4;
		}
	}	// This second comprovation is done in order to make movement more dynamic, and less rigid

	if (path.At(count)->y != path.At(count-1)->y) {
		if (path.At(count)->y < path.At(count-1)->y) {
			(*y) += height / 4;
		}
		else if (path.At(count)->y > path.At(count-1)->y) {
			(*y) -= height / 4;
		}
	}
	else if (path.At(count)->y != path.At(count-2)->y) {
		if (path.At(count)->y > path.At(count-2)->y) {
			(*y) -= height / 4;
		}
	}	// This second comprovation is done in order to make movement more dynamic, and less rigid
}

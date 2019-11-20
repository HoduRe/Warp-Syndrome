#include "j1App.h"
#include "Fire_Skull.h"
#include "j1Collision.h"

Enemy_FireSkull::Enemy_FireSkull(int x, int y) : Enemy(x, y) {}

Enemy_FireSkull::~Enemy_FireSkull() {}

void Enemy_FireSkull::Move() {
	
	GeneralMove(&position.x, &position.y, path);

	int height = App->map->data.tile_height;
	if (path.At(path.Count() - 3) != nullptr && path.At(path.Count())->y != path.At(path.Count() - 3)->y) {
		if (path.At(path.Count())->y < path.At(path.Count() - 3)->y) {
			position.y += height / 10;
		}
	}	// This function allows low-diagonal movement, exclusive to flying units, hence why it's in the sub-class Move function
}

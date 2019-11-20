#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "p2DynArray.h"
#include "j1Collision.h"

struct SDL_Texture;

class Enemy
{
protected:
	FrameInfo* currentframe;
	p2List_item<Animations*>* currentAnim;
	iPoint hitbox_w_h;
	p2List<Animations*> playerAnimations;

public:
	Enemy();
	Enemy(int x, int y);
	virtual ~Enemy();

	virtual void Move();
	virtual void Draw(SDL_Texture* sprites);
	void GeneralMove(int* x, int* y, p2DynArray<iPoint>& path);
};

#endif // __ENEMY_H__
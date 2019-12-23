#ifndef __COIN_G_H__
#define __COIN_G_H__
#include "Entity.h"
#include "Particles.h"

class Coin_G :public Character
{
public:
	Coin_G(fPoint Pos);
	~Coin_G();
	bool Update(float dt);
	bool OnCollision();
	bool CleanUp();
	bool PostUpdate();

private:

};





#endif // !__COIN_G_H__


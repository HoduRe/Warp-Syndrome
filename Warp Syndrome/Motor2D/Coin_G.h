#ifndef __COIN_G_H__
#define __COIN_G_H__
#include "Entity.h"
#include "Particles.h"

class Coin_G :public Character
{
public:
	Coin_G(fPoint Pos);
	~Coin_G();

	bool PreUpdate();
	bool Update(float dt);	
	bool PostUpdate();

	bool OnCollision();
	bool CleanUp();
	
	//enables coin animation, rendering and collision detection if its inside the camera boundaries
	bool DoEnable();
private:

};





#endif // !__COIN_G_H__


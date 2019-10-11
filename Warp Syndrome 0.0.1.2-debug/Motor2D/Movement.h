//this module is "Work in Progress", might get deleted later on
#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__
#include "p2Defs.h"
#include "p2Point.h"

//This class will be a "basic" physics engine
class Movement
{
public:
	Movement();
	~Movement();

	//adds a force to the sum of forces
	//Only the second point of the vector is required
	//it is assumed that the first point is (0,0)
	void AddForce(fPoint vector);
	//adds a force to the sum of forces
	//Only the second point of the vector is required
	//it is assumed that the first point is (0,0)
	void AddForce(float vectorX, float vectorY);


	void ConvertForcesToAccel();



private:
	fPoint forces;
	fPoint acceleration;
	fPoint velocity;
	fPoint position;
};

Movement::Movement()
{
}

Movement::~Movement()
{
}

//adds a force to the overall force
void Movement::AddForce(fPoint vector)
{
	forces+= vector;
}
//adds a force to the overall force
void Movement::AddForce(float vectorX, float vectorY)
{
	fPoint aux;
	aux.x = vectorX;
	aux.y = vectorY;
	forces += aux;
}

//converts forces to acceleration
//sets sum of forces back to 0
void Movement::ConvertForcesToAccel()
{
	acceleration = forces;
	forces.x = 0.0f;
	forces.y = 0.0f;
}

#endif // !__MOVEMENT_H__


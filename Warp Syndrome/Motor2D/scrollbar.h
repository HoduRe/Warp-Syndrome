#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Scrollbar :public UI
{
public:
	Scrollbar(float x, float y, float length);
	virtual ~Scrollbar();

	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Returns true if the scrollbar is being pressed
	bool Pressed();

	// Moves the scrollbar
	bool Move();

private:
	float initial_point, max_point, initial_mouse_pos, current_point;
	SDL_Rect bar_measures;
};

#endif // !__SCROLLBAR_H__

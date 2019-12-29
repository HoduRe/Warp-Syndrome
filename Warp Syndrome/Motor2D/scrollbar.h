#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Scrollbar :public UI
{
public:
	Scrollbar(float x, float y, UI* node, float length, UI_Purpose secondary_type, UI* element = nullptr);
	virtual ~Scrollbar();

	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Moves the scrollbar
	bool Move();

private:
	float initial_point, max_point, initial_mouse_pos, current_point;
	SDL_Rect bar_measures;
	UI* reference_element;
};

#endif // !__SCROLLBAR_H__

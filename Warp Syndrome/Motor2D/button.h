#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Button :public UI
{
public:
	Button(UIType atype = UI_TYPE_BUTTON);
	virtual ~Button();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Returns true if the button is being pushed
	bool Pushed();

public:
	bool pushed;
};

#endif // !__BUTTON_H__

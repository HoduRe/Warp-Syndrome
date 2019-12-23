#ifndef __EDITABLE_TEXT_H__
#define __EDITABLE_TEXT_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Editable_Text :public UI
{
public:
	Editable_Text(float x, float y);
	virtual ~Editable_Text();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	bool focus;
	int max_characters;
};

#endif // !__EDITABLE_TEXT_H__

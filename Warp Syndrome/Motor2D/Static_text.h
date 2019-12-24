#ifndef __STATIC_TEXT_H__
#define __STATIC_TEXT_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"
#include "j1Fonts.h"

class Static_Text :public UI
{
public:
	Static_Text(float x, float y, const char* text_input);
	virtual ~Static_Text();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	_TTF_Font* font;
};

#endif // !__STATIC_TEXT_H__

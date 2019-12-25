#ifndef __EDITABLE_TEXT_H__
#define __EDITABLE_TEXT_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"
#include "j1Fonts.h"
#include "j1Input.h"

class Editable_Text :public UI
{
public:
	Editable_Text(float x, float y, UI* node, float width, bool focus = true);
	virtual ~Editable_Text();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	SDL_Texture* text_texture;
	SDL_Rect rect;
	SDL_Rect cursor;
	_TTF_Font* font;
	char text[CHAR_ARRAY];
	int max_width;
};

#endif // !__EDITABLE_TEXT_H__

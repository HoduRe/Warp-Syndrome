#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Static_Image :public UI
{
public:
	Static_Image(float x, float y, UI* node, SDL_Texture* texture_pointer, SDL_Rect* rect,bool renderasrect=false, int r=255, int g=255, int b=255, int a=255, bool render = false);

	virtual ~Static_Image();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool SetColor(SDL_Color color);
	bool SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

private:
	bool render_print;
	bool renderasrect;
	SDL_Color rect_color;
};

#endif // !__STATIC_IMAGE_H__

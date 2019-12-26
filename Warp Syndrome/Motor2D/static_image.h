#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Static_Image :public UI
{
public:
	Static_Image(float x, float y, UI* node, SDL_Texture* texture_pointer, SDL_Rect* rect, bool render = false);
	virtual ~Static_Image();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	bool render_print;

};

#endif // !__STATIC_IMAGE_H__

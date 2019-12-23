#ifndef __STATIC_TEXT_H__
#define __STATIC_TEXT_H__
#include "SDL/include/SDL.h"
#include "UI_Elements.h"

class Static_Text :public UI
{
public:
	Static_Text(float x, float y, p2SString* text_input);
	virtual ~Static_Text();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	p2SString text;
	int font;
	int textscale;
};

#endif // !__STATIC_TEXT_H__

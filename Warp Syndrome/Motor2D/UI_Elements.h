#ifndef __UI_H__
#define __UI_H__
#include "SDL/include/SDL.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "p2DynArray.h"

enum UIType
{
	UI_TYPE_BUTTON,
	UI_TYPE_STATIC_IMAGE,
	UI_TYPE_STATIC_TEXT,
	UI_TYPE_EDITABLE_TEXT,
	UI_TYPE_SLIDER,
	UI_TYPE_UNKNOWN
};

enum UI_Purpose {
	PURPOSE_UNSPECIFIED,
	BUTTON_GAME_LOOP,
	BUTTON_OPEN_SAVES,
	BUTTON_CREDITS,
	BUTTON_SETTINGS,
	BUTTON_OPEN_MENU,
	BUTTON_CLOSE_MENU,
	BUTTON_MAIN_MENU,
	BUTTON_OPTION_SELECTION,
	BUTTON_EXIT,
	SCROLLBAR_MUSIC,
	SCROLLBAR_SFX,
	SCROLLBAR_MASK,
	STATIC_TEXT_MASK,
	PURPOSE_UNKNOWN
};

enum UICallbackState
{
	UI_CALLBACK_CLICKED,
	UI_CALLBACK_HOVER,
	UI_CALLBACK_UNKNOWN
};

class UI
{
public:
	UI(float x, float y, UI* node);
	virtual ~UI();

	virtual bool Awake();
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	// Returns true when the element is pressed
	bool Pressed();
	// Returns true when the element has the mouse on it (or the focus)
	bool Hover();

	bool CallListeners(UICallbackState state= UI_CALLBACK_UNKNOWN);
public:

	fPoint position;
	UIType type;
	UI_Purpose purpose_type;
	UI* parent;
	SDL_Texture* texture;
	SDL_Texture* hover_texture;
	SDL_Rect texture_section; //rectangle which will be displayed from the texture
	p2DynArray<j1Module*> listeners;
};
#endif // __UI_H__

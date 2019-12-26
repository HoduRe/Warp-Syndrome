#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "UI_Elements.h"
#include "SDL/include/SDL.h"

// ----------------------------------------------------
class j1GUI : public j1Module
{
public:

	j1GUI();

	// Destructor
	virtual ~j1GUI();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called before render is available
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called render has been executed
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Adds a UI element to the list
	UI* AddUIElement(UI* UIElement);

	// Returns a pointer to atlas
	SDL_Texture* GetAtlas() const;

	// Deletes all elements that have as parent the given pointer
	void DeleteOnParent(UI* deleteparent);
	
	// Deletes all elements that have as parent the given pointer + the parent itself
	bool DeleteWithParent(UI* deleteparent);

	// Deletes all elements on reverse order
	void DeleteAll();

	p2List<UI*> UI_list;
	p2List_item<UI*>* focus;
	UI* last_parent;

private:
	p2SString atlas_file_name;
	SDL_Texture* atlas;

};

#endif // __j1GUI_H__

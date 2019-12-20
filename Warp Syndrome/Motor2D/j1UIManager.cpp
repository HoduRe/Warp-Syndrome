#include "j1App.h"
#include "j1UIManager.h"
#include "p2Log.h"

j1UIManager::j1UIManager() : j1Module() { name.create("ui_m"); }

// Destructor
j1UIManager::~j1UIManager() {}

// Called before render is available
bool j1UIManager::PreUpdate() {

	return true;
}

bool j1UIManager::Update(float dt) {

	return true;
}

bool j1UIManager::PostUpdate() {

	return true;
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Unloading UIManager");
	p2List_item<UI*>* item = UI_list.start;
	while (item != NULL)
	{
			item->data->CleanUp();
			RELEASE(item->data);
			UI_list.del(item);
			
			item = item->next;
	}
	
	UI_list.clear();

	return true;
}

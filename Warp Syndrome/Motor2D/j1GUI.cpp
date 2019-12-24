#include "j1App.h"
#include "j1GUI.h"
#include "p2Log.h"

j1GUI::j1GUI() : j1Module() { name.create("gui"); }

// Destructor
j1GUI::~j1GUI() {}

bool j1GUI::Awake(pugi::xml_node& config) {
	atlas_file_name = config.child("atlas").attribute("file").as_string("");
	return true;
}

bool j1GUI::Start() {
	atlas = App->tex->Load(atlas_file_name.GetString());
	return true;
}

// Called before render is available
bool j1GUI::PreUpdate() {

	p2List_item<UI*>* item = UI_list.start;
	while (item != NULL) {
		item->data->PreUpdate();
		item = item->next;
	}

	return true;
}

bool j1GUI::Update(float dt) {

	p2List_item<UI*>* item = UI_list.start;
	while (item != NULL) {
		item->data->Update(dt);
		item = item->next;
	}

	return true;
}

bool j1GUI::PostUpdate() {

	p2List_item<UI*>* item = UI_list.start;
	while (item != NULL) {
		item->data->PostUpdate();
		item = item->next;
	}

	return true;
}

// Called before quitting
bool j1GUI::CleanUp()
{
	LOG("Unloading UIManager");
	p2List_item<UI*>* item = UI_list.start;
	while (item != NULL) {
			item->data->CleanUp();
			RELEASE(item->data);
			UI_list.del(item);
			
			item = item->next;
	}
	
	UI_list.clear();
	App->tex->UnLoad(atlas);

	return true;
}

void j1GUI::AddUIElement(UI* UIElement) {
	UI_list.add(UIElement);
}

SDL_Texture* j1GUI::GetAtlas() const {
	return atlas;
}
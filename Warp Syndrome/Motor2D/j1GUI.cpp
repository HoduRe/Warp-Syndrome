#include "j1App.h"
#include "j1GUI.h"
#include "j1Input.h"
#include "p2Log.h"

j1GUI::j1GUI() : j1Module() {
	name.create("gui");
	focus = nullptr;
	last_parent = nullptr;
}

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
		if (item->data->Pressed() == true && item->data->parent == last_parent) { focus = item; }
		item = item->next;
	}
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) { focus = focus->next; }

	item = UI_list.start;
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
	CleanAllElements();
	App->tex->UnLoad(atlas);

	return true;
}

UI* j1GUI::AddUIElement(UI* UIElement) {
	UI_list.add(UIElement);
	return UIElement;
}

SDL_Texture* j1GUI::GetAtlas() const {
	return atlas;
}

bool j1GUI::CleanAllElements()
{
	p2List_item<UI*>* item = UI_list.start;
	while (item != NULL) {
		item->data->CleanUp();
		RELEASE(item->data);
		UI_list.del(item);

		item = item->next;
	}

	UI_list.clear();
	return true;
}
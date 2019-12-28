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

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) {
		if (focus != nullptr && focus->next != nullptr) { focus = focus->next; }
		else if (UI_list.start == nullptr) { focus = nullptr; }
		else { focus = UI_list.start; }
	}

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
	DeleteAll();
	App->tex->UnLoad(atlas);

	return true;
}

UI* j1GUI::AddUIElement(UI* UIElement) {
	UI_list.add(UIElement);
	return UIElement;
}

SDL_Texture* j1GUI::GetAtlas() const { return atlas; }

void j1GUI::DeleteOnParent() {
	UI* aux_parent = last_parent;
	p2List_item<UI*>* item = UI_list.start;
	while (item != nullptr) {
		if (item->data->parent == aux_parent) {
			item->data->CleanUp();
			RELEASE(item->data);
			UI_list.del(UI_list.At(UI_list.find(item->data)));
			if (UI_list.start == nullptr) { item = nullptr; }
			else { item = UI_list.start; }
		}
		if (item != nullptr) { item = item->next; }
	}
	focus = nullptr;
}

bool j1GUI::DeleteWithParent() {
	bool ret = true;
	UI* aux_parent = last_parent;
	p2List_item<UI*>* item = UI_list.start;
	while (item != nullptr) {
		if (item->data->parent == aux_parent) {
			item->data->CleanUp();
			RELEASE(item->data);
			UI_list.del(UI_list.At(UI_list.find(item->data)));
			if (UI_list.start == nullptr) { item = nullptr; }
			else { item = UI_list.start; }
		}
		if (item != nullptr) { item = item->next; }
	}

	int findparent = UI_list.find(aux_parent);
	if (findparent != -1)
	{
		item = UI_list.At(UI_list.find(aux_parent));//takes the parent
		item->data->CleanUp();
		RELEASE(item->data);
		UI_list.del(item);
	}
	else ret = false;
	focus = nullptr;
	return ret;
}

void j1GUI::DeleteAll() {
	p2List_item<UI*>* item = UI_list.start;
	while (last_parent != nullptr) {
		UI* aux_parent = last_parent;
		while (item != NULL) {
			if (item->data->parent == aux_parent) {
				item->data->CleanUp();
				RELEASE(item->data);
				UI_list.del(UI_list.At(UI_list.find(item->data)));
				if (UI_list.start == nullptr) { item = nullptr; }
				else { item = UI_list.start; }
			}
			if (item != nullptr) { item = item->next; }
		}
		if (last_parent != nullptr) { last_parent = last_parent->parent; }
	}
	item = UI_list.start;
	while(item!=NULL)
	{
		item->data->CleanUp();
		RELEASE(item->data);
		UI_list.del(UI_list.At(UI_list.find(item->data)));
		item = item->next;
	}
	UI_list.clear();
	focus = nullptr;
}

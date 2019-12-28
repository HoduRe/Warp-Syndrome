#include "j1App.h"
#include "UI_Elements.h"
#include "j1GUI.h"
#include "j1Input.h"
#include "j1Audio.h"

//UI=====================================================
UI::UI(float x, float y, UI* node) {
	position.x = x;
	position.y = y;
	texture_section = { 0, 0, 0, 0 };
	texture = App->gui->GetAtlas();
	hover_texture = App->gui->GetHoverAtlas();
	purpose_type = PURPOSE_UNSPECIFIED;
	parent = node;
	App->gui->last_parent = parent;
	if (parent != nullptr) { App->gui->focus = nullptr; }
}

UI::~UI() { this->CleanUp(); }

bool UI::Awake() { return true; }

bool UI::Start() { return true; }

bool UI::PreUpdate() { return true; }

bool UI::Update(float dt) { return true; }

bool UI::PostUpdate() { return true; }

bool UI::CleanUp() {
	if (texture != App->gui->GetAtlas()) {
		App->tex->UnLoad(texture);
	}
	texture = nullptr;
	hover_texture = nullptr;
	if (parent != nullptr) { App->gui->last_parent = parent->parent; }
	else { App->gui->last_parent = nullptr; }
	parent = nullptr;
	listeners.Clear();
	return true;
}

bool UI::Pressed() {
	if (parent == App->gui->last_parent) {
		iPoint posi;
		App->input->GetMousePosition(posi.x, posi.y);
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == 1 || (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == 2 && type == UI_TYPE_SLIDER)) {
			if (posi.x >= position.x && posi.x <= position.x + texture_section.w && posi.y >= position.y && posi.y <= position.y + texture_section.h) {
				App->audio->PlayFx(App->gui->button_click);
				return true;
			}
		}
		else if (App->gui->focus != nullptr && App->gui->focus->data == this && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			App->audio->PlayFx(App->gui->button_click);
			return true;
		}
	}
	return false;
}

bool UI::Hover() {
	if (parent == App->gui->last_parent) {
		iPoint posi;
		App->input->GetMousePosition(posi.x, posi.y);
		if (posi.x >= position.x && posi.x <= position.x + texture_section.w && posi.y >= position.y && posi.y <= position.y + texture_section.h) {
			return true;
		}
		else if (App->gui->focus != nullptr && App->gui->focus->data == this) {
			return true;
		}
	}
	return false;
}

//calls all the listeners with information of this Ui element
bool UI::CallListeners(UICallbackState state)
{
	if (listeners.Count() > 0) 
	{
		for (int i = 0; i <= listeners.Count() - 1; i++)
		{
			if (listeners[i] != NULL)
				listeners[i]->OnListen(this, state);
		}
	}
	return true;
}
#include "j1App.h"
#include "UI_Elements.h"
#include "j1GUI.h"
#include "j1Input.h"

//UI=====================================================
UI::UI(float x, float y) {
	position.x = x;
	position.y = y;
	texture_section = { 0, 0, 0, 0 };
	texture = App->gui->GetAtlas();
}

UI::~UI() { this->CleanUp(); }

bool UI::Awake() { return true; }

bool UI::Start() { return true; }

bool UI::PreUpdate() { return true; }

bool UI::Update(float dt) { return true; }

bool UI::PostUpdate() { return true; }

bool UI::CleanUp() {
	App->tex->UnLoad(texture);
	texture = nullptr;
	return true;
}

bool UI::Pressed() {
	iPoint posi;
	App->input->GetMousePosition(posi.x, posi.y);
	if (posi.x >= position.x && posi.x <= position.x + texture_section.w && posi.y >= position.y && posi.y <= position.y + texture_section.h && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == 2) {
		return true;
	}
	return false;
}

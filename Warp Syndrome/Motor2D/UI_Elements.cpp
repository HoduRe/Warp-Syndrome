#include "j1App.h"
#include "UI_Elements.h"
#include "j1GUI.h"

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

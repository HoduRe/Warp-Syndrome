#include "UI.h"

//UI=====================================================
UI::UI(UIType eType) :type(eType) {
	position = { 0.0f,0.0f };
	texture = nullptr;
	texture_section = { 0,0,0,0 };
}

UI::~UI() { this->CleanUp(); }

bool UI::Awake() { return true; }

bool UI::Start() { return true; }

bool UI::PreUpdate() { return true; }

bool UI::Update(float dt) {
	return true;
}

bool UI::PostUpdate() { return true; }

bool UI::CleanUp() {
	texture = nullptr;
	return true;
}

#include "j1App.h"
#include "button.h"
#include "Entity.h"
#include "j1Textures.h"


Button::Button(UIType atype) :UI(atype) {
	texture = nullptr;
	pushed = false;
}

Button::~Button() {

}

bool Button::PreUpdate() {

	return true;
}

bool Button::Update(float dt) {

	return true;
}

bool Button::PostUpdate() {

	return true;
}

bool Button::CleanUp() {

	return true;
}

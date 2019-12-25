#include "j1App.h"
#include "button.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"

Button::Button(float x, float y) : UI(x, y) {
	pushed = false;
	texture_section = { 416, 172, 218, 58};
	type = UI_TYPE_BUTTON;
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

	if (Pressed() == true) {
		texture_section.x = 416;
		App->render->Blit(texture, position.x, position.y, &texture_section);
		CallListeners(UI_CALLBACK_CLICKED);
	}
	else {
		texture_section.x = 647;
		App->render->Blit(texture, position.x, position.y, &texture_section);
	}

	return true;
}

bool Button::CleanUp() {

	return true;
}

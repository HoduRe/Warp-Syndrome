#include "j1App.h"
#include "Static_text.h"
#include "j1Textures.h"
#include "j1Render.h"

Static_Text::Static_Text(float x, float y, p2SString* text_input) : UI(x, y) {
	text = *text_input;
//	texture_section = *rect;	TODO: make this based on the amount of letters / if the module fonts already return it
}

Static_Text::~Static_Text() {

}

bool Static_Text::PreUpdate() {

	return true;
}

bool Static_Text::Update(float dt) {

	return true;
}

bool Static_Text::PostUpdate() {

	App->render->Blit(texture, position.x, position.y, &texture_section);

	return true;
}

bool Static_Text::CleanUp() {

	return true;
}

#include "j1App.h"
#include "editable_text.h"
#include "j1Textures.h"
#include "j1Render.h"

Editable_Text::Editable_Text(float x, float y) : UI(x, y) {
	focus = false;
	max_characters = 10;
}

Editable_Text::~Editable_Text() {

}

bool Editable_Text::PreUpdate() {

	return true;
}

bool Editable_Text::Update(float dt) {

	return true;
}

bool Editable_Text::PostUpdate() {

	App->render->Blit(texture, position.x, position.y, &texture_section);

	return true;
}

bool Editable_Text::CleanUp() {

	return true;
}

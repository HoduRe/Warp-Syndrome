#include "j1App.h"
#include "Static_text.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Render.h"

Static_Text::Static_Text(float x, float y, UI* node, const char* text_input) : UI(x, y, node) {
	font = App->font->fonts.start->data;
	texture = App->font->Print(text_input, {}, font);
	App->font->CalcSize(text_input, texture_section.w, texture_section.h);
	type = UI_TYPE_STATIC_TEXT;
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

	App->render->Blit(texture, position.x, position.y, &texture_section,false,0.0f,0.0f,0.0f,0.0f);

	return true;
}

bool Static_Text::CleanUp() {

	return true;
}

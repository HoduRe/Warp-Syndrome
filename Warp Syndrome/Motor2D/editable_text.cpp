#include "j1App.h"
#include "editable_text.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Console.h"

Editable_Text::Editable_Text(float x, float y, UI* node, float width, bool focus) : UI(x, y, node) {
	texture_section = { 494, 577, (int)width + 10, 45} ;
	rect = { 0, 0, 0, 0};
	cursor = { (int)x, (int)y, 1, 5};
	font = App->font->fonts.start->data;
	max_width = width;
	type = UI_TYPE_EDITABLE_TEXT;
}

Editable_Text::~Editable_Text() {}

bool Editable_Text::PreUpdate() {

	return true;
}

bool Editable_Text::Update(float dt) {

	if (App->gui->focus != nullptr && App->gui->focus->data == this) {
		for (int i = 0; i < CHAR_ARRAY - 1; i++) { text[i] = App->input->text[i]; }
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		App->console->ReceiveText(text);
		memset(text, NULL, sizeof(text));
		memset(App->input->text, NULL, sizeof(App->input->text));
		App->gui->focus = nullptr;
	}
	// Move cursor

	return true;
}

bool Editable_Text::PostUpdate() {

	// Measurements
	text_texture = App->font->Print(text, max_width, {255,255,255,255}, font);
	App->font->CalcSize(text, rect.w, rect.h, max_width);
//	texture_section.h = rect.h + 10;
	char cursor_distance[CHAR_ARRAY];
	memset(cursor_distance, NULL, sizeof(cursor_distance));
	int i = 0, cursor_aux = App->input->GetCursor();
	while (i < cursor_aux+1) {
		cursor_distance[i] = App->input->text[i];
		i++;
	}
	App->font->CalcSize(cursor_distance, cursor.x, cursor.h, max_width);

	cursor.x += position.x;

	// Blits
	/*if (Hover()) { App->render->Blit(hover_texture, position.x - 5, position.y - 5, &texture_section, false, 0.0f, 0.0f, 0.0f, 0.0f); }
	else { App->render->Blit(texture, position.x - 5, position.y - 5, &texture_section, false, 0.0f, 0.0f, 0.0f, 0.0f); }*/
	App->render->Blit(text_texture, position.x, position.y, &rect,false,0.0f,0.0f,0.0f,0.0f);
	App->render->DrawQuad(cursor, 255, 255, 255, 255, true, false);
	
	App->tex->UnLoad(text_texture);

	return true;
}

bool Editable_Text::CleanUp() {
	App->tex->UnLoad(text_texture);
	text_texture = nullptr;
	return true;
}

p2SString Editable_Text::ReturnText() { return text; }

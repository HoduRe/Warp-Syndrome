#include "j1App.h"
#include "Static_text.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Render.h"

Static_Text::Static_Text(float x, float y, UI* node, const char* text_input,Uint32 p_width, UI_Purpose second_type, int* counter, int mask_height, int r, int g, int b, int a) : UI(x, y, node) {
	font = App->font->fonts.start->data;
	row_w = p_width;
	texture = App->font->Print(text_input,row_w, { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a }, font);
	App->font->CalcSize(text_input, texture_section.w, texture_section.h,row_w);
	purpose_type = second_type;
	if (second_type == STATIC_TEXT_MASK) { max_height = mask_height; }
	type = UI_TYPE_STATIC_TEXT;
	counter_path = counter;
}

Static_Text::~Static_Text() {

}

bool Static_Text::PreUpdate() {

	return true;
}

bool Static_Text::Update(float dt) {

	if (purpose_type == STATIC_TEXT_VARIABLE) {
		char text[5] = "x000";
		text[1] = (char)* counter_path / 100 + '0';
		text[2] = ((char)* counter_path % 100) / 10 + '0';
		text[3] = ((char)* counter_path % 100) % 10 + '0';
		NewText(text);
	}

	return true;
}

bool Static_Text::PostUpdate() {

	if (purpose_type == STATIC_TEXT_MASK && texture_section.h > max_height) { texture_section.h = max_height; }

	App->render->Blit(texture, position.x, position.y, &texture_section,false,0.0f,0.0f,0.0f,0.0f);

	return true;
}

bool Static_Text::CleanUp() {
	counter_path = nullptr;

	return true;
}

void Static_Text::NewText(const char* text)
{
		App->tex->UnLoad(texture);
		texture = App->font->Print(text,row_w, {255,255,255,255}, font);
		App->font->CalcSize(text, texture_section.w, texture_section.h,row_w);

}
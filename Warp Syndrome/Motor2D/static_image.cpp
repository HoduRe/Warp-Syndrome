#include "j1App.h"
#include "static_image.h"
#include "j1Textures.h"
#include "j1Render.h"

Static_Image::Static_Image(float x, float y, UI* node, SDL_Texture* texture_pointer, SDL_Rect* rect, bool render_as_rect, int r, int g, int b, int a,bool render) : UI(x, y, node) {
	texture = texture_pointer;
	texture_section = *rect;
	type = UI_TYPE_STATIC_IMAGE;
	render_print = render;
	SetColor(r, g, b, a);
	renderasrect = render_as_rect;
}


Static_Image::~Static_Image() {

}

bool Static_Image::PreUpdate() {

	return true;
}

bool Static_Image::Update(float dt) {

	return true;
}

bool Static_Image::PostUpdate() {
	if (Pressed())
		CallListeners(UI_CALLBACK_CLICKED);
	if (render_print == true) { SDL_RenderCopy(App->render->renderer, texture, NULL, NULL); }
	else if (!renderasrect) { App->render->Blit(texture, position.x, position.y, &texture_section, false, 0.0f, 0.0f, 0.0f, 0.0f); }
	else App->render->DrawQuad(texture_section, rect_color.r, rect_color.g, rect_color.b, rect_color.a, true, false);
	return true;
}

bool Static_Image::CleanUp() {
	return true;
}
bool Static_Image::SetColor(SDL_Color color)
{
	rect_color = color;
	return true;
}
bool Static_Image::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	rect_color = { r,g,b,a };
	return true;
}
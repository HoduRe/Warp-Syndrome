#include "j1App.h"
#include "static_image.h"
#include "j1Textures.h"
#include "j1Render.h"

Static_Image::Static_Image(float x, float y, UI* node, SDL_Texture* texture_pointer, SDL_Rect* rect, bool render) : UI(x, y, node) {
	texture = texture_pointer;
	texture_section = *rect;
	type = UI_TYPE_STATIC_IMAGE;
	render_print = render;
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

	if (render_print == true) { SDL_RenderCopy(App->render->renderer, texture, NULL, NULL); }
	else { App->render->Blit(texture, position.x, position.y, &texture_section, false, 0.0f, 0.0f, 0.0f, 0.0f); }

	return true;
}

bool Static_Image::CleanUp() {
	App->tex->UnLoad(texture);
	texture = nullptr;
	return true;
}

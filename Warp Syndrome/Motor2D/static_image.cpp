#include "j1App.h"
#include "static_image.h"
#include "j1Textures.h"
#include "j1Render.h"

Static_Image::Static_Image(float x, float y, UI* node, SDL_Texture* texture_pointer, SDL_Rect* rect) : UI(x, y, parent) {
	texture = texture_pointer;
	texture_section = *rect;
	type = UI_TYPE_STATIC_IMAGE;
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

	App->render->Blit(texture, position.x, position.y, &texture_section);

	return true;
}

bool Static_Image::CleanUp() {

	return true;
}

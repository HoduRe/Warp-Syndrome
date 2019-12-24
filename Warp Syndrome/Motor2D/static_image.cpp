#include "j1App.h"
#include "static_image.h"
#include "j1Textures.h"
#include "j1Render.h"

Static_Image::Static_Image(float x, float y, SDL_Texture* texture_pointer, SDL_Rect* rect) : UI(x, y) {
	texture = texture_pointer;
	texture_section = *rect;
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
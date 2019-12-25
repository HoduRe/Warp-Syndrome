#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1GUI.h"
#include "button.h"
#include "scrollbar.h"
#include "Static_text.h"
#include "editable_text.h"
#include "j1SceneIntro.h"
#include "level_manager.h"
#include "j1Scene.h"
#include "UI_Elements.h"

j1SceneIntro::j1SceneIntro() : j1Module()
{
	name.create("scene_intro");
}

// Destructor
j1SceneIntro::~j1SceneIntro()
{}

// Called before the first frame
bool j1SceneIntro::Start()
{
	UI* element;
	element = App->gui->AddUIElement(new Button(150, 50, nullptr, BUTTON_GAME_LOOP));
	element->listeners.PushBack(App->scene);
	element = App->gui->AddUIElement(new Scrollbar(200, 250, nullptr, 50));
	element->listeners.PushBack(App->scene);
	element = App->gui->AddUIElement(new Static_Text(200, 200, nullptr, "Hola Mundo"));
	element->listeners.PushBack(App->scene);
	element = App->gui->AddUIElement(new Editable_Text(400, 200, nullptr, 200));
	element->listeners.PushBack(App->scene);

	return true;
}

// Called each loop iteration
bool j1SceneIntro::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1SceneIntro::Update(float dt) {

	return true;
}

// Called each loop iteration
bool j1SceneIntro::PostUpdate() {
	return true;
}

// Called before quitting
bool j1SceneIntro::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


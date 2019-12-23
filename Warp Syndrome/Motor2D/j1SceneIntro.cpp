#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1GUI.h"
#include "button.h"
#include "scrollbar.h"
#include "j1SceneIntro.h"
#include "level_manager.h"

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
	App->gui->AddUIElement(new Button(150, 50));
	App->gui->AddUIElement(new Scrollbar(200, 250, 50));
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


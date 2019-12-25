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
#include "UI_Elements.h"
#include "j1GUI.h"
#include "button.h"
#include "scrollbar.h"
#include "Static_text.h"
#include "editable_text.h"
#include "j1SceneManager.h"
#include "level_manager.h"
#include "j1Scene.h"


j1SceneManager::j1SceneManager() : j1Module()
{
	name.create("scene_manager");
}

// Destructor
j1SceneManager::~j1SceneManager()
{}

// Called before the first frame
bool j1SceneManager::Start()
{
	currentloop = G_C_START;//TODO change this to start in the main menu

	return true;
}

// Called each loop iteration
bool j1SceneManager::PreUpdate()
{
	bool ret = true;
	//changes loop
	if (!doingaction)
	{

		switch (currentloop)
		{
		case G_C_START:
			LoadMainMenu();
			App->paused = true;
			currentloop = G_C_MAIN_MENU;
			break;
		case G_C_MAIN_MENU:
			if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN/*button play pressed*/)
			{
				//this transition unloads and loads the corresponding ui automatically
				App->transitions->ChangeTransition(Transition_Mode::TM_CHANGE_TO_GAME, 2.0f);
				doingaction = true;
				currentloop = G_C_INGAME;
			}
			else if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN/*button quit pressed*/)
			{
				ret = false; //this will now quit the game
			}
			else if (false/*button continue pressed*/)
			{
				//TODO will cause trouble due to the game loop structure
			}
			break;
		case G_C_PAUSE_MENU:
			//when the game unpauses
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
				App->paused = false;
				currentloop = G_C_INGAME;
				UnloadPauseMenu();
				LoadHUD();//reloads the hud due to ui cleanup
			}
			//when the game goes to the menu
			else if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
			{
				App->transitions->ChangeTransition(Transition_Mode::TM_CHANGE_TO_MENU, 2.0f);
				doingaction = true;
				currentloop = G_C_MAIN_MENU;

			}

			break;
		case G_C_INGAME:
			//when the game pauses
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
				App->paused = true;
				currentloop = G_C_PAUSE_MENU;
				LoadPauseMenu();
			}
			//when the game goes to the menu ¿Can we go to the menu directly?
			break;

		case G_C_UNKNOWN:
		default:
			ret = false;//quits the game
			break;
		}
	}
	return ret;
}

// Called each loop iteration
bool j1SceneManager::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool j1SceneManager::PostUpdate() {

	return true;
}

// Called before quitting
bool j1SceneManager::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//this function will listen to the different Ui imputs and act accordingly
bool j1SceneManager::OnListen(UI* element, UICallbackState cstate)
{
	if (cstate == UICallbackState::UI_CALLBACK_CLICKED)
		element->position.x += 50;
	return true;
}

//Loads all the UI for the main menu
bool j1SceneManager::LoadMainMenu()
{
	//placeholder UI
	UI* element;
	element = App->gui->AddUIElement(new Button(150, 50, nullptr, BUTTON_GAME_LOOP));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Scrollbar(200, 250, nullptr, 50));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Static_Text(200, 200, nullptr, "Hola Mundo"));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Editable_Text(400, 200, nullptr, 200));
	element->listeners.PushBack(this);

	return true;
}

bool j1SceneManager::UnloadMainMenu()
{
	App->gui->DeleteAll();
	return true;
}

//Loads all the UI for the pause menu
bool j1SceneManager::LoadPauseMenu()
{
	//placeholder UI
	UI* element;
	element = App->gui->AddUIElement(new Button(150, 50, nullptr, BUTTON_GAME_LOOP));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Scrollbar(200, 250, nullptr, 50));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Static_Text(200, 200, nullptr, "Hola Mundo"));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Editable_Text(400, 200, nullptr, 200));
	element->listeners.PushBack(this);

	return true;
}

bool j1SceneManager::UnloadPauseMenu()
{
	App->gui->DeleteAll();
	
	return true;
}

//Loads all the UI for the HUD
bool j1SceneManager::LoadHUD()
{
	//placeholder UI
	UI* element;
	element = App->gui->AddUIElement(new Button(150, 50, nullptr, BUTTON_GAME_LOOP));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Scrollbar(200, 250, nullptr, 50));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Static_Text(200, 200, nullptr, "Hola Mundo"));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Editable_Text(400, 200, nullptr, 200));
	element->listeners.PushBack(this);
	return true;
}

bool j1SceneManager::UnloadHUD()
{
	App->gui->DeleteAll();
	return true;
}


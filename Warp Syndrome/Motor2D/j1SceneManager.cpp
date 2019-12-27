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
#include "static_image.h"
#include "j1SceneManager.h"
#include "level_manager.h"
#include "j1Scene.h"
#include "j1Window.h"

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
	element = nullptr;
	App->win->GetWindowSize(width, height);
	currentloop = G_C_START;//TODO change this to start in the main menu
	ui_type = UI_Purpose::PURPOSE_UNKNOWN;
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
			App->scene->draw = false;
			break;
		case G_C_MAIN_MENU:
			if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN||ui_type==UI_Purpose::BUTTON_GAME_LOOP/*button play pressed*/)
			{
				//this transition unloads and loads the corresponding ui automatically
				App->transitions->ChangeTransition(Transition_Mode::TM_CHANGE_TO_GAME, 2.0f);
				doingaction = true;
				currentloop = G_C_INGAME;
			}
			else if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || ui_type == UI_Purpose::BUTTON_EXIT/*button quit pressed*/) {
				ret = false; //this will now quit the game
			}
			else if (ui_type == UI_Purpose::BUTTON_SETTINGS/*button settings / credit are pressed*/) {

			}
			else if (ui_type == UI_Purpose::BUTTON_CREDITS/*button settings / credit are pressed*/) {
				p2List_item<UI*>* item = App->gui->UI_list.start;
				while (item != nullptr) {
					if (item->data->purpose_type == BUTTON_CREDITS) {
						element = App->gui->AddUIElement(new Button(3 * width / 5 + width / 10, height / 10, item->data, BUTTON_CLOSE_MENU));
						element->listeners.PushBack(this);
						App->gui->AddUIElement(new Static_Text(3 * width / 5 + ((width - 3 * width / 5) / 2), (height / 10) + height / 30, item->data, "Go Back"));
						item = nullptr;
					}
					if (item != nullptr) { item = item->next; }
				}

			}
			else if (ui_type == UI_Purpose::BUTTON_CLOSE_MENU) { App->gui->DeleteOnParent(); }
			else if (false/*button continue pressed*/)
			{
				//TODO will cause trouble due to the game loop structure
			}
			App->scene->blit_colliders = false;
			break;
		case G_C_PAUSE_MENU:
			//when the game unpauses
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN||ui_type == UI_Purpose::BUTTON_CLOSE_MENU) {
				App->paused = false;
				currentloop = G_C_INGAME;
				UnloadPauseMenu();
				LoadHUD();//reloads the hud due to ui cleanup
			}
			//when the game goes to the menu
			else if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || ui_type == UI_Purpose::BUTTON_GAME_LOOP)
			{
				App->transitions->ChangeTransition(Transition_Mode::TM_CHANGE_TO_MENU, 2.0f);
				doingaction = true;
				currentloop = G_C_MAIN_MENU;

			}

			break;
		case G_C_INGAME:
			//when the game pauses
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN || ui_type == UI_Purpose::BUTTON_OPEN_MENU) {
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
	ui_type = UI_Purpose::PURPOSE_UNKNOWN;
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
	element = nullptr;

	LOG("Freeing scene");

	return true;
}

//this function will listen to the different Ui imputs and act accordingly
bool j1SceneManager::OnListen(UI* element, UICallbackState cstate)
{
	if (cstate == UICallbackState::UI_CALLBACK_CLICKED)
	{
		ui_type = element->purpose_type;
	}
	return true;
}

//Loads all the UI for the main menu
bool j1SceneManager::LoadMainMenu() {

	SDL_Rect texture_rec = { 0, 0, 640, 62 };
	App->gui->AddUIElement(new Static_Image(0, 0, nullptr, App->tex->Load("textures/Loading_screen_image.png"), &texture_rec, true));
	App->gui->AddUIElement(new Static_Image(width / 5, height / 8, nullptr, App->tex->Load("textures/Logo.png"), &texture_rec));
	element = App->gui->AddUIElement(new Button(width / 3, height / 3, nullptr, BUTTON_GAME_LOOP));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Button(width / 3, (height + height / 3) / 3, nullptr, BUTTON_GAME_LOOP));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Button(width / 3, (height + 2 * height / 3) / 3, nullptr, BUTTON_SETTINGS));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Button(width / 3, (2 * height) / 3, nullptr, BUTTON_CREDITS));
	element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Button(width / 3, (2 * height + height / 3) / 3, nullptr, BUTTON_EXIT));
	element->listeners.PushBack(this);
	float t_width = (width / 3) + (width / 12);
	float t_height = height / 35;
	element = App->gui->AddUIElement(new Static_Text(t_width, height / 3 + t_height, nullptr, "   Play"));
	element = App->gui->AddUIElement(new Static_Text(t_width, (height + height / 3) / 3 + t_height, nullptr, "Continue"));
	element = App->gui->AddUIElement(new Static_Text(t_width, (height + 2 * height / 3) / 3 + t_height, nullptr, "Settings"));
	element = App->gui->AddUIElement(new Static_Text(t_width, (2 * height) / 3 + t_height, nullptr, "Credits"));
	element = App->gui->AddUIElement(new Static_Text(t_width, (2 * height + height / 3) / 3 + t_height, nullptr, "   Exit"));

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


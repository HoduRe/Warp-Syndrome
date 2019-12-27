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
	uint scale = App->win->GetScale();
	App->win->GetWindowSize(width, height);
	width *= scale;
	height *= scale;
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
			if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN || ui_type == UI_Purpose::BUTTON_GAME_LOOP/*button play pressed*/)
			{
				//this transition unloads and loads the corresponding ui automatically
				if (App->transitions->actual_transition == Transition_Mode::TM_UNKNOWN)
				{
					App->transitions->ChangeTransition(Transition_Mode::TM_CHANGE_TO_GAME, 2.0f);
					doingaction = true;
					currentloop = G_C_INGAME;
				}
			}
			else if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || ui_type == UI_Purpose::BUTTON_EXIT/*button quit pressed*/) {
				ret = false; //this will now quit the game
			}
			else if (ui_type == UI_Purpose::BUTTON_SETTINGS/*button settings / credit are pressed*/) {
				p2List_item<UI*>* item = GetListElement(BUTTON_SETTINGS);
				SDL_Rect measures = { 31, 540, 422, 454 };
				App->gui->AddUIElement(new Static_Image(width / 8, height / 8, item->data, App->gui->GetAtlas(), &measures));
				element = App->gui->AddUIElement(new Button(3 * width / 5 + width / 10, height / 10, item->data, BUTTON_CLOSE_MENU));
				element->listeners.PushBack(this);
				App->gui->AddUIElement(new Static_Text(3 * width / 5 + ((width - 3 * width / 5) / 2), (height / 10) + height / 30, item->data, "Go Back"));
				measures = { 0, 0, 50, 40 };
				App->gui->AddUIElement(new Static_Image(1.25 * (width / 8), 1.25 * (height / 8), item->data, App->tex->Load("textures/volume.png"), &measures));
				element = App->gui->AddUIElement(new Scrollbar(1.35 * (width / 8), 2 * (height / 8), item->data, 138, SCROLLBAR_MUSIC));
				element->listeners.PushBack(this);
				App->gui->AddUIElement(new Static_Image(1.25 * (width / 8), element->position.y + (height / 4), item->data, App->tex->Load("textures/volume_high.png"), &measures));
				App->gui->AddUIElement(new Static_Image(1.25 * (width / 3), 1.25 * (height / 8), item->data, App->tex->Load("textures/sfx.png"), &measures));
				element = App->gui->AddUIElement(new Scrollbar(1.35 * (width / 3), 2 * (height / 8), item->data, 138, SCROLLBAR_SFX));
				element->listeners.PushBack(this);
				App->gui->AddUIElement(new Static_Image(1.25 * (width / 3), element->position.y + (height / 4), item->data, App->tex->Load("textures/sfx_high.png"), &measures));
			}
			else if (ui_type == UI_Purpose::BUTTON_CREDITS/*button settings / credit are pressed*/) {
				p2List_item<UI*>* item = GetListElement(BUTTON_CREDITS);
				SDL_Rect measures = { 31, 540, 422, 454 };
				App->gui->AddUIElement(new Static_Image(width / 8, height / 8, item->data, App->gui->GetAtlas(), &measures));
				element = App->gui->AddUIElement(new Button(3 * width / 5 + width / 10, height / 10, item->data, BUTTON_CLOSE_MENU));
				element->listeners.PushBack(this);
				App->gui->AddUIElement(new Static_Text(3 * width / 5 + ((width - 3 * width / 5) / 2), (height / 10) + height / 30, item->data, "Go Back"));
				App->gui->AddUIElement(new Static_Text(width / 8 + width / 16, height / 8 + (height / 16), item->data,
					"Credit goes to Oscar Perez and Ferran-Roger Basart."));
				App->gui->AddUIElement(new Static_Text(width / 8 + width / 16, height / 8 + 2 * (height / 16), item->data,
					"We know this UI is lame. Just note we are trying."));
				App->gui->AddUIElement(new Static_Text(width / 8 + width / 16, height / 8 + 3 * (height / 16), item->data,
					"Still, it must be nice."));
				App->gui->AddUIElement(new Static_Text(width / 8 + width / 16, height / 8 + 4 * (height / 16), item->data,
					"It must be nice."));
				App->gui->AddUIElement(new Static_Text(width / 8 + width / 16, height / 8 + 5 * (height / 16), item->data,
					"To have the president on our side."));
				App->gui->AddUIElement(new Static_Text(width / 8 + width / 16, height / 8 + 6 * (height / 16), item->data,
					"If you got that, you are the real cool."));
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
			else if ((App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || ui_type == UI_Purpose::BUTTON_GAME_LOOP)&& App->transitions->actual_transition == Transition_Mode::TM_UNKNOWN)
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
	return true;
}

bool j1SceneManager::UnloadHUD()
{
	App->gui->DeleteAll();
	return true;
}

p2List_item<UI*>* j1SceneManager::GetListElement(UI_Purpose purpose) {
	p2List_item<UI*>* item = App->gui->UI_list.start;
	p2List_item<UI*>* true_item = nullptr;
	while (item != nullptr) {
		if (item->data->purpose_type == purpose) { true_item = item; item = nullptr; }
		if (item != nullptr) { item = item->next; }
	}
	return true_item;
}

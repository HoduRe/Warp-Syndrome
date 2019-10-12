#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(App->map->map_name.GetString());
	App->render->camera.x = App->player->playerpos.x;
	App->render->camera.y = App->player->playerpos.y;
	camvelocity = { 0.0f,0.0f };
	camstate = CS_STILL;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (reload == true)
	{
		bool ret = true;
		switch (currentlevel)
		{
		case NOLEVEL:
			LOG("could not load a level, switching to default one.");
			if (ret != App->map->ReloadMap("hello2.tmx"))
				LOG("Could not load default level. Error.");
			break;
		case LEVEL1:
			currentlevel = LEVEL2;
			if (ret != App->map->ReloadMap("first_level.tmx"))
				currentlevel = NOLEVEL;

			reload = false;
			break;
		case LEVEL2:
			currentlevel = LEVEL3;
			if (ret != App->map->ReloadMap("sewers.tmx"))
				currentlevel = NOLEVEL;

			reload = false;
			break;
		case LEVEL3:
			currentlevel = LEVEL4;
			if (ret != App->map->ReloadMap("orthogonal-outside.tmx"))
				currentlevel = NOLEVEL;

			reload = false;
			break;
		case LEVEL4:
			currentlevel = LEVEL1;
			if (ret != App->map->ReloadMap("first_level.tmx"))
				currentlevel = NOLEVEL;

			reload = false;
			break;
		}


	}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->player->AddPosition(0.0f, -4.0f);

		//App->render->camera.y -= 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->player->AddPosition(0.0f, 4.0f);

		//App->render->camera.y += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->player->AddPosition(-4.0f, 0.0f);
		App->player->SetFliped(true);
		//App->render->camera.x -= 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->player->AddPosition(4.0f, 0.0f);
		App->player->SetFliped(false);
		//App->render->camera.x += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		reload = true;
	//TODO position camera relative to the player
	RepositionCamera();
	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


void j1Scene::RepositionCamera()
{
	//variables-------------------------------------------------------------------
	fPoint playerpos = App->player->GetPosition();
	bool playerfliped = App->player->GetFliped();
	int scale = App->win->GetScale();
	int camW = App->render->camera.w;
	int camH = App->render->camera.h;
	fPoint target;
	fPoint currentcam;
	CamState lastcamstate = camstate;
	currentcam.x = App->render->camera.x;
	currentcam.y = App->render->camera.y;
	if (!playerfliped)
		target.x = playerpos.x + (camW / 3);
	else target.x = playerpos.x - (camW / 3);

	target.y = playerpos.y;


	if (((currentcam.x * -1) + (camW / 3) > target.x))//move camera to the left
	{
		camstate = CS_MOVING_LEFT;
	}
	else if ((currentcam.x * -1) + (camW * 2 / 3) < target.x)//move camera to the right
	{
		camstate = CS_MOVING_RIGHT;
	}
	else camstate = CS_STILL;

	switch (camstate)
	{
	case CS_STILL:
		switch (lastcamstate)
		{
		case CS_STILL:			
			camvelocity.x = 0;
			break;
		case CS_MOVING_LEFT:
			camvelocity.x = 0;
			break;
		case CS_MOVING_RIGHT:
			camvelocity.x = 0;
			break;
		}
		break;
	case CS_MOVING_LEFT:
		switch (lastcamstate)
		{
		case CS_STILL:
			camvelocity.x = 0;
			break;
		case CS_MOVING_LEFT:
			camvelocity.x += 0.05f;
			if (camvelocity.x > 2.0f)camvelocity.x = 1.0f;
			break;
		case CS_MOVING_RIGHT:
			camvelocity.x = 0;
			break;
		}

		currentcam.x += 8.0f * camvelocity.x;

		break;
	case CS_MOVING_RIGHT:
		switch (lastcamstate)
		{
		case CS_STILL:
			camvelocity.x = 0;
			break;
		case CS_MOVING_LEFT:
			camvelocity.x = 0;
			break;
		case CS_MOVING_RIGHT:
			camvelocity.x += 0.05f;
			if (camvelocity.x > 2.0f)camvelocity.x = 1.0f;
			break;
		}

		currentcam.x -= 8.0f * camvelocity.x;

		break;
	}
	currentcam.y = -1 * (target.y - (camH * 2 / 3));

	App->render->camera.x = currentcam.x;
	App->render->camera.y = currentcam.y;
	LOG("-");


	//copy cam position to actual camera (not done before because camera position is an int, so there's low precision if we work with 0.x values)
	/*App->render->camera.x = fCamPos.x;
	App->render->camera.y = fCamPos.y;*/

}


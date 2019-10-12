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
			if (ret != App->map->ReloadMap("hello2.tmx"))
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
		App->player->AddPosition(0.0f, -0.02f);

		App->render->camera.y -= 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		//App->player->AddPosition(0.0f, 0.02f);

		App->render->camera.y += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		//App->player->AddPosition(-0.02f, 0.0f);
		App->player->SetFliped(true);
		App->render->camera.x -= 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		//App->player->AddPosition(0.02f, 0.0f);
		App->player->SetFliped(false);
		App->render->camera.x += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		reload = true;
	//TODO position camera relative to the player
	//RepositionCamera();
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
	//variables
	fPoint playerpos = App->player->GetPosition();
	bool playerfliped = App->player->GetFliped();
	float repositionvelocity=0.04f;
	SDL_Rect camerarect = App->render->camera;//camera numbers go negative, so we flip the x and the y position during the function for a better understanding of the concepts
	//camerarect.x *= -1;
	//camerarect.y *= -1;
	int scale = App->win->GetScale();
	fPoint targetpos;//this target point is at 1/4 the screensize before the player
	fPoint cameracenter;
	cameracenter.x = camerarect.x - (camerarect.w / 2);
	cameracenter.y = camerarect.y - (camerarect.h / 2);

	//defining the target point----------------------------------------------------
	if (!playerfliped)
		targetpos.x = playerpos.x + (camerarect.w / 4);
	else targetpos.x = playerpos.x - (camerarect.w / 4);

	targetpos.y = playerpos.y - (camerarect.h * scale / 4);

	//moving the camera------------------------------------------------------------
	if ((cameracenter.x >= targetpos.x - 2 && cameracenter.x < targetpos.x) || (cameracenter.x <= targetpos.x + 2 && cameracenter.x >= targetpos.x))//detects if the camera is within a 2 pixel radius from the target
		camerarect.x = targetpos.x+cameracenter.x;
	if (cameracenter.x < targetpos.x)
		camerarect.x +=0.04f;
	if(cameracenter.x>targetpos.x)
		camerarect.x -= 0.04f;


	App->render->camera.x = targetpos.x + cameracenter.x;
	App->render->camera.y = targetpos.y + cameracenter.y;
	//transalte from function to camera--------------------------------------------
	//camerarect.x *= -1;
	//camerarect.y *= -1;
	//TODO next 2 lines of code
	//App->render->camera.x = ((camerarect.x) +;
	//App->render->camera.y = (targetpos.y + ;

}


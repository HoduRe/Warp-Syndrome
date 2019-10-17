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
	App->render->camera.x = App->player->GetPosition().x;
	App->render->camera.y = App->player->GetPosition().y;
	camvelocity = { 0.0f,0.0f };
	arrivedtoline = false;
	snapping = false;
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

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		reload = true;
	//TODO position camera relative to the player



	//RepositionCamera currently deactivated

	//App->render->camera.x = -((App->player->GetPosition().x * App->win->GetScale()) - (App->render->camera.w / 2)); //Debug Camera. Center on half width 1/3 height
	//App->render->camera.y = -((App->player->GetPosition().y * App->win->GetScale()) - (App->render->camera.w * 2 / 3));

	RepositionCamera();
	//camera boundaries
	//x coordinate
	if (-App->render->camera.x < 0)App->render->camera.x = 0;
	else if (-App->render->camera.x + App->render->camera.w > App->map->data.width * App->map->data.tile_width)
		App->render->camera.x = -(App->map->data.width * App->map->data.tile_width - App->render->camera.w);

	//y coordinate
	if (-App->render->camera.y < 0)App->render->camera.y = 0;
	else if (-App->render->camera.y + App->render->camera.h > App->map->data.height * App->map->data.tile_height)
		App->render->camera.y = -(App->map->data.height * App->map->data.tile_height - App->render->camera.h);




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

//this function moves the camera around
void j1Scene::RepositionCamera()
{
	////variables-------------------------------------------------------------------
	fPoint playerpos = App->player->GetPosition();
	bool playerfliped = App->player->GetFliped();

	//int scale = App->win->GetScale();
	int camW = App->render->camera.w;
	int camH = App->render->camera.h;
	fPoint target;
	fPoint currentcam;
	//CamState lastcamstate = camstate;
	currentcam.x = App->render->camera.x;
	currentcam.y = App->render->camera.y;
	if (!playerfliped)
		target.x = playerpos.x + (camW*0.1f); //change the number multiplying by the camW to set the distance of the taget from the player when is looking right
	else target.x = playerpos.x - (camW*0.1f);//change the number multiplying by the camW to set the distance of the taget from the player when is looking left

	target.y = playerpos.y;


	currentcam.y = -target.y + (camH * 2 / 3);
	currentcam.x = CameraGoToTarget(App->render->camera, target);

	App->render->camera.x = currentcam.x;
	App->render->camera.y = currentcam.y;



}

//returns a float corresponding to the new Camera X position Note it doesn't reset camvel
float j1Scene::CameraGoToTarget(SDL_Rect camera, fPoint target)
{
	float newcamX = camera.x;
	float playervel = App->player->GetVelocity().x;
	float camdisplacementvel = playervel * 3;

	if (camvelocity.x > 1.0f)camvelocity.x = 1.0f;

	if (target.x > -camera.x + (camera.w/2))
	{
		newcamX -= camdisplacementvel*camvelocity.x;
		LOG("Cam --, positon: %f", newcamX);
	}
	else if (target.x < -camera.x + (camera.w/2))
	{
		newcamX += camdisplacementvel*camvelocity.x;
		LOG("Cam ++, positon: %f", newcamX);

	}
	camvelocity.x += 0.05f;//change this value to change camera accel
	//TODO take the increment of cam vel (cam accel) and put it in the configuration xml
	
	if (-camera.x+(camera.w/2) <=target.x+(camdisplacementvel/2)+1 && -camera.x + (camera.w / 2) >= target.x - (camdisplacementvel/2)-1)
	{
		newcamX = -(target.x - (camera.w / 2));
		LOG("Cam pinned, positon: %f", newcamX);
		camvelocity = { 0.0f,0.0f };
	}


	return newcamX;
}






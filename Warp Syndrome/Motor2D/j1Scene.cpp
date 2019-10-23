#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1State.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "level_manager.h"

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
	App->map->LoadNew(App->level_m->default_level->data->overworld.GetString());
	App->render->camera.x = -(App->player->GetPosition().x-(App->render->camera.w/2));
	App->render->camera.y = -(App->player->GetPosition().y-(App->render->camera.h / 2));
	camaccel = { 0.0f,0.0f };
	arrivedtoline = false;
	distancetoplayer = { 0,0 };
	snapping = false;
	start = true;

	App->audio->PlayMusic(App->map->data.music_path.GetString());



	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		App->state->SetBlitColliders();
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		App->state->SetGodmode();
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		App->level_m->ChangeToLevel2();
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->level_m->ChangeToLevel1();
	}


	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		App->level_m->RestartLevel();
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		App->level_m->ChangeToNextLevel();
	}


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

	if (!App->state->GetGodmode()&&(App->collision->GroundCollision()||start)) //if its colliding with the ground execute camera movement
	{


		if (!playerfliped)
			target.x = playerpos.x + (camW * 0.1f); //change the number multiplying by the camW to set the distance of the taget from the player when is looking right
		else target.x = playerpos.x - (camW * 0.1f);//change the number multiplying by the camW to set the distance of the taget from the player when is looking left

		target.y = playerpos.y;
		currentcam.y = -target.y + (camH * 2 / 3);
		currentcam.x = CameraGoToTarget(App->render->camera, target);


		distancetoplayer.x = playerpos.x - (-currentcam.x);

		distancetoplayer.y = playerpos.y - (-currentcam.y);
		start = false;
	}
	else
	{
		currentcam.x = -(playerpos.x - distancetoplayer.x);
		currentcam.y = -(playerpos.y - distancetoplayer.y);
	}

	App->render->camera.x = currentcam.x;
	App->render->camera.y = currentcam.y;

}

//returns a float corresponding to the new Camera X position Note it doesn't reset camvel
float j1Scene::CameraGoToTarget(SDL_Rect camera, fPoint target)
{
	float newcamX = camera.x;
	float playervel = App->player->GetVelocity().x;
	float camdisplacementvel = playervel * 3;

	if (camaccel.x > 1.0f)camaccel.x = 1.0f;

	if (target.x > -camera.x + (camera.w / 2))
	{
		newcamX -= camdisplacementvel * camaccel.x;
		//LOG("Cam --, positon: %f", newcamX);
	}
	else if (target.x < -camera.x + (camera.w / 2))
	{
		newcamX += camdisplacementvel * camaccel.x;
		//LOG("Cam ++, positon: %f", newcamX);

	}
	camaccel.x += 0.05f;//change this value to change camera accel
	//TODO take the increment of cam vel (cam accel) and put it in the configuration xml

	if (-camera.x + (camera.w / 2) <= target.x + (camdisplacementvel / 2) + 1 && -camera.x + (camera.w / 2) >= target.x - (camdisplacementvel / 2) - 1)
	{
		newcamX = -(target.x - (camera.w / 2));
		//LOG("Cam pinned, positon: %f", newcamX);
		camaccel = { 0.0f,0.0f };
	}


	return newcamX;
}



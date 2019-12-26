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
#include "j1Collision.h"
#include "level_manager.h"
#include "Particles.h"
#include "j1PathFinding.h"
#include "j1EntityManager.h"
#include "Player.h"
#include "Coin_G.h"
#include "UI_Elements.h"

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
	App->render->camera.x = -(App->entity_m->player->pos.x - (App->render->camera.w / 2));
	App->render->camera.y = -(App->entity_m->player->pos.y - (App->render->camera.h / 2));
	teleport_sfx = App->audio->LoadFx("audio/fx/casting_charge_matter_grow_04.wav");
	jump_sfx = App->audio->LoadFx("audio/fx/ferba_says_huh.wav");
	death_sfx = App->audio->LoadFx("audio/fx/mud_splat_heavy_03.wav");
	coin_sfx = App->audio->LoadFx("audio/fx/coin_pickup.wav");
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
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		App->displayMapInfo = !App->displayMapInfo;

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		blit_colliders = !blit_colliders;
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		App->entity_m->player->SetGodmode();
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
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		App->ToggleCapping();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->AddVolume(-60.0f*dt);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->AddVolume(60.0f*dt);
	}

	//TODO delete debug coin key
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
		AnimatedParticle* p = new AnimatedParticle("Coin_G", false, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, { -50.0f,-200.0f }, 1.0f, App->entity_m->player->texture, 2.0f, { 0.0f,0.0f }, { -50.0f,-43.0f });
		App->entity_m->AddEntity(p);
		
	}
	//TODO delete debug coin key
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) 
	{
		AnimatedParticle* q = new AnimatedParticle("Coin_G", false, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 2.0f, { 0.0f,0.0f }, { -50.0f,-43.0f });
		App->entity_m->AddEntity(q);
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		Coin_G* coin = new Coin_G({ App->entity_m->player->pos.x + 200,App->entity_m->player->pos.y });
		App->entity_m->AddEntity(coin);
	}
	//End of particle debug keys=======================================

	RepositionCamera(dt);
	//camera boundaries
	//x coordinate
	if (-App->render->currentcam.x < 0)App->render->currentcam.x = 0;
	else if (-App->render->currentcam.x + App->render->camera.w > App->map->data.width * App->map->data.tile_width)
		App->render->currentcam.x = -(App->map->data.width * App->map->data.tile_width - App->render->camera.w);

	//y coordinate
	if (-App->render->currentcam.y < 0)App->render->currentcam.y = 0;
	else if (-App->render->currentcam.y + App->render->camera.h > App->map->data.height * App->map->data.tile_height)
		App->render->currentcam.y = -(App->map->data.height * App->map->data.tile_height - App->render->camera.h);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	if(draw)App->map->Draw();
	bool ret = true;
	
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}

//this function moves the camera around
void j1Scene::RepositionCamera(float dt)
{
	////variables-------------------------------------------------------------------
	fPoint playerpos = App->entity_m->player->pos;
	bool playerfliped = App->entity_m->player->fliped;

	//int scale = App->win->GetScale();
	int camW = App->render->camera.w;
	int camH = App->render->camera.h;
	fPoint target;

	//CamState lastcamstate = camstate;

	if (!App->entity_m->player->GetGodmode() && (App->collision->GroundCollision() || start)) //if its colliding with the ground execute camera movement
	{


		if (!playerfliped)
			target.x = playerpos.x + (camW * 0.1f); //change the number multiplying by the camW to set the distance of the taget from the player when is looking right
		else target.x = playerpos.x - (camW * 0.1f);//change the number multiplying by the camW to set the distance of the taget from the player when is looking left

		target.y = playerpos.y;
		App->render->currentcam.y = -target.y + (camH * 2 / 3);
		App->render->currentcam.x = CameraGoToTarget(App->render->currentcam, {camW,camH}, target, dt);


		distancetoplayer.x = playerpos.x - (-App->render->currentcam.x);

		distancetoplayer.y = playerpos.y - (-App->render->currentcam.y);
		start = false;
	}
	else
	{
		App->render->currentcam.x = -(playerpos.x - distancetoplayer.x);
		App->render->currentcam.y = -(playerpos.y - distancetoplayer.y);
	}

}

//returns a float corresponding to the new Camera X position Note it doesn't reset camvel
float j1Scene::CameraGoToTarget(fPoint campos,iPoint cammeasures, fPoint target,float dt)
{
	float newcamX = campos.x;
	float playervel = App->entity_m->player->speed.x;
	float camdisplacementvel = playervel * 3*dt;

	if (camaccel.x > 1.0f)camaccel.x = 1.0f;

	if (target.x > -campos.x + (cammeasures.x / 2))
	{
		newcamX -= camdisplacementvel * camaccel.x;
		//LOG("Cam --, positon: %f", newcamX);
	}
	else if (target.x < -campos.x + (cammeasures.x / 2))
	{
		newcamX += camdisplacementvel * camaccel.x;
		//LOG("Cam ++, positon: %f", newcamX);

	}
	camaccel.x += 3.125f*dt;//change this value to change camera accel

	//TODO take the increment of cam vel (cam accel) and put it in the configuration xml

	if (-campos.x + (cammeasures.x / 2) <= target.x + (camdisplacementvel / 2) + 1 && -campos.x + (cammeasures.x / 2) >= target.x - (camdisplacementvel / 2) - 1)
	{
		newcamX = -(target.x - (cammeasures.x / 2));
		//LOG("Cam pinned, positon: %f", newcamX);
		camaccel = { 0.0f,0.0f };
	}


	return newcamX;
}



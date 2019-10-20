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
	App->map->LoadNew(App->map->map_name.GetString());
	App->render->camera.x = App->player->GetPosition().x;
	App->render->camera.y = App->player->GetPosition().y;
	camvelocity = { 0.0f,0.0f };
	arrivedtoline = false;
	snapping = false;
	reload = false;
	currentlevel = LEVEL1;

	//TODO load this from xml

	//load loadingscreen textures
	loading.hexagonLogo = App->tex->Load("textures/hexagon.png");
	loading.externalLogo = App->tex->Load("textures/external.png");
	loading.internalLogo = App->tex->Load("textures/internal.png");
	loading.loadingText = App->tex->Load("textures/loading.png");
	loading.imageLogo = App->tex->Load("textures/loadingimage.png");

	//load loadingscreen
	uint windSizeW;
	uint windSizeH;
	App->win->GetWindowSize(windSizeW, windSizeH);
	textcenter.x = windSizeW / 2;
	textcenter.y = (windSizeH / 2) - 100;
	symbolcenter.x =  windSizeW / 2;
	symbolcenter.y = (windSizeH / 2) + 100;
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

	
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		reload = true;//When reload=true, reloads a map

	




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
	LoadNewLevel(textcenter, symbolcenter);
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(loading.externalLogo);
	App->tex->UnLoad(loading.internalLogo);
	App->tex->UnLoad(loading.hexagonLogo);
	App->tex->UnLoad(loading.loadingText);
	App->tex->UnLoad(loading.imageLogo);

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
		target.x = playerpos.x + (camW * 0.1f); //change the number multiplying by the camW to set the distance of the taget from the player when is looking right
	else target.x = playerpos.x - (camW * 0.1f);//change the number multiplying by the camW to set the distance of the taget from the player when is looking left

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

	if (target.x > -camera.x + (camera.w / 2))
	{
		newcamX -= camdisplacementvel * camvelocity.x;
		//LOG("Cam --, positon: %f", newcamX);
	}
	else if (target.x < -camera.x + (camera.w / 2))
	{
		newcamX += camdisplacementvel * camvelocity.x;
		//LOG("Cam ++, positon: %f", newcamX);

	}
	camvelocity.x += 0.05f;//change this value to change camera accel
	//TODO take the increment of cam vel (cam accel) and put it in the configuration xml

	if (-camera.x + (camera.w / 2) <= target.x + (camdisplacementvel / 2) + 1 && -camera.x + (camera.w / 2) >= target.x - (camdisplacementvel / 2) - 1)
	{
		newcamX = -(target.x - (camera.w / 2));
		//LOG("Cam pinned, positon: %f", newcamX);
		camvelocity = { 0.0f,0.0f };
	}


	return newcamX;
}


void j1Scene::LoadNewLevel(iPoint textcenterpos, iPoint symbolcenterpos)
{
	bool ret = true;
	if (reload)
	{
		uint textwidth = 0;
		uint textheight = 0;
		uint symbolwidth = 0;
		uint symbolheigth = 0;

		App->tex->GetSize(loading.loadingText, textwidth, textheight);//gets the width and height of the texture
		App->tex->GetSize(loading.externalLogo, symbolwidth, symbolheigth);//gets the width and height of the texture

		iPoint textpos;//converts the center position to the upper left corner pos of the texture
		textpos.x = textcenterpos.x - (textwidth / 2);
		textpos.y = textcenterpos.y - (textheight / 2);
		iPoint symbolpos;//converts the center position to the upper left corner pos of the texture
		symbolpos.x = symbolcenter.x - (symbolwidth / 2);
		symbolpos.y = symbolcenter.y - (symbolheigth / 2);


		if (loading.currenttime >= loading.fadetime)//fade time 1 sec or 60 frames
			loading.fadeended = true;

		int alpha = (255 / loading.fadetime) * loading.currenttime; //every sec alpha goes from 0 to 255
		SDL_Rect screen = App->render->viewport;
		switch (loading.fade)
		{
		case FADE_UNKNOWN:
			loading.fade = FADE_IN;
			break;
		case FADE_IN:

			App->render->DrawQuad(screen, 0, 0, 0, alpha, true, false);

			if (loading.fadeended)//finished fading in
			{
				App->render->DrawQuad(screen, 0, 0, 0, 255, true, false);
				loading.currenttime = 0;
				loading.fade = FADE_CHANGINGLVL;
				loading.fadeended = false;
			}
			else
				loading.currenttime++;

			break;
		case FADE_CHANGINGLVL:
			App->render->DrawQuad(screen, 0, 0, 0, 255, true, false);

			//blit the loading textures
			App->render->Blit(loading.loadingText, textpos.x, textpos.y, NULL, NULL, NULL, NULL, 0, 0);
			SDL_RenderCopy(App->render->renderer, loading.imageLogo, NULL, NULL);
			App->render->Blit(loading.externalLogo, symbolpos.x, symbolpos.y, NULL, NULL, NULL, NULL, 0, 0, loading.degrees * 1.5, symbolwidth / 2, symbolheigth / 2);
			App->render->Blit(loading.internalLogo, symbolpos.x, symbolpos.y, NULL, NULL, NULL, NULL, 0, 0, -loading.degrees, symbolwidth / 2, symbolheigth / 2);
			App->render->Blit(loading.hexagonLogo, symbolpos.x, symbolpos.y, NULL, NULL, NULL, NULL, 0, 0, loading.degrees, symbolwidth / 2, symbolheigth / 2);
			loading.degrees++;


			if (loading.transition == 120)//loads the next map
			{

				switch (currentlevel)
				{
				case NOLEVEL:
					LOG("could not load a level, switching to default one.");
					if (App->map->ReloadMap("hello2.tmx"))
						LOG("Could not load default level. Error.");
					break;
				case LEVEL1:
					currentlevel = LEVEL2;
					App->map->ReloadMap("first_level.tmx");


					break;
				case LEVEL2:
					currentlevel = LEVEL3;
					App->map->ReloadMap("second_level.tmx");


					break;
				case LEVEL3:
					currentlevel = LEVEL4;
					App->map->ReloadMap("orthogonal-outside.tmx");


					break;
				case LEVEL4:
					currentlevel = LEVEL1;
					App->map->ReloadMap("first_level.tmx");


					break;
				}



			}

			App->player->ResetPlayerToStart();

			if (loading.transition >= 360)
			{
				loading.transition = 0;
				loading.fade = FADE_OUT;
			}
			loading.transition++;
			break;

		case FADE_OUT:
			App->render->DrawQuad(screen, 0, 0, 0, 255 - alpha, true, false);

			if (loading.fadeended)//finished fading in
			{
				loading.currenttime = 0;
				loading.fade = FADE_ENDED;
			}
			else
				loading.currenttime++;
			break;
		case FADE_ENDED:
			loading.fade = FADE_UNKNOWN;
			loading.degrees = 0;
			loading.transition = 0;
			loading.fadeended = false;
			reload = false;
			break;
		default:
			break;
		}

	}

}



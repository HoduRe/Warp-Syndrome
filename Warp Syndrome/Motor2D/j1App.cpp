#include <iostream> 

#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "level_manager.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Grenade.h"
#include "j1PathFinding.h"
#include "j1Console.h"
#include "Particles.h"
#include "transitions.h"
#include "j1EntityManager.h"
#include "j1GUI.h"
#include "j1Fonts.h"
#include "j1SceneManager.h"


// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	collision = new j1Collision();
	level_m = new j1LevelManager();
	transitions = new j1Transitions();
	pathfinding = new j1PathFinding();
	entity_m = new j1EntityManager();
	console = new j1Console();
	gui = new j1GUI();
	font = new j1Fonts();
	scene_manager = new j1SceneManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(font);
	AddModule(level_m);
	AddModule(map);
	AddModule(collision);
	AddModule(scene);
	AddModule(pathfinding);
	AddModule(entity_m);
	AddModule(gui);
	AddModule(scene_manager);
	AddModule(console);
	AddModule(transitions);//has to be always directly before render
	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module,bool enabled)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		save_game.create(app_config.child("saves").child_value());
		load_game.create(save_game.GetString());

		new_max_framerate = app_config.child("framerate_cap").attribute("value").as_uint();
		dt = 1 / new_max_framerate;
	}

	if (ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	//Calculate the dt: differential time since last frame
	original_dt=dt = frame_time.ReadSec();
	if (paused) dt = 0.0f;
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;



	if (!displayMapInfo)
	{
		const char* vSynctext;
		if (vSyncActivated)vSynctext = "ON";
		else vSynctext = "OFF";
		const char* cappingText;
		if (capping)cappingText = "ON";
		else cappingText = "OFF";
		p2SString title("--Warp Syndrome-- FPS: %i Av.FPS: %.2f Last Frame Ms: %02u  Cap: %s Vsync: %s Time since startup: %.3f Frame Count: %lu ",
			frames_on_last_update, avg_fps, last_frame_ms, cappingText, vSynctext, seconds_since_startup, frame_count);
		App->win->SetTitle(title.GetString());

	}
	else
	{
		int coins = -1;
		int lives = -1;
		int score = -1;
		float ingametime = -1.0f;
		if (App->entity_m->player != nullptr)
		{
			coins = App->entity_m->player->coins;//TODO delete these 2 lines of code, they are made for debugging purposes
			lives = App->entity_m->player->lives;
			score = App->entity_m->player->score;
			ingametime = App->entity_m->player->ingame_time;
		}

		p2SString title("--Warp Syndrome-- Map:%dx%d Tiles:%dx%d Tilesets:%d Name: %s Coins: %i Lives: %i Score: %i Ingame Time: %.2f",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(), App->map->data.name.GetString(),
			coins,lives,score,ingametime);
		App->win->SetTitle(title.GetString());
	}
	//p2SString previoustitle = SDL_GetWindowTitle(win->window);
	//static char newtitle[256];
	//sprintf_s(newtitle, 256,"%s//%s", previoustitle.GetString(), title.GetString());
	//App->win->SetTitle(newtitle);

	//Use SDL_Delay to make sure you get your capped framerate
	uint32 delay = (1000 / new_max_framerate) - last_frame_ms;
	j1PerfTimer perf;
	if (last_frame_ms < (1000 / new_max_framerate) && capping == true)
	{
		perf.Start();
		SDL_Delay(delay);
		LOG("We waited for %u milliseconds and got back in %f", delay, perf.ReadMs());
	}

	//Measure accurately the amount of time it SDL_Delay actually waits compared to what was expected

}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("AppPreUpdate", Profiler::Color::Black);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("AppUpdate", Profiler::Color::Yellow);

	bool ret = true;
	p2List_item<j1Module*>* m_item;
	m_item = modules.start;
	j1Module* pModule = NULL;

	for (m_item = modules.start; m_item != NULL && ret == true; m_item = m_item->next)
	{
		pModule = m_item->data;

		if (pModule->active == false) {
			continue;
		}

		if (dt > 0.5f)dt = 0.016f;//caps dt to maximum 0.5 sec*frame because when loading a new map during the 1st frame the dt is so big that causes issues
		ret = m_item->data->Update(dt);

	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("AppPostUpdate", Profiler::Color::Orange);

	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?


	want_to_save = true;
}

// ---------------------------------------
void j1App::GetSaveGames(p2List<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while (item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if (ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if (ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

void j1App::ToggleCapping()
{
	if (capping)capping = false;
	else capping = true;
}

bool j1App::NewCap(int cap)
{
	bool ret = false;
	if (cap >= 30 && cap <= 120)
	{
		new_max_framerate = cap;

		pugi::xml_document	config_file;
		pugi::xml_node		config;
		pugi::xml_node		app_config;

		config = LoadConfig(config_file);

		if (config.empty() == false)
		{
			app_config = config.child("app");
			app_config.child("framerate_cap").attribute("value") = cap;
			ret = true;
			config_file.save_file("config.xml");
			config_file.reset();
			LOG("Succesfully changed frame capping to %i", cap);
		}
		
	}
	else
	{
		LOG("ERROR! FPS Capping to %i is out of safe range.", cap);
		LOG("Safe rage goes bewteen 30 & 120");

		ret = true;
	}
	return ret;
}

p2SString j1App::GetLoadGameString()
{
	return load_game;
}

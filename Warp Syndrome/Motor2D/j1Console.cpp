#include "j1App.h"
#include "j1Window.h"
#include "j1Console.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1GUI.h"
#include "Static_text.h"
#include "editable_text.h"
#include "static_image.h"
#include "j1SceneManager.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "Player.h"


Command::Command(const char* name, j1Module* callback, uint min_n_arguments, uint max_n_arguments) :name(name), callback(callback), min_n_arguments(min_n_arguments), max_n_arguments(max_n_arguments)
{}
Command::~Command()
{
	name.Clear();
	max_n_arguments = min_n_arguments = 0;
	callback = nullptr;
}
void Command::CommandCallback(p2DynArray<p2SString>* arguments)
{
	callback->OnCommand(this, arguments);
}



j1Console::j1Console() : j1Module()
{
	name.create("console");
	console_opened = false;
}

// Destructor
j1Console::~j1Console() {}

// Called before render is available
bool j1Console::Awake(pugi::xml_node& config)
{
	LOG("Loading console");
	bool ret = true;

	return ret;
}
bool j1Console::Start()
{
	output_text.Clear();
	output_text = "PRUEBAS";

	//Creates commands------------------------------------------------------------------------
	CreateCommand("list", this, 0, 0); //displays a list of commands
	CreateCommand("map", this, 1, 1); //tries to load the map with the imput name
	CreateCommand("god_mode", this, 0, 1);//if no arguments are imput, toggles god mode, else sets god mode to argument
	CreateCommand("fps", this, 0, 1);//if no argument is given, displays the current frame cap, else sets the cap to the fps given, if inside limits
	CreateCommand("quit", this, 0, 1);//quits the game
	return true;
}
bool j1Console::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)//Toggles the console
	{
		if (!console_opened)
			OpenConsole();
		else CloseConsole();
	}


	return true;
}

bool j1Console::Update(float dt)
{
	//debug keys //TODO delete this when the imput text works
	if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN)//list
	{
		const char* t = "List";
		ReceiveText(t);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)//map
	{
		const char* t = "map second_levelo.tmx";
		ReceiveText(t);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)//god_mode
	{
		const char* t = "god_mode on";
		ReceiveText(t);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_3) == KEY_DOWN)//fps
	{
		const char* t = "fps 20";
		ReceiveText(t);
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN)//fps
	{
		const char* t = "quit";
		ReceiveText(t);
	}

	return true;
}

bool j1Console::PostUpdate()
{
	bool ret = true;
	if (App->scene_manager->currentloop != GameCycle::G_C_INGAME && console_opened)
		CloseConsole();
	if (quit)
		ret = false;
	return ret;
}

// Called before quitting
bool j1Console::CleanUp()
{
	LOG("Unloading console");
	p2List_item<Command*>* item = commands.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	commands.clear();
	output_text.Clear();
	console_parent = nullptr;
	return true;
}

//creates the ui elements of the console
bool j1Console::OpenConsole()
{
	//texture = App->tex->Load("textures/atlas.png");
	uint w;
	uint h;
	uint escale = App->win->GetScale();
	App->win->GetWindowSize(w, h);
	w *= escale;
	h *= escale;
	UI* element;
	UI* focus_element;
	SDL_Rect rect = { 0,0,(int)w,(int)h };
	console_parent = App->gui->AddUIElement(new Static_Text(0, 0, nullptr, output_text.GetString()));
	console_parent->listeners.PushBack(this);
	focus_element = App->gui->AddUIElement(new Editable_Text(50, 80, console_parent, w));
	focus_element->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Static_Image(0.0f, 0.0f, console_parent, texture, &rect));
	element->listeners.PushBack(this);

	App->gui->focus = App->gui->UI_list.At(App->gui->UI_list.find(focus_element));;
	console_opened = true;
	return true;
}
bool j1Console::CloseConsole()
{
	App->gui->DeleteWithParent();
	console_opened = false;
	return true;
}
bool j1Console::CreateCommand(const char* command, j1Module* callback, uint min_n_arguments, uint max_n_arguments)
{
	Command* comm = new Command(command, callback, min_n_arguments, max_n_arguments);
	commands.add(comm);
	return true;

}
bool j1Console::OnListen(UI* element, UICallbackState state)
{
	if (state == UICallbackState::UI_CALLBACK_CLICKED)//TODO change this to be enter to accept command
	{
		if (element->type == UIType::UI_TYPE_EDITABLE_TEXT)
		{

		}
	}
	return true;
}
bool j1Console::OnCommand(Command* command, p2DynArray<p2SString>* arguments)
{
	if (command->name == "list")
	{
		//display all the command names on console
	}
	else if (command->name == "map")
	{
		App->map->ReloadMap(arguments->At(0)->GetString());
	}
	else if (command->name == "god_mode")
	{
		if (arguments->Count() == 0)//toggles god mode
		{
			App->entity_m->player->SetGodmode();
		}
		else//sets god mode from an argument
		{
			if (*arguments->At(0) == "on")
				App->entity_m->player->SetGodmode(true);

			if (*arguments->At(0) == "off")
				App->entity_m->player->SetGodmode(false);
		}
	}
	else if (command->name == "fps")
	{
		if (arguments->Count() == 0)//no arguments
		{
			//display fps info
		}
		else
		{
			int cap = std::stoi(arguments->At(0)->GetString());
			App->NewCap(cap);
		}
	}
	else if (command->name == "quit")
	{
		LOG("QUITTING");
		quit = true;
	}

	return true;
}

//receives a line of text and interprets it as a command
bool j1Console::ReceiveText(const char* text)
{
	bool command_sent = false;
	p2SString c = TransformToLower(text);//transforms the text to lowercase
	const char* input_text = c.GetString();
	p2DynArray<p2SString>* separated_words = ReturnWords(input_text);//returns the words in separated in an array

	//tries to find a command that matches the input
	p2List_item<Command*>* item = commands.start;
	while (item != NULL && !command_sent &&separated_words->Count()>0)
	{

		if (item->data->name == *separated_words->At(0))//command found!
		{
			separated_words->Pop(*separated_words->At(0));//deletes the first word (its the command and we not longer need it)


			if (separated_words->Count() >= item->data->min_n_arguments && separated_words->Count() <= item->data->max_n_arguments)
			{
				//right number of arguments
				item->data->CommandCallback(separated_words);
				command_sent = true;
			}
			else
			{
				//ERROR! Wrong number of arguments
				//Display error
			}


		}
		else
		{
			//ERROR! Command not found
			//Display error

		}

		item = item->next;
	}

	separated_words->Clear();
	delete separated_words;

	return true;
}

p2SString j1Console::TransformToLower(const char* text)
{
	std::string string = text;
	std::string newstring;
	for (int i = 0; i < string.length(); i++)
	{
		int c = (tolower(string.at(i)));
		newstring += c;
	}

	p2SString s = newstring.data();
	return s;
}


p2DynArray<p2SString>* j1Console::ReturnWords(const char* text)
{
	p2DynArray<p2SString>* list = new p2DynArray<p2SString>;
	std::string aux;
	for (int i = 0; i <= strlen(text); i++)
	{
		if (isspace(text[i]) == 0 && i != strlen(text))//if it isn't a space
		{
			aux.push_back(text[i]);
		}
		else
		{
			if (aux.length() > 0)//we do this in case that there are 2 spaces together
			{
				p2SString s = (aux.data());
				list->PushBack(s);
			}
			aux.clear();

		}
	}

	return list;

}
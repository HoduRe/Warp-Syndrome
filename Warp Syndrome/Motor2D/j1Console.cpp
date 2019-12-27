#include "j1App.h"
#include "j1Console.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1GUI.h"
#include "Static_text.h"
#include "editable_text.h"


Command::Command(const char* name, j1Module* callback, uint n_arguments) :name(name), callback(callback), n_arguments(n_arguments)
{}
Command::~Command()
{
	name.Clear();
	n_arguments = 0;
	callback = nullptr;
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
	output_text = "pruebas";
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

bool j1Console::Update(float dt) {

	return true;
}

bool j1Console::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Console::CleanUp()
{
	LOG("Unloading console");
	p2List_item<Command*>* item = commands.start;
	
	while (item!=NULL)
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
	UI* element;
	console_parent = App->gui->AddUIElement(new Static_Text(170, 70, nullptr, output_text.GetString()));
	console_parent->listeners.PushBack(this);
	element = App->gui->AddUIElement(new Editable_Text(180,80,console_parent,200));
	element->listeners.PushBack(this);
	console_opened = true;
	return true;
}
bool j1Console::CloseConsole()
{
	App->gui->DeleteWithParent();
	console_opened = false;
	return true;
}
bool j1Console::CreateCommand(const char* command, j1Module* callback, uint n_arguments)
{
	Command* comm = new Command(command, callback, n_arguments);
	commands.add(comm);
	return true;

}
bool j1Console::OnListen(UI* element, UICallbackState state)
{
	if (state == UICallbackState::UI_CALLBACK_CLICKED)//TODO change this to be enter to accept command
	{

	}
	return true;
}
bool j1Console::OnCommand(Command* command)
{
	return true;
}

#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "j1GUI.h"
#include "UI_Elements.h"

// ----------------------------------------------------
class Command
{
public:
	Command(const char* name, j1Module* callback, uint n_arguments);
	~Command();

private:
	p2SString name;
	uint n_arguments;
	j1Module* callback;

};

class j1Console : public j1Module
{
public:

	j1Console();

	// Destructor
	virtual ~j1Console();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);
	bool Start();
	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OpenConsole();
	bool CloseConsole();
	bool CreateCommand(const char* command, j1Module* callback, uint n_arguments);

	bool OnListen(UI* element, UICallbackState state);
	bool OnCommand(Command* command);


private:
	p2List<Command*> commands;
	p2SString output_text;
	bool console_opened;
	UI* console_parent;
};

#endif // __j1CONSOLE_H__
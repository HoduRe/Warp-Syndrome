#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "j1GUI.h"
#include "UI_Elements.h"
#include "Static_text.h"

// ----------------------------------------------------
class Command
{
public:
	Command(const char* name, j1Module* callback, uint min_n_arguments,uint max_n_arguments);
	~Command();
	void CommandCallback(p2DynArray<p2SString>* arguments);

public:
	p2SString name;
	uint min_n_arguments;
	uint max_n_arguments;
private:
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
	bool CreateCommand(const char* command, j1Module* callback, uint min_n_arguments, uint max_n_arguments);

	bool OnListen(UI* element, UICallbackState state);
	bool OnCommand(Command* command, p2DynArray<p2SString>* arguments);
	bool ReceiveText(const char* text);
	bool AddTextToOutput(const char* text);

	//transforms the letters of a string to their lowercase version
	p2SString TransformToLower(const char* text);
	//given a string returns the list of words in that command
	p2DynArray<p2SString>* ReturnWords(const char* text);


private:
	p2List<Command*> commands;
	p2SString output_text;
	Static_Text* outputbox;
	bool console_opened;
	UI* console_parent;
	SDL_Texture* texture;
	SDL_Rect outputrect;
	bool quit = false;
};

#endif // __j1CONSOLE_H__
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.create("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
	memset(text, NULL, sizeof(text));
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	cursor = -1;
	state = READING_NONE;
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	if (text[CHAR_ARRAY - 2] != NULL) { state = READING_STOP; }
	switch (state) {
	case READING_NONE:
		if (App->gui->focus != nullptr) {
			if (App->gui->focus->data->type == UI_TYPE_EDITABLE_TEXT && state == READING_NONE) {
				state = READING_START;
				cursor = -1;
			}
		}
		break;
	case READING_START:
		memset(text, NULL, sizeof(text));
		SDL_StartTextInput();
		state = READING_ONGOING;
		if (App->gui->focus != nullptr) {
			if (App->gui->focus->data->type != UI_TYPE_EDITABLE_TEXT) { state = READING_STOP; }
		}
		else { state = READING_STOP; }
		break;
	case READING_ONGOING:
		if (GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) { state = READING_STOP; }
		else if (GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && cursor != -1) {
			cursor--;
		}
		else if (GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && cursor != CHAR_ARRAY - 2 && text[cursor + 1] != NULL) {
			cursor++;
		}
		else if (GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && cursor != -1) {
			int i = cursor;
			text[cursor] = NULL;
			while (text[i + 1] != NULL) {
				text[i] = text[i + 1];
				i++;
			}
			text[i] = NULL;
			cursor--;
		}
		else if (GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && text[cursor + 1] != NULL) {
			int i = cursor + 1;
			text[i] = NULL;
			while (text[i + 1] != NULL) {
				text[i] = text[i + 1];
				i++;
			}
			text[i] = NULL;
		}
		if (App->gui->focus != nullptr) {
			if (App->gui->focus->data->type != UI_TYPE_EDITABLE_TEXT) { state = READING_STOP; }
		}
		else { state = READING_STOP; }
		break;
	case READING_STOP:
		SDL_StopTextInput();
		state = READING_NONE;
		break;
	}

	int i = 0;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_TEXTINPUT:
			while (event.text.text[i] != NULL) {
				i++;
			}
			AddLetter(event.text.text[i - 1]);
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			int scale = App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			break;
		}
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

void j1Input::AddLetter(char newchar) {
	if (text[cursor + 1] == NULL) {
		text[cursor + 1] = newchar;
	}
	else {
		int length = CHAR_ARRAY - 1;
		while (length != cursor) {
			text[length] = text[length - 1];
			length--;
		}
		text[cursor + 1] = newchar;
	}
	cursor++;
}

int j1Input::GetCursor() { return cursor; }
void j1Input::SetCursor(int i) { cursor = i; }

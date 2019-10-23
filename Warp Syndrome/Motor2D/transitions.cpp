#include "transitions.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "level_manager.h"

j1Transitions::j1Transitions() {}

j1Transitions::~j1Transitions() {}

bool j1Transitions::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Transitions::Start()
{
	//load loadingscreen textures
	hexagonLogo = App->tex->Load("textures/hexagon.png");
	externalLogo = App->tex->Load("textures/external.png");
	internalLogo = App->tex->Load("textures/internal.png");
	loadingText = App->tex->Load("textures/loading.png");
	imageLogo = App->tex->Load("textures/loadingimage.png");

	//load loadingscreen
	uint windSizeW;
	uint windSizeH;
	App->win->GetWindowSize(windSizeW, windSizeH);
	textcenter.x = windSizeW / 2;
	textcenter.y = (windSizeH / 2) - 100;
	symbolcenter.x = windSizeW / 2;
	symbolcenter.y = (windSizeH / 2) + 100;
	actual_state = TS_UNKNOWN;
	actual_transition = TM_UNKNOWN;
	return true;
}

bool j1Transitions::PreUpdate()
{
	return true;
}

bool j1Transitions::Update(float dt)
{

	return true;
}

bool j1Transitions::PostUpdate()
{


	switch (actual_transition)
	{
	case TM_RESTART_LEVEL:
		switch (actual_state)
		{
		case TS_START:
			actual_state = TS_FADE_OUT;
			break;
		case TS_FADE_OUT:
			if (Fade_Out(function_frames_length))
			{
				actual_state = TS_BLACK_SCREEN;
			}
			break;
		case TS_BLACK_SCREEN:
			BlackScreen();
			App->level_m->RestartLevelObjects();
			actual_state = TS_FADE_IN;
			break;
		case TS_FADE_IN:
			if (Fade_In(function_frames_length))
			{
				actual_state = TS_FINISHED;
			}
			break;

		case TS_FINISHED:
			actual_state = TS_UNKNOWN;
			actual_transition = TM_UNKNOWN;
			break;
		case TS_UNKNOWN:
			break;
		}
		break;
	case TM_CHANGE_TO_NEXT_LEVEL:
		switch (actual_state)
		{
		case TS_START:
			actual_state = TS_FADE_OUT;
			break;
		case TS_FADE_OUT:
			if (Fade_Out(function_frames_length))
			{
				actual_state = TS_LOADING_START;
			}
			break;
		case TS_LOADING_START:
			if (LoadingScreen(function_frames_length))
			{
				actual_state = TS_LOADING_FINISH;
				App->level_m->Go_To_Next_Lvl();
			}
			break;
		case TS_LOADING_FINISH:
			if (LoadingScreen(function_frames_length))
			{
				actual_state = TS_FADE_IN;
			}
			break;
		case TS_FADE_IN:
			if (Fade_In(function_frames_length))
			{
				actual_state = TS_FINISHED;
			}
			break;
		case TS_FINISHED:
			actual_state = TS_UNKNOWN;
			actual_transition = TM_UNKNOWN;
			break;
		case TS_UNKNOWN:
			break;
		default:
			break;
		}
		break;
	case TM_UNKNOWN:
		break;
	default:
		break;
	}

	return true;
}

bool j1Transitions::CleanUp()
{
	App->tex->UnLoad(externalLogo);
	App->tex->UnLoad(internalLogo);
	App->tex->UnLoad(hexagonLogo);
	App->tex->UnLoad(loadingText);
	App->tex->UnLoad(imageLogo);
	return true;
}

//Save/Load
bool j1Transitions::Load(pugi::xml_node& ldata)
{
	return true;
}
bool j1Transitions::Save(pugi::xml_node& ldata) const
{
	return true;
}

bool j1Transitions::Fade_Out(uint frames_length)
{
	bool ret = false;
	SDL_Rect screen = App->render->viewport;

	int alpha = (256 / frames_length) * timer;
	App->render->DrawQuad(screen, 0, 0, 0, alpha, true, false);
	timer++;
	if (timer >= frames_length) {
		timer = 0; //reset timer to 0
		ret = true;
	}
	return ret;
}

bool j1Transitions::Fade_In(uint frames_length)
{
	bool ret = false;
	SDL_Rect screen = App->render->viewport;

	int alpha = (256 / frames_length) * timer;
	App->render->DrawQuad(screen, 0, 0, 0, 255 - alpha, true, false);
	timer++;
	if (timer >= frames_length)
	{
		timer = 0; //reset timer to 0
		ret = true;
	}
	return ret;
}

bool j1Transitions::LoadingScreen(uint frames_length)
{
	bool ret = false;
	//variables
	uint textwidth = 0;
	uint textheight = 0;
	uint symbolwidth = 0;
	uint symbolheigth = 0;
	App->tex->GetSize(loadingText, textwidth, textheight);//gets the width and height of the texture
	App->tex->GetSize(externalLogo, symbolwidth, symbolheigth);//gets the width and height of the texture

	iPoint textpos;//converts the center position to the upper left corner pos of the texture
	textpos.x = textcenter.x - (textwidth / 2);
	textpos.y = textcenter.y - (textheight / 2);
	iPoint symbolpos;//converts the center position to the upper left corner pos of the texture
	symbolpos.x = symbolcenter.x - (symbolwidth / 2);
	symbolpos.y = symbolcenter.y - (symbolheigth / 2);
	SDL_Rect screen = App->render->viewport;

	//actual loading
	App->render->DrawQuad(screen, 0, 0, 0, 255, true, false);

	//blit the loading textures
	App->render->Blit(loadingText, textpos.x, textpos.y, NULL, NULL, NULL, NULL, 0, 0);
	SDL_RenderCopy(App->render->renderer, imageLogo, NULL, NULL);
	App->render->Blit(externalLogo, symbolpos.x, symbolpos.y, NULL, NULL, NULL, NULL, 0, 0, degrees * 1.5, symbolwidth / 2, symbolheigth / 2);
	App->render->Blit(internalLogo, symbolpos.x, symbolpos.y, NULL, NULL, NULL, NULL, 0, 0, -degrees, symbolwidth / 2, symbolheigth / 2);
	App->render->Blit(hexagonLogo, symbolpos.x, symbolpos.y, NULL, NULL, NULL, NULL, 0, 0, degrees, symbolwidth / 2, symbolheigth / 2);
	degrees++;

	if (degrees >= frames_length)
	{
		degrees = 0;
		ret = true;

	}
	return ret;
}

bool j1Transitions::BlackScreen(uint frames_length)
{
	bool ret = false;
	SDL_Rect screen = App->render->viewport;
	App->render->DrawQuad(screen, 0, 0, 0, 255, true, false);
	timer++;
	if (timer >= frames_length)
	{
		timer = 0; //reset timer to 0
		ret = true;
	}
	return ret;
}

bool j1Transitions::BlackScreen()
{
	bool ret = false;
	SDL_Rect screen = App->render->viewport;
	App->render->DrawQuad(screen, 0, 0, 0, 255, true, false);
	return ret;
}

void j1Transitions::ChangeTransition(Transition_Mode mode, uint frames_length)
{
	if (actual_transition == TM_UNKNOWN)
	{
		actual_transition = mode;
		actual_state = TS_START;
		function_frames_length = frames_length;
	}

}


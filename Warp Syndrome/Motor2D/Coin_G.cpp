#include "j1App.h"
#include "j1EntityManager.h"
#include "Coin_G.h"
#include "j1Render.h"

Coin_G::Coin_G(fPoint Pos) :Character(EntityType::E_TYPE_COIN_G)
{
	texture = App->tex->Load("textures/Coin_G.png");
	//sets the new coin animation
	Animations* anim = new Animations();
	anim->animationname = "Coin_G";
	anim->animationloop = true;
	anim->AddFrame(0.1f, { 0,0,16,16 }, { 8, 8 });
	anim->AddFrame(0.1f, { 16,0,16,16 }, { 8, 8 });
	anim->AddFrame(0.1f, { 32,0,16,16 }, { 8, 8 });
	anim->AddFrame(0.1f, { 48,0,16,16 }, { 8, 8 });
	anim->AddFrame(0.1f, { 64,0,16,16 }, { 8, 8 });

	animations_list.add(anim);
	currentAnim = animations_list.start;
	currentframe = currentAnim->data->GetCurrentFrame();

	pos = Pos;
}

Coin_G::~Coin_G()
{
	this->CleanUp();
}

bool Coin_G::Update(float dt)
{
	animations_list.start->data->StepAnimation(dt);
	OnCollision();
	return true;
}

bool Coin_G::PostUpdate()
{
	App->render->Blit(texture, pos.x, pos.y, &currentAnim->data->GetCurrentFrame()->animationRect, fliped);
	return true;
}
bool Coin_G::OnCollision()
{
	bool ret = false;
	SDL_Rect r1 =
	{
		pos.x,
		pos.y,
		animations_list.start->data->GetCurrentFrame()->animationRect.w,
		animations_list.start->data->GetCurrentFrame()->animationRect.h
	};

	SDL_Rect r2 =
	{
		App->entity_m->player->pos.x-App->entity_m->player->hitbox_w_h.x*0.5f,
		App->entity_m->player->pos.y - App->entity_m->player->hitbox_w_h.y * 0.5f,
		App->entity_m->player->hitbox_w_h.x,
		App->entity_m->player->hitbox_w_h.y

	};

	if (App->collision->CheckCollisionSimplified(&r1,&r2))//if collision detected
	{
		destroy = true;
		AnimatedParticle* p = new AnimatedParticle("Coin_G", false, { pos.x,pos.y }, { -50.0f,-200.0f }, 1.0f, App->entity_m->player->texture, 2.0f, { 0.0f,0.0f }, { 0.0f,0.0f });
		App->entity_m->AddEntity(p);
		App->entity_m->player->coins += 1;
	}

	return ret;
}

bool Coin_G::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
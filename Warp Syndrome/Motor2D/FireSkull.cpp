#include "j1App.h"
#include "FireSkull.h"
#include "j1Collision.h"
#include "j1Render.h"

Enemy_FireSkull::Enemy_FireSkull(int x, int y) : Enemy(x, y, E_STATE_DEFAULT, E_TYPE_FIRE_SKULL)
{
	last_state = E_STATE_UNKNOWN;
	chase_distance = 25;
	health = 1.0f;
}
Enemy_FireSkull::Enemy_FireSkull(int x, int y, enemy_states startingstate, float aHealth) : Enemy(x, y, startingstate, E_TYPE_FIRE_SKULL)
{
	last_state = E_STATE_UNKNOWN;
	chase_distance = 25;
	health = aHealth;
}


Enemy_FireSkull::~Enemy_FireSkull()
{
	this->CleanUp();
}

bool Enemy_FireSkull::CleanUp()
{
	return true;
}

bool Enemy_FireSkull::PreUpdate()
{
	DoEnable();
	if (enabled) { KillPlayer(); }
	return true;
}

bool Enemy_FireSkull::Update(float dt)
{
	if (health <= 0.0f)
	{
		state = enemy_states::E_STATE_DIE;
	}
	if (enabled)
	{
		fPoint posbuffer = pos;//saves the position before movement

		player_distance = CheckDistance(pos.x, pos.y);//checks the distance in tiles between the enemy and the player
		last_state = state;//last state is the state before changing it during the frame
		anim_state = StepCurrentAnimation(dt);	// steps the current animation and saves the flag about whether its finished or not

		switch (state)//enemy state machine========================================================
		{

			//=========================================================================================
		case E_STATE_DEFAULT: //normal enemy behaviour when doesn't see the player


			if (player_distance <= chase_distance)//change to chasing when sees an enemy
				state = E_STATE_CHASING;

			break;
			//=========================================================================================
			//=========================================================================================
		case E_STATE_CHASING:
			//pathfinding here, execute pathfinding once every 1/2 a sec or so
			//Also move the player in this state, when arrived to the destination tile go to default case
			if (pathfinding_update_rate >= 0.5f)//2 seconds of refresh rate
			{
				DoPathFinding();
				pathfinding_update_rate = 0.0f;

			}

			Move(dt);
			pathfinding_update_rate += dt;
			break;
			//=========================================================================================
			//=========================================================================================
		case E_STATE_ATTACKING:
			//When attack ends go to default
			break;
			//=========================================================================================
			//=========================================================================================
		case E_STATE_DIE:

			//execute die anim
			//when die anim ends, destroy the entity
			destroy = true;
			break;
			//=========================================================================================
			//=========================================================================================
		case E_STATE_UNKNOWN:
			break;
		default:
			break;
		}
		fliped = (FlipCharacter(pos, posbuffer));//flips the player
		CheckAnimation(state, last_state);
	}

	return true;
}

bool Enemy_FireSkull::PostUpdate()
{
	if (enabled)
		Enemy::Draw();
	return true;
}

bool Enemy_FireSkull::Save(pugi::xml_node& data) const
{
	data.append_attribute("state") = (int)state;
	data.append_attribute("last_state") = (int)last_state;
	return true;
}
void Enemy_FireSkull::ChangeAnimation(Elemental_Anim_List animations)
{
	p2List<Animations*>* pAnimList = &animations_list;//pointer to the character's animation list
	p2List_item<Animations*>* currentanim = currentAnim;//pointer to the current animation

	currentanim->data->ResetAnimation();//resets the current animation before changing to another one

	p2List_item<Animations*>* newanim = nullptr;
	switch (animations)
	{
	case EL_ANIM_IDLE:
		newanim = currentanim->data->GetAnimFromName("idle", pAnimList);
		break;
	case EL_ANIM_MOVING:
		newanim = currentanim->data->GetAnimFromName("moving", pAnimList);
		break;
	case EL_ANIM_ATTACKING:
		newanim = currentanim->data->GetAnimFromName("moving", pAnimList);
		break;
	case EL_ANIM_DIE:
		newanim = currentanim->data->GetAnimFromName("idle", pAnimList);
		break;
	case EL_ANIM_UNKNOWN:
		newanim = currentanim->data->GetAnimFromName("", pAnimList);
		break;
	}

	currentAnim = newanim;//sets the current animation of the character

}

void Enemy_FireSkull::CheckAnimation(enemy_states currentstate, enemy_states laststate)//if there is a change in the character state, changes aniamtion
{
	if (currentstate != laststate)
	{
		switch (currentstate)
		{
		case E_STATE_DEFAULT:
			ChangeAnimation(EL_ANIM_IDLE);
			break;
		case E_STATE_CHASING:
			ChangeAnimation(EL_ANIM_MOVING);
			break;
		case E_STATE_ATTACKING:
			ChangeAnimation(EL_ANIM_ATTACKING);
			break;
		case E_STATE_DIE:
			ChangeAnimation(EL_ANIM_DIE);
			break;
		case E_STATE_UNKNOWN:
			ChangeAnimation(EL_ANIM_UNKNOWN);
			break;
		default:
			break;
		}

	}
}
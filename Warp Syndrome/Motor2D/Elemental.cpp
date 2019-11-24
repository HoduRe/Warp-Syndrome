#include "j1App.h"
#include "Elemental.h"
#include "j1Collision.h"

Enemy_Elemental::Enemy_Elemental(int x, int y) : Enemy(x, y,E_STATE_DEFAULT,E_TYPE_ELEMENTAL)
{
	last_state
}
Enemy_Elemental::Enemy_Elemental(int x, int y, enemy_states startingstate) : Enemy(x, y,startingstate, E_TYPE_ELEMENTAL)
{
}


Enemy_Elemental::~Enemy_Elemental() {}


bool Enemy_Elemental::PreUpdate()
{

}

bool Enemy_Elemental::Update(float dt)
{

}

bool Enemy_Elemental::PostUpdate()
{
	Draw();
}

void Enemy_Elemental::Draw()
{

}

void Enemy_Elemental::Move() 
{
}

void Enemy_Elemental::ChangeAnimation(Elemental_Anim_List animations)
{
	p2List<Animations*>* pAnimList = &animations_list;//pointer to the character's animation list //TODO this is now loaded directly from the class, no need for a pointer
	p2List_item<Animations*>* currentanim = currentAnim;//pointer to the current animation//TODO this is now loaded directly from the class, no need for a pointer

	currentanim->data->ResetAnimation();//resets the current animation before changing to another one

	p2List_item<Animations*>* newanim = nullptr;
	switch (animations)
	{
	case EL_ANIM_IDLE:
		newanim = currentanim->data->GetAnimFromName("idle", pAnimList);//TODO put valid animations here
		break;
	case EL_ANIM_MOVING:
		newanim = currentanim->data->GetAnimFromName("idle", pAnimList);
		break;
	case EL_ANIM_ATTACKING:
		newanim = currentanim->data->GetAnimFromName("idle", pAnimList);
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

void Enemy_Elemental::CheckAnimation(enemy_states currentstate, enemy_states laststate)//if there is a change in the character state, changes aniamtion
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
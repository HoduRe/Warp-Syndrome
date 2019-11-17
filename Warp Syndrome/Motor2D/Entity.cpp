#include "Entity.h"

//Entity=====================================================
Entity::Entity(EntityType eType) :type(eType)
{
	pos = { 0.0f,0.0f };
	speed = { 0.0f,0.0f };
	texture = nullptr;
	texture_section = { 0,0,0,0 };
	fliped = false;
	destroy = false;
	enabled = false;
	health = 0.0f;
}
Entity::~Entity()
{}

bool Entity::Awake()
{
	return true;
}
 bool Entity::Start()
 {
	 return true;
 }
 bool Entity::PreUpdate(float dt)
 {
	 return true;
 }
 bool Entity::Update(float dt)
 {
	 return true;
 }
 bool Entity::PostUpdate(float dt)
 {
	 return true;
 }
 bool Entity::CleanUp()
 {
	 return true;
 }
 bool Entity::Load(pugi::xml_node&)
 {
	 return true;
 }
 bool Entity::Save(pugi::xml_node&) const
 {
	 return true;
 }
void Entity::Enable(){}
void Entity::Disable(){}
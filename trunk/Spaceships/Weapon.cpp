#include "precompiled.h"
#include "Weapon.h"

Weapon::Weapon(BulletFactory* factory, GameObject* owner)
{
	this->factory = factory;
	fireDuration = 0;
	this->owner = owner;
	lastTimeFired = GetTickCount() - 1;
}

Weapon::~Weapon(void)
{
}

bool Weapon::IsFiring()
{
	bool retvalue = (GetTickCount() - lastTimeFired) < fireDuration;
	return retvalue;
}

void Weapon::Fire(const float x, const float y, const float rotation, Controller* controller)
{
	if(!this->IsFiring())
	{
		lastTimeFired = GetTickCount();
		GameObject* bullet = Fire(x, y, rotation);
		if(bullet != NULL)
		{
			bullet->SetController(controller);
			Message* message = 
				new Message(MessageType::GAME_OBJECT_ACTION, MessageContent::CREATE_OBJECT, (void*)bullet); 
			controller->PassMessage(message);
		}
	}
}

void Weapon::SetFireDuration(int duration)
{
	this->fireDuration = duration;
}

void Weapon::Update()
{

}
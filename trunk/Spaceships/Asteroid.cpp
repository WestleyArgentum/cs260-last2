#include "Asteroid.h"

Asteroid::Asteroid()
{
	updatecounter = 0;
	hitpoints = 300;
	font = NULL;
}

Asteroid::~Asteroid(void)
{
	
}

bool Asteroid::IsDead(){
	float x = this->mobile->GetValue(PhysicsComponent::PositionX);
	float y = this->mobile->GetValue(PhysicsComponent::PositionY);

	if(x < -100 || y < -100 || x > WORLD_SIZE_X + 100 || y > WORLD_SIZE_Y + 100)
	{
		return true;
	}

	if(this->hitpoints <= 0)
		return true;

	if(this->_lifeTime == 0)
		return true;
	
 	return false;
}


void Asteroid::OnUpdate()
{
	KillSlowly();
}

void Asteroid::PreProcessMessages()
{

}
void Asteroid::PostProcessMessages()
{

}

void Asteroid::ProcessMessage(Message* msg)
{
	
	assert(msg != NULL);
	
	if(msg->GetMessageType() == MessageType::GAME_OBJECT_ACTION && 
		msg->GetMessageContent() == MessageContent::COLLISION)
	{
		// get the object that we collided with
		GameObject* obj = (GameObject*)msg->GetExtended();
		assert(obj != NULL);		
		hitpoints -= obj->GetDamage();

		ObjectType type = obj->GetType();

		// TODO:  this sucks too, refactor it.  if/else if/else is not very maintainable
		if(type == ObjectType::ASTEROID)
		{
			delete msg;
			return;
		}
		else if(type == ObjectType::SPACESHIP)
		{
			delete msg;
			return;
		}	
		else if(type == ObjectType::BULLET)
		{
			if(hitpoints <= 0)
			{
				hitpoints = 1;
				// display score
				this->mobile->SetCollider(NULL);
				mobile->SetValue(PhysicsComponent::GeneralVelocity, 1);
				mobile->SetValue(PhysicsComponent::Rotation, 0);
				this->_lifeTime = 100;

				assert(font != NULL);

				TextRenderer* score = new TextRenderer(font);
				score->SetText("100");
				score->SetSize(60, 14);
				delete renderable;
				renderable = (IRenderable*)score;
			}
		}
			

	
	}
	
	delete msg;
	return;
}

void Asteroid::PreGetRenderable()
{
	this->renderable->SetPosition(
		this->mobile->GetValue(PhysicsComponent::PositionX),
		this->mobile->GetValue(PhysicsComponent::PositionY));

	this->renderable->SetRotation(
		mobile->GetValue(PhysicsComponent::Rotation));

}

ObjectType Asteroid::GetType()
{
	return ObjectType::ASTEROID;
}

#include "SpaceShip.h"
#include "Bullet.h"
#include "PlasmaCannon.h"
#include "DeathBlossom.h"
SpaceShip::SpaceShip()
{
	this->hitpoints = SpaceShipBaseHitpoints;
	rateOfFire = 5;
	bulletCounter = 0;
	primaryWeapon = (IWeapon*) new PlasmaCannon(NULL, this);
	secondaryWeapon = (IWeapon*) new  DeathBlossom(new BulletFactory(), this);
}

SpaceShip::~SpaceShip(void)
{
	GlobalDebug::GetInstance()->DbgOut("Deleting SpaceShip");
}

bool SpaceShip::IsDead(){
	int hp = 10;
	if(hp <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float SpaceShip::GetGunPositionX()
{

	return PhysicsCalculator::CalcNewXValue(
		this->mobile->GetValue(PhysicsComponent::Rotation),
		this->renderable->GetCenter().x, 
		30.0f);
}

float SpaceShip::GetGunPositionY()
{
	return PhysicsCalculator::CalcNewYValue(
		this->mobile->GetValue(PhysicsComponent::Rotation),
		this->renderable->GetCenter().y,
		30.0f);
}

GameObject* SpaceShip::FirePrimary()
{
	if(bulletCounter == 0)
	{
		// we can fire
		this->bulletCounter = rateOfFire;
	}
	else
	{
		primaryWeapon->Fire(GetGunPositionX(), GetGunPositionY(), this->mobile->GetValue(PhysicsComponent::Rotation), this->owningController);
	}

	return NULL;
}

GameObject* SpaceShip::FireSecondary()
{
	if(bulletCounter == 0)
	{
		// we can fire
		this->bulletCounter = rateOfFire;
	}
	else
	{
		secondaryWeapon->Fire(GetGunPositionX(), GetGunPositionY(), this->mobile->GetValue(PhysicsComponent::Rotation), this->owningController);
	}

	return NULL;


}

void SpaceShip::OnUpdate()
{
	if(bulletCounter > 0)
	{
		bulletCounter--;
	}
}

void SpaceShip::PreProcessMessages()
{
	this->mobile->SetValue(PhysicsComponent::GeneralVelocity, 0);

}
void SpaceShip::ProcessMessage(Message* msg)
{
	// TODO: massive OCP violation- these things should have their own message handlers
	if(msg->GetMessageType() == MessageType::USER_INPUT)
	{
		int msgContent = msg->GetMessageContent();
		if(msgContent == GameAction::MOVE_UP)
		{
			this->mobile->SetValue(PhysicsComponent::GeneralVelocity, spaceShipVelocity);
		}
		else if(msgContent == GameAction::MOVE_RIGHT)
		{
			float theta = this->mobile->GetValue(PhysicsComponent::Rotation);
			theta += _turnstep;
			if(theta > 1)
				theta -= 1;
			mobile->SetValue(PhysicsComponent::Rotation, theta);
		}
		else if(msgContent == GameAction::MOVE_LEFT)
		{
			float theta = this->mobile->GetValue(PhysicsComponent::Rotation);
			theta -= _turnstep;
			if(theta <= 0)
				theta += 1;
			mobile->SetValue(PhysicsComponent::Rotation, theta);			
		}
		else if(msgContent == GameAction::FIRE_PRIMARY)
		{
			this->FirePrimary();			
		}
		else if(msgContent == GameAction::FIRE_SECONDARY)
		{
			this->FireSecondary();
		}
		
	}
	else if(msg->GetMessageType() == MessageType::GAME_OBJECT_ACTION)
	{
		if(msg->GetMessageContent() == MessageContent::COLLISION)
		{
			GameObject* objectICollidedWith = (GameObject*)msg->GetExtended();

		}
	}
	delete msg;
	
}
void SpaceShip::PostProcessMessages()
{

}

void SpaceShip::PreGetRenderable()
{
	assert(this->renderable != NULL);
	float x, y, r;
	x = mobile->GetValue(PhysicsComponent::PositionX);
	y = mobile->GetValue(PhysicsComponent::PositionY);
	r = mobile->GetValue(PhysicsComponent::Rotation) * 2 * PI;
	renderable->SetPosition(x, y);
	renderable->SetRotation(r);
}

ObjectType SpaceShip::GetType()
{
	return ObjectType::SPACESHIP;
}

void SpaceShip::SetPrimaryWeapon(IWeapon* weapon)
{
	primaryWeapon = weapon;

}
#include "precompiled.h"
#include "DeathBlossomVisitor.h"
#include "Bullet.h"

DeathBlossomVisitor::DeathBlossomVisitor(BulletFactory* factory)
{
	bulletFactory = factory;
	// This value looked the best after careful experimentation.  Lower values make a 
	// very nice spiral pattern, higher values are also a spiral but very loose, close to 
	// a straight line.
	rotationStep = .07;
	currentRotation = 0;
	InitializeStateMap();	
}

void DeathBlossomVisitor::InitializeStateMap()
{
	DeathBlossomState* chargingState = new DeathBlossomState;
	DeathBlossomState* firingState = new DeathBlossomState;
	DeathBlossomState* cooldownState = new DeathBlossomState;

	chargingState->StateName = DeathBlossomStateEnum::CHARGING;
	chargingState->Duration = 1500;
	chargingState->NextState = firingState;

	firingState->StateName = DeathBlossomStateEnum::FIRING;
	firingState->Duration = 3000;
	chargingState->NextState = cooldownState;

	cooldownState->StateName = DeathBlossomStateEnum::COOLDOWN;
	cooldownState->Duration = 1500;
	chargingState->NextState = NULL;

	states[DeathBlossomStateEnum::CHARGING] = chargingState;
	states[DeathBlossomStateEnum::FIRING] = firingState;
	states[DeathBlossomStateEnum::COOLDOWN] = cooldownState;
}

DeathBlossomVisitor::~DeathBlossomVisitor(void)
{
	map<DeathBlossomStateEnum,DeathBlossomState*>::iterator items;
	for(items = states.begin(); items != states.end(); ++items)
	{
		delete items->second;
	}
}

void DeathBlossomVisitor::OnAccept()
{
	stateStartTime = GetTickCount();
	currentState = states[DeathBlossomStateEnum::CHARGING];
}

void DeathBlossomVisitor::OnLeave()
{
	
}

void DeathBlossomVisitor::OnVisit(GameObject* object)
{
	
	if(GetTickCount() - stateStartTime > currentState->Duration)
	{
		stateStartTime = GetTickCount();
		currentState = states[currentState->StateName]->NextState;
	}	

	if(currentState == NULL)
	{
		this->Leave();
		return;
	}

	assert(currentState != NULL);

	Mobile* mobile = object->GetMobile();
	float oldRotation = mobile->GetValue(PhysicsComponent::Rotation);
	float newRotation = oldRotation + rotationStep;
	while(newRotation > 1.0)
		newRotation -= 1.0;
	mobile->SetValue(PhysicsComponent::Rotation, newRotation);

	if(currentState->ShouldBeFiring)
		FireBullets();
}

void DeathBlossomVisitor::ProcessMessage(Message* msg)
{
	// suppress user input so that the spaceship can't move while this is
	// firing.
	if(msg->GetMessageType() != MessageType::USER_INPUT)
	{
		this->objectVisiting->PassMessageDirect(msg);
	}
	else
	{
		delete msg;
	}
}

const float bulletsToFire = 4.0;

void DeathBlossomVisitor::FireBullets()
{
	float x, y, rotation;
	rotation = objectVisiting->GetMobile()->GetValue(PhysicsComponent::Rotation);
	x = objectVisiting->GetMobile()->GetValue(PhysicsComponent::PositionX);
	y = objectVisiting->GetMobile()->GetValue(PhysicsComponent::PositionY);

	for(float i = 0.0; i < bulletsToFire; i++)
	{
		int nextID = IDFactory::GetNextID();
		float newX, newY;
		rotation = rotation + (i / bulletsToFire);
		while(rotation > 1.0)
		{
			rotation -= 1;
		}

		assert(rotation >= 0.0 && rotation <= 1.0);
		newX = PhysicsCalculator::CalcNewXValue(rotation, x, 50);
		newY = PhysicsCalculator::CalcNewYValue(rotation, y, 50);

		Mobile* m = new Mobile();
		GameObject* bullet = new Bullet();
		Collider* c = new Collider();

		m->SetValue(PhysicsComponent::PositionX, newX );
		m->SetValue(PhysicsComponent::PositionY, newY );
		m->SetValue(PhysicsComponent::GeneralVelocity, 12);
		m->SetValue(
			PhysicsComponent::Rotation, rotation);


		c->SetRadius(1);

		bullet->SetID(nextID);
		bullet->SetLifetime(30);
		bullet->SetDamage(500);

		m->SetCollider(c);
		m->ShouldBindToWorld(false);
		bullet->SetMobile(m);
		bullet->SetParent(objectVisiting);
		this->owningController->PassMessage(
			new Message(MessageType::GAME_OBJECT_ACTION, 
			MessageContent::CREATE_OBJECT, (void*)bullet)
			);
	}
}
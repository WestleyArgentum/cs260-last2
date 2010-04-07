#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(void)
{
	InitializeCriticalSection(&objectsLock);
};

PhysicsWorld::~PhysicsWorld(void)
{
	DeleteCriticalSection(&objectsLock);
	while(objects.size() > 0)
	{
		Mobile* m = objects.front();
		objects.pop_front();
		delete m;	
	}	
};

void PhysicsWorld::Update()
{
	// move everything
	DoMovement();
	
	// check for collisions 
	HandleCollisions();

	// collisions should have cleared out the list
	assert(objects.size() == 0);
};

void PhysicsWorld::DoMovement()
{
	list<Mobile*>::iterator mobileIter;
	for(mobileIter = objects.begin(); mobileIter != objects.end(); ++mobileIter)
	{
		
		Mobile* current = (Mobile*)*mobileIter;				
		float newX, newY;
		assert(current != NULL);

		float currentX, currentY, currentR, currentVelocity;
		currentX = current->GetValue(PhysicsComponent::PositionX);
		currentY = current->GetValue(PhysicsComponent::PositionY);
		currentR = current->GetValue(PhysicsComponent::Rotation);
		currentVelocity = current->GetValue(PhysicsComponent::GeneralVelocity);
		
		newX = PhysicsCalculator::CalcNewXValue(
			currentR,
			currentX,
			currentVelocity
			);

		newY = PhysicsCalculator::CalcNewYValue(
			currentR,
			currentY,
			currentVelocity
			);


		// enforce world boundaries if we need to
		if(current->IsBoundToWorld())
		{
			if(newX < 0)
			{
				newX = 0;
			}
			else if(newX >= this->worldSizeX)
			{
				newX = this->worldSizeX;
			}

			if(newY < 0)
			{
				newY = 0;
			}
			else if(newY >= this->worldSizeY)
			{
				newY = this->worldSizeY;
			}
		}


#ifdef MOBILE_DEBUG
		GlobalDebug::GetInstance()->DbgOutInt("\r\nMoving object ID:", current->GetId());
		GlobalDebug::GetInstance()->DbgOut("\r\nCurrent stats: \r\n");
		GlobalDebug::GetInstance()->DbgOutFloat("\r\nPositionX: ", currentX);
		GlobalDebug::GetInstance()->DbgOutFloat("\r\nPositionY: ", currentY);
		GlobalDebug::GetInstance()->DbgOutFloat("\r\nRotation: ", currentR);
		GlobalDebug::GetInstance()->DbgOutFloat("\r\nVelocity: ", currentVelocity);
		GlobalDebug::GetInstance()->DbgOutFloat("\r\nMoving to X: ", newX);
		GlobalDebug::GetInstance()->DbgOutFloat("\r\nMoving to Y: ", newY);
		GlobalDebug::GetInstance()->DbgOutInt("\r\nFinished mobing object ID:", current->GetId());
#endif
		current->SetValue(PhysicsComponent::PositionX, newX);
		current->SetValue(PhysicsComponent::PositionY, newY);			
	}
};

void PhysicsWorld::HandleCollisions()
{
	while(objects.size() > 0)
	{
		Mobile* mobileToCheck;
		mobileToCheck = objects.front();
		objects.pop_front();

		list<Mobile*>::iterator subIter;
		Collider* colliderToCheck = mobileToCheck->GetCollider();
				
		//don't check for non-existent colliders.  not every object can collide
		if(colliderToCheck == NULL)
			continue;

		for(subIter = objects.begin(); subIter != objects.end(); ++subIter)
		{
			//iterate over every other collider in the list
			// compare temp to every object in the list
			Mobile* compMobile = (Mobile*)*subIter;
			Collider* compCollider = compMobile->GetCollider();

			// check collision if these point at different objects
			// one if statement here wouldn't be as readable and the compiler will
			// optimize this anyway unless it's a horrible piece of crap
			if(compCollider == NULL)
				continue;
			if(mobileToCheck->GetId() == compMobile->GetId())
				continue;
			if(mobileToCheck->GetId() == compCollider->GetParentID())
				continue;
			if(colliderToCheck->GetParentID() == compMobile->GetId())
				continue;
			
			// see if we collided.  don't check against dead objects
			// or objects that are owned by each other
			if(colliderToCheck->CollidedWith(compCollider))
			{
				if(colliderToCheck->GetOwner()->GetType() == ObjectType::BULLET)
					GlobalDebug::GetInstance()->DbgOut("Bullet collision");

				if(compCollider->GetOwner()->GetType() == ObjectType::BULLET)
					GlobalDebug::GetInstance()->DbgOut("Bullet collision");


				GlobalDebug::GetInstance()->DbgOut("Collision!");

				Message* compColliderMessage = new Message(
					MessageType::GAME_OBJECT_ACTION, 
					MessageContent::COLLISION, 
					colliderToCheck->GetOwner());

				Message* colliderToCheckMessage = new Message(
					MessageType::GAME_OBJECT_ACTION, 
					MessageContent::COLLISION, 
					compCollider->GetOwner());

				colliderToCheck->GetOwner()->SendMessage(colliderToCheckMessage);
				compCollider->GetOwner()->SendMessage(compColliderMessage);			
			}
		}
	}
}

void PhysicsWorld::SetWorldSize(int x, int y)
{
	worldSizeX = x;
	worldSizeY = y;
}

void PhysicsWorld::WrapAround(bool value)
{
	wrapAround = value;
}

list<Mobile*>* PhysicsWorld::GetAllObjects()
{

	list<Mobile*>* temp = new list<Mobile*>(objects);
	return temp;
}

void PhysicsWorld::Init(){
	this->objects.clear();
}

void PhysicsWorld::PassMessage(Message* msg)
{
	if(msg->GetMessageType() == MessageType::GAME_OBJECT_ACTION && 
		msg->GetMessageContent() == MessageContent::DO_PHYSICS)
	{
		Mobile* mobile = (Mobile*)msg->GetExtended();		
		// we should never have passed in a null object under any circumstances
		assert(mobile != NULL);
		this->objects.push_back(mobile);

	}
	delete msg;
}
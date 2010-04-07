#include "Mobile.h"

Mobile::Mobile()
{
	collider = NULL;
	owner = NULL;
	componentValues = new float[PhysicsComponentsCount];

	SecureZeroMemory(componentValues, sizeof(float) * PhysicsComponentsCount);
	bindToWorld = true;
}

Mobile::~Mobile(void)
{
	if(collider != NULL)
	{
		delete collider;
	}

	delete componentValues;
}

GameObject* Mobile::GetOwner()
{
	return owner;
}

void Mobile::SetOwner(GameObject* value)
{
	assert(value != NULL);
	owner = value;
	if(collider != NULL)
	{
		collider->SetOwner(value);
	}
}

int Mobile::GetId()
{
	// every mobile has an owner
	assert(owner != NULL);
	return owner->GetID();
}

Collider* Mobile::GetCollider()
{
	if(collider != NULL)
	{
		collider->SetPosition(GetValue(PhysicsComponent::PositionX), GetValue(PhysicsComponent::PositionY));
	}
	return collider;
}

void Mobile::SetCollider(Collider* collider)
{
	this->collider = collider;
	if(collider != NULL)
	{
		collider->SetOwner(this->GetOwner());
	}
}

void Mobile::SetValue(PhysicsComponent component, float value)
{
	componentValues[component] = value;
}

float Mobile::GetValue(PhysicsComponent component)
{
	return componentValues[component];
}

void Mobile::ShouldBindToWorld(bool value)
{
	bindToWorld = value;
}

bool Mobile::IsBoundToWorld()
{
	return bindToWorld;
}

GameObject* Mobile::GetParent()
{
	assert(owner != NULL);
	return this->owner->GetParent();
}
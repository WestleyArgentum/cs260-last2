#include "precompiled.h"
#include "PlasmaCannon.h"

PlasmaCannon::PlasmaCannon(BulletFactory* factory, GameObject* owner) : Weapon(factory, owner)
{
	this->SetFireDuration(100);
}

PlasmaCannon::~PlasmaCannon(void)
{
}

void PlasmaCannon::CeaseFire()
{
}

GameObject* PlasmaCannon::Fire(const float x, const float y, const float rotation)
{
	int nextID = IDFactory::GetNextID();

	Mobile* m = new Mobile();
	GameObject* bullet = new Bullet();
	Collider* c = new Collider();

	m->SetValue(PhysicsComponent::PositionX, x);
	m->SetValue(PhysicsComponent::PositionY, y);
	m->SetValue(PhysicsComponent::GeneralVelocity, 12);
	m->SetValue(
		PhysicsComponent::Rotation, 
		rotation);
	
	c->SetRadius(1);
	
	bullet->SetID(nextID);
	
	m->SetCollider(c);
	bullet->SetMobile(m);
	bullet->SetParent(owner);
	return bullet;

	return (GameObject*) bullet;

}
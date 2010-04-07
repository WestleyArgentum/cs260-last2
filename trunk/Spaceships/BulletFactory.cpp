#include "precompiled.h"
#include "BulletFactory.h"
#include "Bullet.h"

BulletFactory::BulletFactory(void)
{
}

BulletFactory::~BulletFactory(void)
{
}

GameObject* BulletFactory::GetNewObject()
{
	int nextID = IDFactory::GetNextID();

	// TODO:  this sucks, fix it
		Mobile* m = new Mobile();
		GameObject* bullet = new Bullet();
		Collider* c = new Collider();

	m->SetValue(PhysicsComponent::GeneralVelocity, 12);
	
	c->SetRadius(1);
	
	
	bullet->SetID(nextID);
	m->SetCollider(c);
	bullet->SetMobile(m);
	return bullet;
}

GameObject* BulletFactory::GetBullet(float x, float y, float rotation, GameObject* owner, Controller* owningController)
{
	GameObject* bullet = new Bullet();
	Collider* c = new Collider();
	Mobile* m = new Mobile();
	bullet->SetID(IDFactory::GetNextID());
	m->SetCollider(c);
	bullet->SetMobile(m);
	
	c->SetRadius(collisionRadius);

	m->SetValue(PhysicsComponent::PositionX, x);
	m->SetValue(PhysicsComponent::PositionY, y);
	m->SetValue(PhysicsComponent::Rotation, rotation);
	
	bullet->SetParent(owner);	
	bullet->SetController(owningController);
	bullet->SetDamage(damage);
	bullet->SetRenderable(bulletRenderable);
	return bullet;

}

void BulletFactory::SetBulletStats(int colRadius, int dmg, IRenderable* renderable)
{
	collisionRadius = colRadius;
	damage = dmg;
	bulletRenderable = renderable;
}

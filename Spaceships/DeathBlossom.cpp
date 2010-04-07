#include "precompiled.h"
#include "DeathBlossom.h"

DeathBlossom::DeathBlossom(BulletFactory* factory, GameObject* owner) : Weapon(factory, owner)
{
    deathBlossomVisitor = (IVisitor*)new DeathBlossomVisitor(factory);
}

DeathBlossom::~DeathBlossom(void)
{
	owner->RemoveVisitor(deathBlossomVisitor);
	delete deathBlossomVisitor;
}

GameObject* DeathBlossom::Fire(const float x, const float y, const float rotation)
{
	this->owner->AcceptVisitor(deathBlossomVisitor);
	// TODO: refactor- this shouldn't need a return value
	return NULL;
}
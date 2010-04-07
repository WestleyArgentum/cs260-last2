#pragma once
#include "weapon.h"
#include "DeathBlossomVisitor.h"

class DeathBlossom : Weapon
{
public:
	DeathBlossom(BulletFactory* factory, GameObject* owner);
	~DeathBlossom(void);
protected:
	virtual GameObject* Fire(const float x, const float y, const float rotation);
private:
	IVisitor* deathBlossomVisitor;
};


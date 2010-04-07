#pragma once
#include "weapon.h"
#include "Bullet.h"
class PlasmaCannon :
	public Weapon
{
public:
	PlasmaCannon(BulletFactory* factory, GameObject* owner);
	~PlasmaCannon(void);
	virtual void CeaseFire();
protected:
	virtual GameObject* Fire(const float x, const float y, const float rotation);
};


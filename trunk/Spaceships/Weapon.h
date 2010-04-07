#pragma once
#include "BulletFactory.h"
#include "IWeapon.h"
class Weapon : IWeapon
{
public:
	Weapon(BulletFactory* factory, GameObject* owner);
	
	// x, y, and rotation are the position at which the object's weapon exists on the object
	// preconditions:  controller must not be null
	// invariants: x, y, z, rotation
	// postconditions:  a message to create a new bullet(s) will be sent to the controller
	void Fire(const float x, const float y, const float rotation, Controller* controller);

	virtual void Update();
	
	void SetFireDuration(const int duration);
	
	bool IsFiring();
	
//	virtual void CeaseFire() = 0;
	
	virtual ~Weapon(void);
protected:
	virtual GameObject* Fire(const float x, const float y, const float rotation) = 0;
	GameObject* owner;

private:
	BulletFactory* factory;	
	unsigned int fireDuration;
	unsigned int lastTimeFired;
	bool isFiring;
	
};


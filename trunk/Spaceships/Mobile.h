#pragma once
#include "Collider.h"

#define NO_OWNER -1;

enum PhysicsComponent
{
	AccellerationX = 0,
	AccellerationY,
	GeneralVelocity,	// velocity in whatever direction it's currently moving
	VelocityX,
	VelocityY,
	PositionX,
	PositionY,
	MaxAccellerationX,
	MaxAccellerationY,
	MaxVelocityX,
	MaxVelocityY,
	RotationStep,
	Rotation,
	GravityX,
	GravityY,
	PhysicsComponentsCount
};

// this shouldn't be necessary either but it makes it compile so whatever
// TODO: fix this
class GameObject;

class Mobile
{
public:
	Mobile();
	virtual ~Mobile(void);
	
	GameObject* GetOwner();		
	int* OwnedObjects();	
	void SetOwner(GameObject* owner);
	void SetValue(PhysicsComponent component, float value);
	float GetValue(PhysicsComponent component);
	int GetId();
	Collider* GetCollider();
	void SetCollider(Collider* value);
	// set to true to enforce that this mobile can't ever move
	// outside of the defined world coordinates.  Bad idea to
	// set this for objects that should die off-screen like asteroids
	void ShouldBindToWorld(bool value);
	bool IsBoundToWorld();
	GameObject* GetParent();
	
private:
	GameObject* owner;
	float* componentValues;
	Collider* collider;		
	bool bindToWorld;
};

struct MobileFinder: public std::binary_function<Mobile*, int, bool>
{
public:
	bool operator () (Mobile* mobile, const int id) const 
	{
		return mobile->GetId() == id;	
	}
};
#pragma once
#include "GameObject.h"

// the compiler is full of hate
class GameObject;

class Collider
{
public:
	Collider();
	virtual ~Collider(void);

	bool CollidedWith(Collider* obj);

	virtual int DoDamage(Collider* obj);
	
	
	virtual int SetDamage(int d);
	virtual void SetPosition(float x, float y);
	int GetHitpoints();
	void SetRadius(int r);
//	void SetParentID(int id);
	int GetParentID();
	int GetOwnerID();
	void SetOwner(GameObject* object);
	GameObject* GetOwner();
	int GetDamage();



protected:
	//how much damange I can take
	int hitPoints;

	//how much damage I do
	int damage;

	//position and collision radius
	float x;
	float y;
	int radius;	
	//int ownerID;
	//int parentID;

	GameObject* owner;
};

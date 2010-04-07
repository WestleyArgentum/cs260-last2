#pragma once
#include "Collider.h"
#include "IPrimitiveRenderable.h"

const int bullet_lifetime = 40;


class Bullet :
	public GameObject
{
public:
	Bullet();
	virtual ~Bullet(void);
	bool IsDead();
	virtual ObjectType GetType();
protected:
	void PreProcessMessages();
	void PostProcessMessages();
	void ProcessMessage(Message* msg);
	void OnUpdate();
	void PreGetRenderable();
};

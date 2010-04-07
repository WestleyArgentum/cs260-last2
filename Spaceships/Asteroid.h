#pragma once
#include "GameObject.h"
#include "TextRenderer.h"

class Asteroid :
	public GameObject
{
public:
	Asteroid();
	~Asteroid(void);
	virtual bool IsDead();
	virtual ObjectType GetType();
protected:
	void OnUpdate();
	void PreProcessMessages();
	void PostProcessMessages();
	void ProcessMessage(Message* msg);
	void PreGetRenderable();
private:
	int updatecounter;
	int hitpoints;
};

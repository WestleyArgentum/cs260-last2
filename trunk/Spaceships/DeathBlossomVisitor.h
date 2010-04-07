#pragma once
#include "gameobjectvisitor.h"
#include "map"
#include "BulletFactory.h"

enum DeathBlossomStateEnum
{
	CHARGING = 0,
	FIRING,
	COOLDOWN
};

typedef struct DeathBlossomState
{
	DeathBlossomStateEnum StateName;
	int Duration;
	DeathBlossomState* NextState;
	bool ShouldBeFiring;
	float RotationalVelocity;
} DeathBlossomState;

class DeathBlossomVisitor :
	public GameObjectVisitor
{
public:
	DeathBlossomVisitor(BulletFactory* factory);
	~DeathBlossomVisitor(void);
protected:
	virtual void OnAccept();
	virtual void OnLeave();
	virtual void OnVisit(GameObject* object);
	virtual void ProcessMessage(Message* msg);
private:
	int stateStartTime;
	float currentRotation;
	float rotationStep;
	void FireBullets();
	DeathBlossomState* currentState;
	map<DeathBlossomStateEnum,DeathBlossomState*> states;
	void InitializeStateMap();
	BulletFactory* bulletFactory;

};


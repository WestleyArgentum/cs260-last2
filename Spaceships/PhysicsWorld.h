#pragma once
#include "Mobile.h"
#include "GameObject.h"
#include "Message.h"

// TODO: this should probably be PhysicsModel
class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld(void);
	//void AddMobile(Mobile* mobile);
	//void RemoveMobile(int id);
	void Update();
	void SetWorldSize(int x, int y);
	void WrapAround(bool value);
	list<Mobile*>* GetAllObjects();
	void Init();
	void PassMessage(Message* msg);
	/*static int GetWorldSizeX();
	static int GetWorldSizeY();*/

private:
	void DoMovement();
	void HandleCollisions();
	list<Mobile*> objects;
	CRITICAL_SECTION objectsLock;
	int worldSizeX;
	int worldSizeY;
	bool wrapAround;	
};

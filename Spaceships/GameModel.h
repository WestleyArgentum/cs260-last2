/*
this is responsible for maintaining the state of all the game objects like sprites and stuff.  you will 
likely need to make changes here to account for state changes from remote hosts, although you may not need 
to modify it depending on your architecture (I probably wouldn't)
*/

#pragma once
#include "model.h"
#include "GameObject.h"
#include <vector>
#include <list>
#include "SpaceShip.h"

using namespace std;


enum objectType{
PRIMITIVE = 0,	//lines, points, and stuff
SPRITE,	//dumb objects that are computer-controlled
PLAYERSPRITE, //objects that are player-controlled
OBJECTTYPE_COUNT
};

class GameModel :
	public Model
{
public:
	GameModel(void);
	~GameModel(void);

	// main player for this game.  
	GameObject* GetPlayerObject() const;

	// destroyobject removes an object 
	void DestroyObject(const int objectID);

	// add a new object of some type
	void AddNewObject(GameObject* obj);

	// update non-player objects
	void UpdateObjects();

	// destroy all objects in the model
	void Eradicate();

	// Returns the GameObject that corresponds to the given ID or 
	// NULL if no such object exists.
	GameObject* FindObjectById(int id) const;

protected:
	list<GameObject*> _objects;
	GameObject* player;
};

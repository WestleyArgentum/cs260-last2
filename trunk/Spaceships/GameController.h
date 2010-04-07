#pragma once
//
#include "controller.h"
#include "GameModel.h"
#include "GameView.h"

#include "randomnumbergenerator.h"
#include "asteroidfactory.h"
#include "SpaceShipFactory.h"
#include "physics.h"
#include "PhysicsWorld.h"
//#include "objecttype.h"


using namespace std;
const int worldSizeX = WORLD_SIZE_X;
const int worldSizeY = WORLD_SIZE_Y;

class GameController :
	public Controller
{
public:
	GameController(Model* m, View* v, AsteroidFactory* factory, SpaceShipFactory* sFactory);
	virtual ~GameController(void);
	virtual GameState Update();
	virtual void PassMessage(Message* msg);
private:
	void ProcessMessages();
	GameView* _view;
	GameModel* _model;
	int bulletLimiter;
	bool SouldCreateAsteroid(float* x, float* y, float* rotation, float* velocity);
	AsteroidFactory* asteroidFactory;
	SpaceShipFactory* sFactory;
	bool ShouldReset;
	PhysicsWorld physicsWorld;

	bool firstpass;

};

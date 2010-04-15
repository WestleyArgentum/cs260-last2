///////////////////////////////////////////////////////////////////////////////////////
//
//	Game Logic
//	Simple game logic for demo. A good way to approach understand the game sample
//	is to walk through all the code in game logic and follow the execution through
//	the rest of the engine.
//	
//	Authors: Chris Peters, Benjamin Ellinger
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "GameStateManager.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "ComponentCreator.h"
#include "PlayerController.h"
#include "Asteroid.h"
#include "SinglePlayer.h"
#include "ScoreDisplay.h"

#include <ctime>

namespace Framework
{
	//Our global pointer to the game.
	GameStateManager* GSM = NULL;

  ///Initialize these once so they aren't mis-used later...
  const GameStateManager::GameStateID GameStateManager::GS_EXIT("EXIT");
  const GameStateManager::GameStateID GameStateManager::GS_RESTART("RESTART");

  ///Registers all the component types and creates the GameStateObjects
	void GameStateManager::Initialize()
	{
		//Components are explicitly registered in initialize
		RegisterComponent(Transform);
		RegisterComponent(PlayerController);
		RegisterComponent(Asteroid);
    RegisterComponent(ScoreDisplay);

    //GameStates are created and registered here.
    RegisterGameState(SinglePlayer);

    ///Set the initial state and initialize it...
    Curr = Next = "SinglePlayer";
    GameStates[Curr]->Initialize();
	}

  ///Initialize the GSM pointer and check for duplicate creations
  GameStateManager::GameStateManager() : playerShipId_(0)
	{	
		//Set up the global pointer
		ErrorIf(GSM!=NULL,"Logic already initialized");
		GSM = this;

	}

  ///Cleanup all the GameState memory.
	GameStateManager::~GameStateManager()
	{
    ///Cleanup the GameStates
    GameStateMap::iterator b = GameStates.begin(), e = GameStates.end();
    while (b != e)
      delete b++->second;
	}

  ///Pass the message to the current state.
	void GameStateManager::SendMessage(Message * m )
	{
    GameStates[Curr]->SendMessage( m );
	}

	void GameStateManager::Update(float dt)
	{
    if (Next == GS_EXIT)
      2;// TODO: Do something to quit...

    ///Check if we need to change the gamestate.
    if (Next != Curr)
    {
      /// TODO: Check if Next exists!!
      GameStates[Curr]->Cleanup();
      GameStates[Next]->Initialize();
      Curr = Next;
    }
    ///Or if we are to restart the state.
    else if (Next == GS_RESTART)
      GameStates[Curr]->Restart();

    ///Update the current state!
    GameStates[Curr]->Update(dt);
	}

  void GameStateManager::AddGameState( const std::string &name, IGameState *state )
  {
    if (GameStates.count(name))
      return;

    GameStates[name] = state;
  }

}



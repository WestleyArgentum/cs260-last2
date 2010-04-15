///////////////////////////////////////////////////////////////////////////////////////
///
///	\file GameLogic.h
///	Sample game logic for demo.
///	
///	Authors: Benjamin Ellinger, Chris Peters
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////

#pragma once //Makes sure this header is only included once

#include "Graphics.h"
#include "Physics.h"
#include "Engine.h"
#include "IController.h"
#include "IGameState.h"

namespace Framework
{
	///Sample Demo Game Logic
	class GameStateManager : public ISystem
	{
	public:
		GameStateManager();
		~GameStateManager();
		void Initialize();
		void Update(float timeslice);

		virtual std::string GetName(){return "GameStateManager";}
		virtual void SendMessage(Message *);
		//GOC * CreateObjectAt(Vec2& position,float rotation,const std::string& file);
		//void LoadLevelFile(const std::string& file);

		Vec2 WorldMousePosition;

	public:
    typedef std::string GameStateID;
    typedef std::map<GameStateID, IGameState *> GameStateMap;

    GameStateMap GameStates;

    GameStateID Curr;
    GameStateID Next;

    static const GameStateID GS_EXIT;
    static const GameStateID GS_RESTART;

    void AddGameState( const std::string &name, IGameState *state );
		void SpawnRandomAsteroids();

    GOCId playerShipId_;
	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameStateManager* GSM;
}

#define RegisterGameState( state ) GSM->AddGameState( #state , new state(this) )


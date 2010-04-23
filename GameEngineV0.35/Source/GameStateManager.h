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

#include <string>

#include "Graphics.h"
#include "Physics.h"
#include "Engine.h"
#include "IController.h"
#include "IGameState.h"
#include "Stats.h"

namespace Framework
{

  const unsigned NUM_PLAYERS = 8;

	///Class to manage the current game state and all its objects.
	class GameStateManager : public ISystem
	{
	public:
    typedef std::string GameStateID;
    typedef std::map<GameStateID, IGameState *> GameStateMap;

  private:
    ///A map of the available gamestates.
    GameStateMap GameStates;

    ///Keeps track of Current and Next GameStates.
    GameStateID Curr;
    GameStateID Next;

    ///Catalyst states.
    static const GameStateID GS_EXIT;
    static const GameStateID GS_RESTART;

    ///The GOCId of the player...
    GOCId playerShipId_;
    Statistics gameStats_;   ///< Statistics of all the players of the game!

    ///Adds a new GameState to the choices of possible states.
    void AddGameState( const std::string &name, IGameState *state );
    void SpawnRandomAsteroids();

  public:
    GameStateManager();
    ~GameStateManager();

    ///Initialize the first GameState.
    void Initialize();
    void Update(float timeslice);

    ///Interface methods.
    virtual std::string GetName(){return "GameStateManager";}
    virtual void SendMessage(Message *);

    ///Sets the next GameState to state, returns true for success.
    bool ChangeState( GameStateID state );

    ///Returns the current GameStateID.
    GameStateID GetState( void ) const { return Curr; }

		template <typename GameState>
		GameState* GetCurrState ()
		{
			return dynamic_cast<GameState*>(GameStates[Curr]);
		}

    ///Returns the player's GOCId.
    GOCId GetPlayerId( void ) { return playerShipId_; }
    Statistics& GetStats( void ) { return gameStats_; }

    void UpdateStats( const Statistics &stats ) { gameStats_ = stats; }

    ///Stores the current player Id in the current gamestate.
    void SetPlayerId( GOCId pid ) { playerShipId_ = pid; }

    // Statistic update functions.
    void SetPlayerColor( GOCId pid, Color shipColor );
    void AddScoreTo( GOCId pid, int amount );

    // Get all the information about the player in question.
    PlayerStats* GetPlayerInfo( GOCId pid );

    void AddController( Controller *controller );
    void RemoveController( Controller *controller );

		void RegisterComposition(GOCId id);

    /// TODO: Super hacked... need to change this...
    Vec2 WorldMousePosition;
	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameStateManager* GSM;


///Registers a new GameState with the GameStateManager.
#define RegisterGameState( state ) GSM->AddGameState( #state , new state(this) )
#define GetGameState( state )  GSM->GetCurrState<state>()

}

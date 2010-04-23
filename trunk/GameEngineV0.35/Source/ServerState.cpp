
#include "Precompiled.h"
#include "ServerState.h"
#include "WindowsSystem.h"
#include "Physics.h"
#include "Core.h"
#include "GameStateManager.h"
#include "Stats.h"
#include "MessageHub.h"
#include "Network.h"
#include "Console.h"
#include "GameMessages.h"
#include "Transform.h"
#include "PlayerController.h"

#include <iostream>

namespace Framework
{
  void ServerState::HandleConnection(INetMessage *msg)
  {
    ConnectionMessage *connect = static_cast<ConnectionMessage*>(msg);
    ///Create a new player with the name specified.
    GOC *obj = CreateObjectAt(Vec2(0,0),0,"PlayerShip");
    InitializeConnection(connect,obj);
  }

  void ServerState::HandleInput(INetMessage *msg)
  {
    InputButtonMessage *input = static_cast<InputButtonMessage*>(msg);
    ///Handle player input.

    GOC *obj = FACTORY->GetObjectWithId(input->id);
    obj->SendMessage(input);
  }

  ///Goes through every object in the current game and sends a create message for it.
  void ServerState::InitializeConnection(ConnectionMessage *connect, GOC *ship)
  {
    ///Sync the new connections game with the current gameplay.
    GameObjectFactory::GameObjectIdMapType::iterator begin = FACTORY->begin();
    GameObjectFactory::GameObjectIdMapType::const_iterator end = FACTORY->end();
    while (begin != end)
    {
      ///Send off a message telling the connection to create each object.
      NETWORK->SendNetMessage(connect->address, CreateMessage(begin->second->GetType(),begin->first,
        begin->second->has(Transform)->Position, begin->second->has(Transform)->Rotation));
      ++begin;
    }
    
    //Send special message informing the connection which ship is theirs.
    PlayerMessage player;
    player.id = ship->GetId();
    player.statsid = AddPlayerToRoster(player.id);

    NETWORK->SendNetMessage(connect->address,player);
  }

  unsigned ServerState::AddPlayerToRoster( GOCId pid )
  {
    GSM->GetStats()[statsindexbase].playerId_ = pid;
    return statsindexbase++;
  }

	ServerState::ServerState( GameStateManager *gsm ) : IGameState( gsm ), statsindexbase(0)
 	{}

	Framework::ServerState::~ServerState( void )
	{}

	void Framework::ServerState::Initialize( void )
	{
		LoadFromFile("Levels\\Server.txt");

    // spawn character
    GOC* myplayer = CreateObjectAt(Vec2(0,0), 0, "PlayerShip");
    GSM->SetPlayerId(myplayer->GetId());

		SpawnRandomAsteroids();

    ///Adds the player to the roster so statistics can be generated.
    AddPlayerToRoster(myplayer->GetId());

		NETWORK->HostServer();

    CreateConsole();
    std::cout << "Server IP: " << NetAPI->LocalIP() << std::endl;

    ///Initialize all the player stats.
    PlayerStats *stats;

    stats = &GSM->GetStats()[1];
    (*stats) = PlayerStats(0,0,Color(.4f,.2f,.1f,1.f));
    
    stats = &GSM->GetStats()[2];
    (*stats) = PlayerStats(0,0,Color(.2f,.4f,.2f,1.f));
    
    stats = &GSM->GetStats()[3];
    (*stats) = PlayerStats(0,0,Color(.1f,.7f,.0f,1.f));
    
    stats = &GSM->GetStats()[4];
    (*stats) = PlayerStats(0,0,Color(.0f,.5f,.5f,1.f));
    
    stats = &GSM->GetStats()[5];
    (*stats) = PlayerStats(0,0,Color(.2f,.2f,.7f,1.f));
    
    stats = &GSM->GetStats()[6];
    (*stats) = PlayerStats(0,0,Color(.3f,.1f,.4f,1.f));
    
    stats = &GSM->GetStats()[7];
    (*stats) = PlayerStats(0,0,Color(.0f,.0f,.0f,1.f));

	}

	void Framework::ServerState::OnCleanup( void )
	{
    RemoveConsole();
  }

	void Framework::ServerState::AddController( Controller *controller )
	{
    if (controller->GetControllerID() == CID_Player)
      OtherPlayers.push_back(controller);
    else
  		Controllers.push_back(controller);
	}

	void Framework::ServerState::RemoveController( Controller *controller )
	{
    if (controller->GetControllerID() == CID_Player)
      OtherPlayers.erase(controller);
    else
  		Controllers.erase(controller);
	}

	void Framework::ServerState::SendMessage( Message *m )
	{
		switch( m->MessageId )
		{
			// The user has pressed a (letter/number) key, we may respond by creating
			// a specific object based on the key pressed.
		case Mid::CharacterKey:
			{
				// Cast to the derived message type
				MessageCharacterKey * key = (MessageCharacterKey*)m;

				//When different keys are pressed create different objects
				std::string ObjectToCreate;

				if( key->character == '1' )					
					ObjectToCreate = "Ball";
				else if( key->character == '2' )
					ObjectToCreate = "Box";
				else if( key->character == '3' )
					ObjectToCreate = "Asteroid";

				//if( !ObjectToCreate.empty() )
				//	CreateObjectAt(WorldMousePosition,0,ObjectToCreate);

        //Enable Physics Debugging
				if( key->character == 'z' || key->character == 'Z' )
					PHYSICS->AdvanceStep = true;
        if( key->character == 'z' || key->character == 'Z' && IsShiftHeld() )
					PHYSICS->StepModeActive = !PHYSICS->StepModeActive;

				if( key->character == 'g' )
				{
					ToggleDebugDisplay debugMessage( true );
					CORE->BroadcastMessage( &debugMessage );
				}

				else if( key->character == 'h' )
				{
					ToggleDebugDisplay debugMessage( false );
					CORE->BroadcastMessage( &debugMessage );
				}
				break;
			}
		case Mid::FileDrop:
			{
				//FileDrop * drop = (FileDrop*)m;
				//LoadLevelFile(drop->FileName);
				break;
			}
		case Mid::MouseMove:
			{
				//The mouse has moved, update the mouse's world position
				MouseMove * mouseMove = (MouseMove*)m;
				GSM->WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouseMove->MousePosition);
				break;
			}
    case Mid::NetMessage:
      {
        INetMessage *msg = static_cast<INetMessage*>(m);
        switch (msg->Type())
        {
        case NMid::Connection:
          {
            HandleConnection(msg);
            break;
          }
        case NMid::InputBtn:
          {
            HandleInput(msg);
            break;
          }
        }
        break;
      }
		}
	}

	void Framework::ServerState::Update( float dt )
	{
		StatsMessage stats( GSM->GetStats() );
		MessageHub->Post( stats );

    ObjectLinkList<Controller>::iterator b = OtherPlayers.begin(), e = OtherPlayers.end();
    while (b != e)
    {
      static_cast<PlayerController*>(&*b)->ServerUpdate(dt);
      ++b;
    }

		b = Controllers.begin(), e = Controllers.end();
		while (b != e)
		{
			b->Update(dt);
			++b;
		}

    NETWORK->SendNetMessage(StatsMessage(GSM->GetStats()));

    ///Update yourself
    GOC *hack = FACTORY->GetObjectWithId(GSM->GetPlayerId());
    if (hack)
      hack->has(PlayerController)->Update(dt);

    GameObjectFactory::GameObjectIdMapType::iterator begin = FACTORY->begin();
    GameObjectFactory::GameObjectIdMapType::const_iterator end = FACTORY->end();
    while (begin != end)
    {
      NETWORK->SendNetMessage(UpdateMessage(begin->first,begin->second->has(Transform)->Position,begin->second->has(Transform)->Rotation));
      ++begin;
    }


	}

	void Framework::ServerState::Restart( void )
	{
	}

}


#include "Precompiled.h"
#include "ServerState.h"
#include "WindowsSystem.h"
#include "Physics.h"
#include "Core.h"
#include "GameStateManager.h"
#include "Stats.h"
#include "MessageHub.h"
#include "Network.h"

namespace Framework
{


  void ServerState::HandleConnection(INetMessage *msg)
  {

  }
  void ServerState::HandleCreate(INetMessage *msg)
  {

  }
  void ServerState::HandleDestroy(INetMessage *msg)
  {

  }
  void ServerState::HandleInput(INetMessage *msg)
  {

  }

	Framework::ServerState::ServerState( GameStateManager *gsm ) : IGameState( gsm )
 	{}

	Framework::ServerState::~ServerState( void )
	{}

	void Framework::ServerState::Initialize( void )
	{
		LoadFromFile("Levels\\Server.txt");
		SpawnRandomAsteroids();

		ErrorIf(!NETWORK);
		NETWORK->HostServer();
		
	}

	void Framework::ServerState::OnCleanup( void )
	{}

	void Framework::ServerState::AddController( Controller *controller )
	{
		Controllers.push_back(controller);
	}

	void Framework::ServerState::RemoveController( Controller *controller )
	{
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
        case NMid::Create:
          {
            HandleCreate(msg);
            break;
          }
        case NMid::Destroy:
          {
            HandleDestroy(msg);
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
		StatsMessage stats;
		static unsigned score = 0;
		stats.stats_.push_back( PlayerStats(0, ++score ) );

		MessageHub->Post( stats );

		ObjectLinkList<Controller>::iterator b = Controllers.begin(), e = Controllers.end();
		while (b != e)
		{
			b->Update(dt);
			++b;
		}
	}

	void Framework::ServerState::Restart( void )
	{

	}

}

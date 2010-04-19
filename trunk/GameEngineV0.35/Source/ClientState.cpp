#include "Precompiled.h"
#include "ClientState.h"
#include "Network.h"
#include "Factory.h"
#include "UtilityGameFunctions.h"
#include "GameMessages.h"
#include "Core.h"
#include "Physics.h"
#include "PlayerController.h"

Framework::ClientState::ClientState( GameStateManager *gsm ) : IGameState(gsm)
{}

Framework::ClientState::~ClientState( void )
{}

void Framework::ClientState::Initialize( void )
{
	// connect to a server
	ErrorIf(!NETWORK);
	NETWORK->FindServer();
  CORE->SleepSystem(PHYSICS);
}

void Framework::ClientState::OnCleanup( void )
{}

void Framework::ClientState::AddController( Controller *controller )
{
  switch ( controller->GetControllerID() )
  {
  case CID_DisplayScore:
    {
      Controllers.push_back(controller);
      break;
    }
  }
}

void Framework::ClientState::RemoveController( Controller *controller )
{
  switch ( controller->GetControllerID() )
  {
  case CID_DisplayScore:
    {
      Controllers.erase(controller);
      break;
    }
  }
}

void Framework::ClientState::HandleCreate( INetMessage *msg )
{
  CreateMessage* message = static_cast<CreateMessage*>(msg);
  CreateObjectAt(message->pos, message->rot, message->obj_type, message->id);
}

void Framework::ClientState::HandlePlayerId( INetMessage *msg )
{
  PlayerMessage *pid = static_cast<PlayerMessage*>(msg);
  playerid_ = pid->id;
}

void Framework::ClientState::HandleDestroy( INetMessage *msg )
{
  DestroyMessage *destroy = static_cast<DestroyMessage*>(msg);
  FACTORY->DestroyById(destroy->id);
}

void Framework::ClientState::SendMessage( Message *m )
{
  if (m->MessageId == Mid::NetMessage)
  {
    INetMessage *msg = static_cast<INetMessage*>(m);
    switch (msg->Type())
    {
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
    case NMid::PlayerId:
      {
        HandlePlayerId(msg);
        break;
      }
    case NMid::EndGame:
      {
        break;
      }
    case NMid::RestartGame:
      {
        break;
      }
    case NMid::Stats:
      {
        break;
      }
    }
  }
}

void Framework::ClientState::Update( float dt )
{
  GOC *player = FACTORY->GetObjectWithId(playerid_);
  if (player)
  {
    PlayerController *pc = player->has(PlayerController);
    if (pc)
      pc->ClientUpdate(dt);
  }

  for ( ObjectLinkList<Controller>::iterator it = Controllers.begin(); it != Controllers.end(); ++it )
  {
		it->Update( dt );
	}
}

void Framework::ClientState::Restart( void )
{}
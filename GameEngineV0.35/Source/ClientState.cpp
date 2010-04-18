#include "Precompiled.h"
#include "ClientState.h"
#include "Network.h"
#include "Factory.h"
#include "UtilityGameFunctions.h"
#include "GameMessages.h"

Framework::ClientState::ClientState( GameStateManager *gsm ) : IGameState(gsm)
{}

Framework::ClientState::~ClientState( void )
{}

void Framework::ClientState::Initialize( void )
{
	// connect to a server
	ErrorIf(!NETWORK);
	NETWORK->FindServer();
}

void Framework::ClientState::OnCleanup( void )
{}

void Framework::ClientState::AddController( Controller *controller )
{
	// hahaha, no.
}

void Framework::ClientState::RemoveController( Controller *controller )
{
	// ya... na...
}

void Framework::ClientState::HandleCreate( INetMessage *msg )
{
  CreateMessage* message = static_cast<CreateMessage*>(msg);
  CreateObjectAt(message->pos, 0, message->obj_type, message->id);
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

}

void Framework::ClientState::Restart( void )
{}
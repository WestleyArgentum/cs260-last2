#include "Precompiled.h"
#include "ClientState.h"
#include "Network.h"

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
    case NMid::Update:
      {
        HandleUpdate(msg);
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
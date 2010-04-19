#include "Precompiled.h"

#include "BulletController.h"
#include "Body.h"
#include "SinglePlayer.h"
#include "GameStateManager.h"
#include "Transform.h"
#include "Composition.h"
#include "Network.h"

#include <assert.h>

namespace Framework
{

	BulletController::~BulletController()
	{}

	void BulletController::Serialize( ISerializer& stream )
	{
		StreamRead(stream, speed);
	}

	void BulletController::OnInitialize( void )
	{}

	void BulletController::SendMessage( Message* message )
	{
		switch(message->MessageId)
		{
		case Mid::Collide:
			{
        DestroyMessage destroy;
        destroy.id = GetOwner()->GetId();
        NETWORK->SendNetMessage(destroy);

				GetOwner()->Destroy();
				break;
			}
    case Mid::NetMessage:
      {
        switch(static_cast<INetMessage*>(message)->Type())
        {
        case NMid::Update:
          HandleUpdateMessage(static_cast<UpdateMessage*>(message));
          break;
        }
      }
		}
	}

  void BulletController::HandleUpdateMessage(UpdateMessage* mess)
  {
    Transform* trans = GetOwner()->has(Transform);
    trans->Position = mess->pos;
    trans->Rotation = mess->rot;
  }

	void BulletController::LogicalUpdate( float dt )
	{
  }

	void BulletController::DestroyCheck()
	{
		//^? based on time?
	}

}

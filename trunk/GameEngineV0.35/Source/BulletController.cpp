#include "Precompiled.h"

#include "BulletController.h"
#include "Body.h"
#include "SinglePlayer.h"
#include "GameStateManager.h"

namespace Framework
{

	BulletController::~BulletController()
	{}

	void BulletController::Serialize( ISerializer& stream )
	{
		StreamRead(stream, speed);
	}

	void BulletController::OnInitialize( void )
	{
		// set up velocity
		SinglePlayer* state = GetGameState(SinglePlayer);
		if (!state)
			return;  // maybe report some sort of error

		GameObjectComposition* player = FACTORY->GetObjectWithId(state->player_ship_id);
		Vec2 my_vel;
		if (player)
		{
			Transform* transform = player->has(Transform);
			my_vel = Vec2(cos(transform->Rotation - 89.5f) * speed, sin(transform->Rotation - 89.5f) * speed);
		}

		Body* body = GetOwner()->has(Body);
		body->SetVelocity(my_vel);
	}

	void BulletController::SendMessage( Message* message )
	{
		switch(message->MessageId)
		{
		case Mid::Collide:
			{
				GetOwner()->Destroy();
				break;
			}
		}
	}

	void BulletController::LogicalUpdate( float dt )
	{}

	void BulletController::DestroyCheck()
	{
		//^? based on time?
	}

}


// Asteroid.cpp : This is really a bomb.

#include "Precompiled.h"
#include "Asteroid.h"
#include "Transform.h"
#include "Factory.h"
#include "Body.h"
#include "PRNG.h"
#include "Physics.h"
#include "Network.h"

#include "GameStateManager.h"
#include "BulletController.h"
#include "PlayerController.h"
#include "ScoreDisplay.h"
#include "GameMessages.h"

namespace Framework
{
	void Asteroid::OnInitialize()
	{
		SpawnTime = timeGetTime();

		// go!
		Body * bod = GetOwner()->has(Body);

		// give the asteroid a random velocity
		bod->SetVelocity(Vec2(static_cast<float>(Utils::Random(-vel_range_x, vel_range_x)), 
													static_cast<float>(Utils::Random(-vel_range_y, vel_range_y))));

    transform = GetOwner()->has(Transform);
    body = GetOwner()->has(Body);
	}

  void Asteroid::LogicalUpdate( float dt )
  {}

	void Asteroid::Serialize(ISerializer& stream)
	{
		StreamRead(stream,Fuse);
		StreamRead(stream,SubSpawnCount);
		StreamRead(stream, vel_range_x);
		StreamRead(stream, vel_range_y);
	}

	void Asteroid::SendMessage( Message* m )
	{
    switch ( m->MessageId )
    {
    case Mid::Collide:
      {
		    //^! for now we will check what we collided with and behave accordingly...
        MessageCollide *mc = static_cast<MessageCollide*>(m);

          // Reference the object that we collided with for ease of access.
        GOC* &collidedObject = mc->CollidedWith;

          // Ignore Asteroid -> Asteroid collision.
        if ( collidedObject->has(Asteroid) )
        {
          return;
        }
        else
        {
          BulletController *bullet = collidedObject->has(BulletController);

          // If we collided with a player or a bullet, explode this asteroid.
          if ( bullet || collidedObject->has(PlayerController) )
          {
            DestroyMessage destroy;
            destroy.id = GetOwner()->GetId();
            NETWORK->SendNetMessage(destroy);

			      GetOwner()->Destroy();


              // If we should spawn more asteroids after this collision.
			      if ( SubSpawnCount > 0 )
			      {
				      Transform * transform = GetOwner()->has(Transform);
				      for ( int i = -1; i <= 1; ++i)
				      {
					      Vec2 dir( sin( float(i) * D3DX_PI * 0.3f ), cos( float(i) * D3DX_PI * 0.3f) );

                GOC *shrapnel = CreateObjectAt( transform->Position, 0, "Shrapnel" );
					      Body *shrapnelBody = shrapnel->has(Body);

                if ( shrapnelBody )
                {
					        shrapnelBody->SetVelocity( dir * 120 );
                }

                CreateMessage create;
                create.id = shrapnel->GetId();
                create.obj_type = "Shrapnel";
                create.pos = shrapnel->has(Transform)->Position;
                create.rot = shrapnel->has(Transform)->Rotation;
                NETWORK->SendNetMessage(create);
				      }
			      }
          }

          if ( bullet )
          {
              // Get the owner of the bullet and add score to that player.
            GSM->AddScoreTo( bullet->firedFrom, 100 );

              // Create a text object displaying the score that the player recieved.
            GOC *score = CreateObjectAt( transform->Position, 0, "ScoreText_100" );

            if ( score )
            {
              DisplayScore *displayScore = score->has(DisplayScore);
              PlayerStats *pStats = GSM->GetPlayerInfo(bullet->firedFrom);

              if ( displayScore && pStats )
              {
                displayScore->SetColor( pStats->color_ );
              }
            }
          }
		    }
        break;
      }   // case Mid::Collide

    case Mid::NetMessage:
      {
        // set my position to the position of the update message
        UpdateMessage* mess = dynamic_cast<UpdateMessage*>(m);
        if(mess)
        {
          Transform* trans = GetOwner()->has(Transform);
          trans->Position = mess->pos;
          trans->Rotation = mess->rot;
        }
        break;
      }

    }

    return;
	}

}


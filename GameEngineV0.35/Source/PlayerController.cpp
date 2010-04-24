
// PlayerController.cpp : The manifestation (in code) of the brains of the character.

#include "Precompiled.h"

#include "PlayerController.h"
#include "WindowsSystem.h"
#include "GameStateManager.h"
#include "MessageHub.h"
#include "SinglePlayer.h"
#include "UtilityGameFunctions.h"
#include "BulletController.h"
#include "GameMessages.h"
#include "Network.h"
#include "ServerState.h"
#include "Sprite.h"

namespace Framework
{

	 // shouldn't be here
	#define PI 3.14159265358979323846f
	#define DEG_TO_RAD (PI / 180.0f)

  PlayerController::PlayerController() : Controller(CID_Player)
	{
		// could set default value for character speed, etc... but who would forget to set those hahaha
	}

	PlayerController::~PlayerController()
	{
		// if you add something to the character destroy it here
	}

	void PlayerController::OnInitialize()
	{
		transform = GetOwner()->has(Transform);
		body = GetOwner()->has(Body);
		MessageHub->Register(GetOwner()->GetId(), Mid::MouseButton);

		//^! this is a hack
		SinglePlayer* state = GetGameState(SinglePlayer);
		if (state)
			GSM->SetPlayerId(GetOwner()->GetId());
		//else
			//issue some sort of error
	}

	void PlayerController::LogicalUpdate( float dt )
	{
		// handle movement
		if( IsUpHeld() || IsWHeld() )
    {
			body->AddForce(Vec2(cos(transform->Rotation - 90.0f) * speed, sin(transform->Rotation - 90.0f) * speed));  //^! <-- odd

      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 'w';
      NETWORK->SendNetMessage(btn);
    }
		if( IsDownHeld() || IsSHeld() )
    {
			body->AddForce(-Vec2(cos(transform->Rotation - 90.0f) * speed, sin(transform->Rotation - 90.0f) * speed));

      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 's';
      NETWORK->SendNetMessage(btn);
    }
		if( IsLeftHeld() || IsAHeld() )
    {
			transform->Rotation += rot_angle * DEG_TO_RAD;

      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 'a';
      NETWORK->SendNetMessage(btn);
    }
		if( IsRightHeld() || IsDHeld() )
    {
			transform->Rotation -= rot_angle * DEG_TO_RAD;

      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 'd';
      NETWORK->SendNetMessage(btn);
    }


		// check for fire
		if( IsSpaceHeld() )
		{
			//SinglePlayer* state = GetGameState(SinglePlayer);
			if(/*state && */time_last_fire <= 0)
			{
				time_last_fire = recharge_time;  // reset recharge time

				// set up the offset for the bullet (so it is in front of the ship)
				Vec2 laser_offset(cos(transform->Rotation - 89.5f) * 30, sin(transform->Rotation - 89.5f) * 30);

				GOC* bullet = CreateObjectAt(transform->Position + laser_offset, transform->Rotation, "Bullet");

				// set up bullets velocity (a little crude)
				Body* bulletbody = bullet->has(Body);
				BulletController* bulletbrain = bullet->has(BulletController);
        Sprite* bulletsprite = bullet->has(Sprite);

          // Store the player that fired this bullet. (So we know who to give points to!)
        bulletbrain->firedFrom = GetOwner()->GetId();
        bulletsprite->Color = GetOwner()->has(Sprite)->Color;

				Vec2 vel(cos(transform->Rotation - 89.5f) * bulletbrain->speed,
								 sin(transform->Rotation - 89.5f) * bulletbrain->speed);

				vel += body->Velocity;

				bulletbody->Velocity = vel;
        
        ///Send a message to the network to create a bullet.
        NETWORK->SendNetMessage(CreateMessage("Bullet",bullet->GetId(),bullet->has(Transform)->Position, 0));
			}
		}

		// dec the time until we can fire again
		time_last_fire -= dt;

		if( GOC * grabbedObject = FACTORY->GetObjectWithId(GrabbedObjectId))
		{
			Body * body = grabbedObject->has(Body);
			if( IsShiftHeld() )
			{
				//Hard set or Teleport the object
				body->SetPosition(GSM->WorldMousePosition);
				body->SetVelocity(Vec2(0,0));
			}
			else
			{
				//Shove the object around
				body->AddForce( (GSM->WorldMousePosition - body->Position) * 50 );
			}
		}
	}

  void PlayerController::ClientUpdate ( float dt )
  {
    PlayerStats *stats = GSM->GetPlayerInfo(GetOwner()->GetId());
    GetOwner()->has(Sprite)->Color = stats->color_;
    // handle movement
		if( IsUpHeld() || IsWHeld() )
    {
      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 'w';
      NETWORK->SendNetMessage(btn);
    }
		if( IsDownHeld() || IsSHeld() )
    {
      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 's';
      NETWORK->SendNetMessage(btn);
    }
		if( IsLeftHeld() || IsAHeld() )
    {
      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 'a';
      NETWORK->SendNetMessage(btn);
    }
		if( IsRightHeld() || IsDHeld() )
    {
      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.character = 'd';
      NETWORK->SendNetMessage(btn);
    }
    if( IsSpaceHeld() )
    {
      InputButtonMessage btn;
      btn.id = GetOwner()->GetId();
      btn.key = VK_SPACE;
      NETWORK->SendNetMessage(btn);
    }
  }
  
  void PlayerController::ServerUpdate ( float dt )
  {
    time_last_fire -= dt;
  }

	void PlayerController::DestroyCheck()
	{
		if(health <= 0)
		{
			FACTORY->Destroy(GetOwner());
			//^! here we should also end the match (for this player) and maybe take them to a lose screen
		}

		// Here we could kill the character if he flew out of bounds or something also
	}

  void PlayerController::UpdateInput(INetMessage *msg)
  {
    InputButtonMessage *btn = static_cast<InputButtonMessage*>(msg);
    
		// handle movement
    if( btn->character == 'w' || btn->key == VK_UP )
			body->AddForce(Vec2(cos(transform->Rotation - 90.0f) * speed, sin(transform->Rotation - 90.0f) * speed));  //^! <-- odd
		if( btn->character == 's' || btn->key == VK_DOWN )
			body->AddForce(-Vec2(cos(transform->Rotation - 90.0f) * speed, sin(transform->Rotation - 90.0f) * speed));
		if( btn->character == 'a' || btn->key == VK_LEFT )
			transform->Rotation += rot_angle * DEG_TO_RAD;
		if( btn->character == 'd' || btn->key == VK_RIGHT )
			transform->Rotation -= rot_angle * DEG_TO_RAD;
    if ( btn->key == VK_SPACE )
    {
			if(/*state && */time_last_fire <= 0)
			{
				time_last_fire = recharge_time;  // reset recharge time

				// set up the offset for the bullet (so it is in front of the ship)
				Vec2 laser_offset(cos(transform->Rotation - 89.5f) * 30, sin(transform->Rotation - 89.5f) * 30);

				GOC* bullet = CreateObjectAt(transform->Position + laser_offset, transform->Rotation, "Bullet");

				// set up bullets velocity (a little crude)
				Body* bulletbody = bullet->has(Body);
				BulletController* bulletbrain = bullet->has(BulletController);
        Sprite* bulletsprite = bullet->has(Sprite);

          // Store the player that fired this bullet. (So we know who to give points to!)
        bulletbrain->firedFrom = GetOwner()->GetId();
        bulletsprite->Color = GetOwner()->has(Sprite)->Color;

				Vec2 vel(cos(transform->Rotation - 89.5f) * bulletbrain->speed,
								 sin(transform->Rotation - 89.5f) * bulletbrain->speed);

				vel += body->Velocity;

				bulletbody->Velocity = vel;

        ///Send a message to the network to create a bullet.
        NETWORK->SendNetMessage(CreateMessage("Bullet",bullet->GetId(),bullet->has(Transform)->Position,0));
			}
    }
  }

	void PlayerController::Serialize(ISerializer& stream)
	{
		StreamRead(stream, health);
		StreamRead(stream, speed);
		StreamRead(stream, rot_angle);
		StreamRead(stream, recharge_time);
	}

	void PlayerController::SendMessage( Message * message )
	{
		switch( message->MessageId )
		{
    case Mid::Collide:
      {
        transform->Position = body->Position;
        break;
      }
		case Mid::MouseButton:
			{
				MouseButton * mouse = static_cast<MouseButton*>(message);
				//Update the world mouse position
				GSM->WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouse->MousePosition);

				if(mouse->ButtonIsPressed)
				{
					if( mouse->MouseButtonIndex == MouseButton::LeftMouse )
					{
						//On left click attempt to grad a object at the mouse cursor
						GOC * goc = PHYSICS->TestPoint( GSM->WorldMousePosition );
						if( goc )
						{
							GrabbedObjectId = goc->GetId();

							// Give the asteroid a new color, save the old one
							Sprite* sprite = goc->has(Sprite);
							if (sprite)
							{
								GrabbedObjectColor = sprite->Color;
								sprite->Color = GetOwner()->has(Sprite)->Color;
							}
						}
					}
				}
				else
				{
					//If the mouse has been release let go of the grabbed object
					GOC* asteroid = FACTORY->GetObjectWithId(GrabbedObjectId);
					if (asteroid)
					{
						Sprite* sprite = asteroid->has(Sprite);
						if (sprite)
							sprite->Color = GrabbedObjectColor;
					}

					GrabbedObjectId = 0;
				}
				break;
			}
    case Mid::NetMessage:
      {
        INetMessage *msg = static_cast<INetMessage*>(message);
        switch (msg->Type())
        {
        case NMid::Update:
          {
            UpdateMessage *umsg = static_cast<UpdateMessage *>(msg);
            transform->Position = umsg->pos;
            transform->Rotation = umsg->rot;
            break;
          }
        case NMid::InputBtn:
          {
            UpdateInput(msg);
            break;
          }
        }
        break;
      }
    }
	}

}

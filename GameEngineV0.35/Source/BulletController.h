#pragma once

#include "IController.h"
#include "GameMessages.h"

namespace Framework
{

	class BulletController : public Controller
	{
	public:
		virtual ~BulletController ();

		virtual void Serialize (ISerializer& stream);
		virtual void SendMessage (Message* message);

		float speed;      ///< speed of bullet (player must now access)
    GOCId firedFrom;  ///< GameObject id that fired this bullet.

	private:
		virtual void OnInitialize ( void );
		virtual void LogicalUpdate ( float dt );
		virtual void DestroyCheck ();

    void HandleUpdateMessage(UpdateMessage* mess);
	};

}

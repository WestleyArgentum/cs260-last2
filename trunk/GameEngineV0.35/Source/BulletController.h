#pragma once

#include "IController.h"

namespace Framework
{

	class BulletController : public Controller
	{
	public:
		virtual ~BulletController ();

		virtual void Serialize (ISerializer& stream);
		virtual void SendMessage (Message* message);

		float speed;  // speed of bullet (player must now access)

	private:
		virtual void OnInitialize ( void );
		virtual void LogicalUpdate ( float dt );
		virtual void DestroyCheck ();
	};

}

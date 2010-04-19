
// PlayerController.h : The manifestation (in code) of the brains of the character.

#ifndef PLAYERCONTROLLER
#define PLAYERCONTROLLER

#include "IController.h"

namespace Framework
{

	class PlayerController : public Controller
	{
	public:
		PlayerController ();
		virtual ~PlayerController ();
		virtual void Serialize(ISerializer& stream);

		virtual void SendMessage(Message * message);

    virtual void ClientUpdate ( float dt );

		int health;  // health of the character (decremented by collisions, etc)

	private:
		virtual void OnInitialize ();
		virtual void LogicalUpdate ( float dt );
		virtual void DestroyCheck ();
    
    void UpdateInput(INetMessage *msg);

		Transform * transform;
		Body* body;

		float speed;
		float rot_angle; // in degrees
		float recharge_time;

		unsigned GrabbedObjectId;
		Vec4 GrabbedObjectColor;
		float time_last_fire;

	};

}

#endif
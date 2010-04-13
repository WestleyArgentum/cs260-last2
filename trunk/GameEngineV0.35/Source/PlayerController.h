
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
		virtual void Initialize ();

		virtual void SendMessage(Message * message);

		int health;  // health of the character (decremented by collisions, etc)

	private:
		virtual void LogicalUpdate ( float dt );
		virtual void DestroyCheck ();

		Transform * transform;
		Body* body;

		float speed;
		float rot_angle; // in degs
		unsigned GrabbedObjectId;
		Vec4 GrabbedObjectColor;

	};

}

#endif
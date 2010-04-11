
// PlayerController.h : The manifestation (in code) of the brains of the character.

#include "IController.h"

namespace Framework
{

	class PlayerController : public Controller
	{
	public:
		PlayerController ();
		virtual ~PlayerController ();

		int health;  // health of the character (decremented by collisions etc)

	private:
		virtual void LogicalUpdate ( float dt );
		virtual void DestroyCheck ();

	};

}

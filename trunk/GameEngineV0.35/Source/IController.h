
// IController.h : "Brains" or "Logic component" for entities. Updated by logic every loop.

#ifndef ICONTROLLER
#define ICONTROLLER

#include "Component.h"
#include "Transform.h"
#include "Body.h"

namespace Framework
{

	///Sample Demo Component Movement Controller. Used
	///to move objects around the world not under
	///the influence of physics.
	class Controller : public GameComponent
	{
	public:
		// intrusively linked list
		Controller * Next;
		Controller * Prev;

		Controller();
		virtual ~Controller();
		virtual void Initialize();
		virtual void Serialize(ISerializer& stream);

		void Update(float dt);

	protected:
		// implement these when you derive from controller
		virtual void LogicalUpdate ( float dt ) = 0;
		virtual void DestroyCheck ();
	};

}

#endif


// IController.h : "Brains" or "Logic component" for entities. Updated by logic every loop.

#ifndef ICONTROLLER
#define ICONTROLLER

#include "Component.h"
#include "Transform.h"
#include "Body.h"

namespace Framework
{
  enum ControllerID
  {
     CID_Asteroid
    ,CID_Bullet
    ,CID_Player
    ,CID_DisplayScore
    ,CID_ScoreHUD
    ,CID_Camera
  };    // ControllerID

	///Sample Demo Component Movement Controller. Used
	///to move objects around the world not under
	///the influence of physics.
	class Controller : public GameComponent
	{
	public:
		// intrusively linked list
		Controller * Next;
		Controller * Prev;

    Controller ( ControllerID cid ) : cid_(cid) {;}
		virtual ~Controller ();
		virtual void Serialize (ISerializer& stream);

		void Update (float dt);
    virtual void ClientUpdate (float dt) {};

    ControllerID GetControllerID( void ) { return cid_; }

	protected:
		// implement these when you derive from controller
    virtual void OnInitialize ( void ) = 0;
		virtual void LogicalUpdate ( float dt ) = 0;
		virtual void DestroyCheck ();

  private:
		virtual void Initialize();

    ControllerID cid_;
	};

}

#endif

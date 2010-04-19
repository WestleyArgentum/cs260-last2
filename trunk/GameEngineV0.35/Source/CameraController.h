#pragma once

#include "IController.h"
#include "GameMessages.h"

namespace Framework
{

	class CameraController : public Controller
	{
	public:
    CameraController( void ) : Controller(CID_Camera) {;}
    virtual ~CameraController () {;}

		virtual void Serialize (ISerializer& stream);
		virtual void SendMessage (Message* message);

	private:
    virtual void OnInitialize ( void );
    virtual void LogicalUpdate ( float dt );
    virtual void DestroyCheck () { /* we dont want to be destroyed */ }

    void HandleUpdateMessage(UpdateMessage* mess);
	};

}

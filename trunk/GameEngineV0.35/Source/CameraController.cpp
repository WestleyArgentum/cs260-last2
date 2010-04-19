#include "Precompiled.h"

#include "CameraController.h"
#include "GameStateManager.h"
#include "Factory.h"

namespace Framework
{

  void CameraController::Serialize (ISerializer& stream)
  {}

  void CameraController::SendMessage (Message* message)
  {}

  void CameraController::OnInitialize ( void )
  {}

  void CameraController::LogicalUpdate ( float dt )
  {
    // set my positon to the position of the player
    GOC* player = FACTORY->GetObjectWithId(GSM->GetPlayerId());
    if(player)
      GetOwner()->has(Transform)->Position = player->has(Transform)->Position;
  }

}
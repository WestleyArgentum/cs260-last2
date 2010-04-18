#include "Precompiled.h"
#include "Shaders.h"

#include "GameStateManager.h"
#include "Factory.h"
#include "Composition.h"
#include "Transform.h"
#include "WindowsSystem.h"

namespace Framework
{
  namespace
  {
    /// Passes the mouse position to the shader (in screen coordinates) as to integers, mouse_x
    ///  and mouse_y
/**************************************************************************************************/
/**************************************************************************************************/
    void AddMousePosition( ID3DXEffect *shader, Sprite *sprite )
    {
      shader->SetInt( "mouse_x", WINDOWSSYSTEM->MousePosition.x );
      shader->SetInt( "mouse_y", WINDOWSSYSTEM->MousePosition.y );
    }

    /// Gets the main player's position and passes it to the shader as two floats, ship_x and
    ///  ship_y.
/**************************************************************************************************/
/**************************************************************************************************/
    void AddPlayerPosition( ID3DXEffect *shader, Sprite *sprite )
    {
      GOC *ship = Framework::FACTORY->GetObjectWithId( GSM->GetPlayerId() );

      if ( ship )
      {
        Transform *sTrans = ship->has(Transform);

        if ( sTrans )
        {
          Vec2 pos = sTrans->Position - sprite->transform->Position;

          pos.x /= sprite->Size.x;
          pos.y /= -sprite->Size.y;

          pos.x += 0.5f;
          pos.y += 0.5f;

          shader->SetFloat( "ship_x", pos.x );
          shader->SetFloat( "ship_y", pos.y );
        }
      }
    }
  }

#if 0
/**************************************************************************************************/
/**************************************************************************************************/
  void BasicShader::InitPhase( ID3DXEffect *shader, Sprite *sprite )
  {
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void BulletShader::InitPhase( ID3DXEffect *shader, Sprite *sprite )
  {
  }
#endif

/**************************************************************************************************/
/**************************************************************************************************/
  void WaterShader::InitPhase( ID3DXEffect *shader, Sprite *sprite )
  {
    AddPlayerPosition( shader, sprite );
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void MainMenuShader::InitPhase( ID3DXEffect *shader, Sprite *sprite )
  {
    AddMousePosition( shader, sprite );
  }

}   // namespace Framework


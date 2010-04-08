#include "Precompiled.h"
#include "Text.h"
#include "Transform.h"
#include "Graphics.h"


// Rob: Not implemented yet...

namespace Framework
{
  Text::Text(void)
  {
  }

  Text::~Text(void)
  {
    //GRAPHICS->TextList.erase( this );
  }

  void Text::Initialize( void )
  {
    //GRAPHICS->TextList.push_back( this );
  }

  void Text::Serialize( ISerializer &stream )
  {
  }

#if 0
  void Text::Draw( Idirect3DDevice9 *pDevice, ID3DXEffect *shader, float dt )
  {
  }
#endif
}   // namespace Framework



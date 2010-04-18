#pragma once

namespace Framework
{
  enum PixelShaders
  {
	   Basic = 0
	  ,DebugShader
    ,Water
    ,Bullet
    ,Menu

	  ,NumberOfShaders
  };    // enum PixelShaders

  class Sprite;

  struct ISpriteShader
  {
    virtual ~ISpriteShader( void ) {;}

      /// Pass any particular values that you need to pass to a shader, i.e. mouse
    virtual void InitPhase( ID3DXEffect *shader, Sprite *sprite ) = 0;
  };    // IShader

}   // namespace Framework


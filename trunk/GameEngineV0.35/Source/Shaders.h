#pragma once

#include "IShaders.h"

namespace Framework
{
  // Other examples, not needed if your not doing anything extra.
#if 0
  struct BasicShader : public ISpriteShader
  {
    virtual void InitPhase( ID3DXEffect *shader, Sprite *sprite );
  };    // BasicShader

  struct BulletShader : public ISpriteShader
  {
    virtual void InitPhase( ID3DXEffect *shader, Sprite *sprite );
  };    // BasicShader
#endif

  struct WaterShader : public ISpriteShader
  {
    virtual void InitPhase( ID3DXEffect *shader, Sprite *sprite );
  };    // WaterShader

}   // namespace Framework


#pragma once

#include "Composition.h"
#include "VMath.h"

namespace Framework
{
	class Transform;

  class Text : public GameComponent
  {
    public:
      Text *Next;
      Text *Prev;

      Text( void );
      ~Text( void );

		  virtual void Serialize( ISerializer &stream );	
		  virtual void Initialize( void );

      void Draw( void );

    public:
      ID3DXFont *font_;
      unsigned height_;
      unsigned width_;
      std::string fontname_;
      D3DCOLOR color_;

      Transform *transform;
  };    // Text

}   // namespace Framework


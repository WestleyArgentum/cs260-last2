#pragma once

#include "Composition.h"

namespace Framework
{
  class Text : public GameComponent
  {
    public:
      Text *Next;
      Text *Prev;

      Text( void );
      ~Text( void );

		  virtual void Serialize( ISerializer &stream );	
		  virtual void Initialize( void );
  };    // Text

}   // namespace Framework


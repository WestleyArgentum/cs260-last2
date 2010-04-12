#pragma once

#include "IProtocol.h"
#include "GameMessages.h"

namespace Framework
{
  class GameProtocol : public IProtocol
  {
  public:
    GameProtocol( const INetMessage *msg ) : IProtocol(msg) {;}
    virtual ~GameProtocol( void ) {;}

  protected:
    virtual int FormatHeader( char *buffer, unsigned size ) const;

    virtual bool ValidateMessage( char *buffer, unsigned size );

    virtual unsigned StripHeader( char *buffer, unsigned size );

    virtual INetMessage * BuildMessage( char *buffer, unsigned size );
  };
}

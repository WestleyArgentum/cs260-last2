#pragma once

#include "IProtocol.h"
#include "ChatMessages.h"

namespace Framework
{
  class ChatProtocol : public IProtocol
  {
  public:
    ChatProtocol( const INetMessage *msg) : IProtocol(msg) {;}
    virtual ~ChatProtocol( void ) {;}

  protected:
    virtual int FormatHeader( char *buffer, unsigned size ) const;

    virtual bool ValidateMessage( char *buffer, unsigned size );

    virtual unsigned StripHeader( char *buffer, unsigned size );

    virtual INetMessage * BuildMessage( char *buffer, unsigned size );
  };
}

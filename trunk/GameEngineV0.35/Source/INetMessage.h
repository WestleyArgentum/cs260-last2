#pragma once // Make sure this header is only included once.

#include "DataStream.h"
#include "Message.h"

namespace Framework
{
  ///An interface for the NetMessages to be sent through the TCP and UDP sockets.
  class INetMessage
  {
  public:
    ///Makes sure all destructors are virtual.
    virtual ~INetMessage( void ) {;}

    ///Returns the type of message. Used when extracting messages from the list.
    virtual Mid::MessageIdType Type( void ) const = 0;

    ///Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const = 0;

    ///Returns the size of the entire message, including the header.
    virtual unsigned Size( void ) const = 0;

    ///Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const = 0;

    ///Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream ) = 0;
  };
};

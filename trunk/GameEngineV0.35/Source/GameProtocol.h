#pragma once

#include "IProtocol.h"
#include "INetMessage.h"

namespace Framework
{
  class GameProtocol : public IProtocol
  {
    unsigned NumMessages[NMid::NumIds]; ///< The number of messages of each type stored within.

  public:
    GameProtocol( void ) : IProtocol("GameProtocol") {;}
    virtual ~GameProtocol( void ) {;}
 
  protected:
    ///Writes the header into the buffer. Header explains the type of message.
    ///Returns the number of bytes written to the buffer so as to get an offset.
    virtual void FormatHeader( DataStream &stream ) const;

    ///Because Pepper > Salt : Salted hash added here if needed.
    //virtual int PepperMessage( DataStream &stream ) const { return 0; }


    ///Decides whether the packet is a valid message, takes care of decryption and unsalting.
    virtual bool ValidateMessage( DataStream &stream );

    ///Strips the header off the message and stores any data relevant to message creation.
    ///Returns the number of bytes stripped off, so that only the message data is left.
    virtual void StripHeader( DataStream &stream );

    ///Builds a message with the remaining data after the message has been decrypted/validated.
    virtual int BuildMessages( DataStream &stream, MessageList *messages );
  };
}

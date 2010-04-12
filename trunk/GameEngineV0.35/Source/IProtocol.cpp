#include "Precompiled.h"
#include "IProtocol.h"
#include "INetMessage.h"

namespace Framework
{
  ///Formats the buffer into a packet with the correct protocol.
  int IProtocol::FormatPacket( char *buffer, unsigned size )
  {
    ///Start at 0 for now, maybe add a special header later.
    unsigned offset = 0;

    ///Each format call returns the number of bytes it wrote.
    offset += FormatHeader(buffer+offset,size-offset);

    ///Serializes the message data and stores it in the buffer.
    offset += msg_->SerializeData(buffer+offset,size-offset);
    
    ///Applies a hashing method or encryption method.
    offset += PepperMessage(buffer,offset);

    ///Return the number of bytes written.
    return offset;
  }

  ///Attempts to extract a message from the data in the buffer.
  INetMessage * IProtocol::ExtractMessage(char *buffer, unsigned int size)
  {
    ///If the message isn't a valid message, discard it.
    if (!ValidateMessage(buffer,size))
      return 0;

    ///Strips the header and stores any data needed for message interpretation.
    unsigned offset = StripHeader(buffer,size);

    ///Builds a message from the data and info from the header.
    return BuildMessage(buffer+offset,size-offset);
  }

} // Framework namespace

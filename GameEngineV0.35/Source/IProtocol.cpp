#include "Precompiled.h"
#include "IProtocol.h"
#include "INetMessage.h"

namespace Framework
{


  ///Add a message to send. Doesn't delete messages when done with them.
  ///Returns the number of messages stored so far.
  int IProtocol::AddMessage( const INetMessage *msg )
  {
    messages.push_back(msg);
    return messages.size();
  }

  ///Clears messages from internal message list.
  void IProtocol::ClearMessages( void )
  {
    messages.clear();
  }

  ///Formats the buffer into a packet with the correct protocol.
  int IProtocol::FormatPacket( char *buffer, unsigned size ) const
  {
    ///Start at 0 for now, maybe add a special header later.
    unsigned offset = 0;

    ///Write the type of protocol in the beginning of the buffer.
    memcpy(buffer, &pType, sizeof(pType));


    ///Each format call returns the number of bytes it wrote.
    offset += FormatHeader(buffer+offset,size-offset);

    ///Serializes the messages into the buffer.
    NetMessageList::const_iterator begin = messages.begin(), end = messages.end();
    while (begin != end)
      offset += (*begin++)->SerializeData(buffer+offset,size-offset);
    
    ///Applies a hashing method or encryption method.
    offset += PepperMessage(buffer,offset);

    ///Return the number of bytes written.
    return offset;
  }

  ///Attempts to extract a message from the data in the buffer.
  NetMessageList * IProtocol::ExtractMessages(char *buffer, unsigned int size)
  {
    ///If the message isn't a valid message, discard it.
    if (!ValidateMessage(buffer,size))
      return 0;

    ///The container that will hold all of the messages retrieved from the packet.
    NetMessageList *retrieved = new NetMessageList();

    ///Strips the header and stores any data needed for message interpretation.
    unsigned offset = StripHeader(buffer,size);

    ///Builds a message from the data and info from the header. Keeps adding
    ///the size of each message to the offset until we've eaten all messages.
    while ((size - offset) > 0)
    {
      retrieved->push_back(BuildMessage(buffer+offset,size-offset));
      offset += retrieved->back()->Size();
    }

    return retrieved;
  }

} // Framework namespace

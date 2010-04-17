#include "Precompiled.h"
#include "IProtocol.h"
#include "DataStream.h"
#include "INetMessage.h"

namespace Framework
{


  ///Add a message to send. Doesn't delete messages when done with them.
  ///Returns the number of messages stored so far.
  int IProtocol::SetMessageList( MessageList *msgs )
  {
    messages = msgs;
    return messages->size();
  }

  ///Clears messages from internal message list.
  void IProtocol::ClearMessages( void )
  {
    ///Set pointer to null.
    messages = 0;
  }

  ///Formats the buffer into a packet with the correct protocol.
  int IProtocol::FormatPacket( DataStream &stream ) const
  {

    ///Write the type of protocol in the beginning of the buffer.
    //memcpy(buffer, &pType, sizeof(pType));


    ///Each format call returns the number of bytes it wrote.
    FormatHeader(stream);

    ///Serializes the messages into the buffer.
    MessageList::const_iterator begin = messages->begin(), end = messages->end();
    while (stream.IsWriteGood() && begin != end)
      (*begin++)->SerializeData(stream);
    
    ///Applies a hashing method or encryption method.
    PepperMessage(stream);

    ///Return the number of bytes written.
    return stream.Size();
  }

  ///Attempts to extract a message from the data in the buffer.
  int IProtocol::ExtractMessages( DataStream &stream, MessageList &messages)
  {
    ///If the message isn't a valid message, discard it.
    if (!ValidateMessage(stream))
      return 0;

    ///Strips the header and stores any data needed for message interpretation.
    StripHeader(stream);

    ///Builds a message from the data and info from the header. Keeps adding
    ///the size of each message to the offset until we've eaten all messages.
    return BuildMessages(stream,messages);
  }

} // Framework namespace

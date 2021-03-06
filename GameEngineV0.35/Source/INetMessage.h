#pragma once // Make sure this header is only included once.

#include "Message.h"
#include "Core.h"

namespace Framework
{
	class DataStream;

  ///NetMessage Id
  namespace NMid
  {
    ///NetMessage types.
    enum NetMessageIdType
    {
       Connection = 0
      ,Timeout
      ,RestartGame
      ,Update
      ,Create
      ,Destroy
      ,Stats
      ,KeepAlive
      ,InputBtn
			,InputMouse
      ,EndGame
      ,PlayerId

      ,NumIds ///< Keep track of how many there are.
    };
  }

  ///An interface for the NetMessages to be sent through the TCP and UDP sockets.
  class INetMessage : public Message
  {
  public:
    INetMessage( Mid::MessageIdType id = Mid::NetMessage ) : Message(id) {;}
    ///Makes sure all destructors are virtual.
    virtual ~INetMessage( void ) {;}

    ///Returns the type of message. Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const = 0;

    ///Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const = 0;

    ///Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const { return 0;}

    ///Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream ) {;}

    ///Sends itself out into the system.
    virtual void SendThis( void )
    {
      CORE->BroadcastMessage(this);
    }
  };

}

#pragma once

#include "INetMessage.h"

#include "NetUtilities.h"
#include "INetMessage.h"
#include "Message.h"
#include "VMath.h"
#include "Composition.h"

namespace Framework
{
	class CreateMessage : public INetMessage
	{
	public:
		virtual ~CreateMessage( void );

		// Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

		// Creates a carbon copy of the message.
		virtual INetMessage * Clone( void ) const;

		// Writes the contents of the message to the buffer of size size provided.
		virtual int SerializeData( DataStream &stream ) const;

		// Interprets the data in a buffer as the contents of its own message type.
		virtual void InterpretData( DataStream &stream );

		GOCType obj_type;
		GOCId id;
		Vec2 pos;
	};

  class DestroyMessage : public INetMessage
  {
  public:
    DestroyMessage( GOCId id_ = 0 ) : id(id_) {;}

    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const;

    // Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const;

    // Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream );

    GOCId id;
  };    // DestoryMessage

  class UpdateMessage : public INetMessage
  {
  public:
    UpdateMessage( GOCId id_, const Vec2 &pos_ ) : id(id_), pos(pos_) {;}

    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage* Clone( void ) const;

    // Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const;

    // Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream );

    GOCId id;   ///< Id of the object to update
    Vec2 pos;   ///< Position of the object now!
  };    // UpdateMessage

  class ConnectionMessage : public INetMessage
  {
  public:
    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const;

    // Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const;

    // Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream );

    std::string name;
    NetAddress address;
  };

}

#pragma once

#include "INetMessage.h"

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

		// Returns the size of the entire message, including the header.
		virtual unsigned Size( void ) const;

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
    DestroyMessage( GOCId id_ ) : id(id_) {;}

    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const;

    // Returns the size of the entire message, including the header.
    virtual unsigned Size( void ) const;

    // Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const;

    // Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream );

    GOCId id;
  };



}

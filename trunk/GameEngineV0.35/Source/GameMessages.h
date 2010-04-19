#pragma once

#include "INetMessage.h"

#include <string>

#include "NetUtilities.h"
#include "INetMessage.h"
#include "Message.h"
#include "VMath.h"
#include "Composition.h"

namespace Framework
{
	class EndGameMessage : public INetMessage
	{
	public:
    EndGameMessage( const std::string &winner = "", GOCId winnerId = 0 ) : winner_(winner),
      winnerId_(winnerId) {;}

		// Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

		// Creates a carbon copy of the message.
		virtual INetMessage * Clone( void ) const;

		// Writes the contents of the message to the buffer of size size provided.
		virtual int SerializeData( DataStream &stream ) const;

		// Interprets the data in a buffer as the contents of its own message type.
		virtual void InterpretData( DataStream &stream );

    std::string winner_;
    GOCId winnerId_;
	};  // EndGameMessage

  class RestartMessage : public INetMessage
  {
		// Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

		// Creates a carbon copy of the message.
		virtual INetMessage * Clone( void ) const;

    ///Sends itself out into the system.
    virtual void SendThis( void );
  };    // RestartMessage

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

    ///Sends itself out into the system.
    virtual void SendThis( void );

		std::string obj_type;
		GOCId id;
		Vec2 pos;
    float rot;
	};  // CreateMessage

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

    ///Sends itself out into the system.
    virtual void SendThis( void );

    GOCId id;
  };    // DestoryMessage

  class UpdateMessage : public INetMessage
  {
  public:
    UpdateMessage( GOCId id_ = 0, const Vec2 &pos_ = Vec2() ) : id(id_), pos(pos_) {;}

    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage* Clone( void ) const;

    // Writes the contents of the message to the buffer of size size provided.
    virtual int SerializeData( DataStream &stream ) const;

    // Interprets the data in a buffer as the contents of its own message type.
    virtual void InterpretData( DataStream &stream );

    ///Sends itself out into the system.
    virtual void SendThis( void );

    GOCId id;   ///< Id of the object to update
    Vec2 pos;   ///< Position of the object now!
    float rot;  ///< Rotation of the object now!
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

    ///Sends itself out into the system.
    virtual void SendThis( void );

    std::string name;
    NetAddress address;
  };

	class InputButtonMessage : public INetMessage
	{
	public:
		InputButtonMessage ();
		virtual ~InputButtonMessage( void );

		// Used when extracting messages from the list.
		virtual NMid::NetMessageIdType Type( void ) const;

		// Creates a carbon copy of the message.
		virtual INetMessage * Clone( void ) const;

		// Writes the contents of the message to the buffer of size size provided.
		virtual int SerializeData( DataStream &stream ) const;

		// Interprets the data in a buffer as the contents of its own message type.
		virtual void InterpretData( DataStream &stream );

    virtual void SendThis( void );

    // Object sending
    GOCId id;

		// keys -----
		int character;
		WPARAM key;
		// -------

	};  // InputButtonMessage

	class InputMouseMessage : public INetMessage
	{
	public:
		InputMouseMessage ();
		virtual ~InputMouseMessage( void );

		// Used when extracting messages from the list.
		virtual NMid::NetMessageIdType Type( void ) const;

		// Creates a carbon copy of the message.
		virtual INetMessage * Clone( void ) const;

		// Writes the contents of the message to the buffer of size size provided.
		virtual int SerializeData( DataStream &stream ) const;

		// Interprets the data in a buffer as the contents of its own message type.
		virtual void InterpretData( DataStream &stream );

		virtual void SendThis( void );

		enum MouseButtonIndexId
		{
			Invalid_Id,
			LeftMouse,
			RightMouse
		};

		// Object sending
		GOCId id;

		// mouse -----
		MouseButtonIndexId MouseButtonIndex;
		bool ButtonIsPressed;
		Vec2 MousePosition;
		// -------

	};  // InputMouseMessage

  class PlayerMessage : public INetMessage
  {
  public:
    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const;

    virtual int SerializeData( DataStream &stream ) const;
    virtual void InterpretData( DataStream &stream );

    virtual void SendThis( void );

    GOCId id;
  };

}

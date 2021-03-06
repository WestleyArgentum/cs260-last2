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
    EndGameMessage( const std::string &winner = "", const Vec4& color = Vec4() /*GOCId winnerId = 0*/ ) : winner_(winner), color_(color)
      /*winnerId_(winnerId)*/ {;}

		// Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

		// Creates a carbon copy of the message.
		virtual INetMessage * Clone( void ) const;

		// Writes the contents of the message to the buffer of size size provided.
		virtual int SerializeData( DataStream &stream ) const;

		// Interprets the data in a buffer as the contents of its own message type.
		virtual void InterpretData( DataStream &stream );

    std::string winner_;
    //GOCId winnerId_;
    Vec4 color_;
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
    CreateMessage( void ) : id(0), rot(0) {;}
    CreateMessage(const std::string &obj_type_, GOCId id_, Vec2 pos_, float rot_);
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
    UpdateMessage( GOCId id_ = 0, const Vec2 &pos_ = Vec2(), float rot_ = 0.f ) : id(id_), pos(pos_), rot(rot_) {;}

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

  class TimeoutMessage : public INetMessage
  {
  public:
    TimeoutMessage( void ) {;}
    TimeoutMessage(const NetAddress &adr) : address(adr) {;}
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

    NetAddress address;
  };

	class InputButtonMessage : public INetMessage
	{
	public:
		InputButtonMessage(int char_ = 0, WPARAM key_ = 0);
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
    PlayerMessage( GOCId id_ = 0, unsigned statsid_ = 0 ) : id(id_), statsid(statsid_) {;}
    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const;

    // Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const;

    virtual int SerializeData( DataStream &stream ) const;
    virtual void InterpretData( DataStream &stream );

    virtual void SendThis( void );

    GOCId id;
    unsigned statsid;
  };

  class KeepAliveMessage : public INetMessage
  {
  public:
    // Used when extracting messages from the list.
    virtual NMid::NetMessageIdType Type( void ) const { return NMid::KeepAlive; }

    // Creates a carbon copy of the message.
    virtual INetMessage * Clone( void ) const { return new KeepAliveMessage(); }

    virtual int SerializeData( DataStream &stream ) const { return 0; }
    virtual void InterpretData( DataStream &stream ) {}

    virtual void SendThis( void ) {}
  };

}


#include "Precompiled.h"
#include "GameMessages.h"
#include "GameStateManager.h"
#include "MessageHub.h"

namespace Framework
{
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// EndGameMessage
  NMid::NetMessageIdType EndGameMessage::Type( void ) const
  {
    return NMid::EndGame;
  }

  INetMessage* EndGameMessage::Clone( void ) const
  {
		return new EndGameMessage(*this);
  }

  int EndGameMessage::SerializeData( DataStream &stream ) const
  {
    StreamWrite( stream, winner_ );
    //StreamWrite( stream, winnerId_ );
    StreamWrite( stream, color_ );

    return 0;
  }

  void EndGameMessage::InterpretData( DataStream &stream )
  {
    StreamRead( stream, winner_ );
    //StreamRead( stream, winnerId_ );
    StreamRead( stream, color_ );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateMessage

  CreateMessage::CreateMessage(const std::string &obj_type_, GOCId id_, Vec2 pos_, float rot_)
    : obj_type(obj_type_), id(id_), pos(pos_), rot(rot_)
  {
  }

	CreateMessage::~CreateMessage( void )
	{}

  NMid::NetMessageIdType CreateMessage::Type( void ) const
	{
    return NMid::Create;
	}

	INetMessage * CreateMessage::Clone( void ) const
	{
		return new CreateMessage(*this);
	}

	int CreateMessage::SerializeData( DataStream &stream ) const
	{
		StreamWrite(stream, obj_type);
		StreamWrite(stream, id);
		StreamWrite(stream, pos);
    StreamWrite(stream, rot);

		return 0;
	}

	void CreateMessage::InterpretData( DataStream &stream )
	{
		StreamRead(stream, obj_type);
		StreamRead(stream, id);
		StreamRead(stream, pos);
    StreamRead(stream, rot);
  }

  ///Sends itself out into the system.
  void CreateMessage::SendThis( void )
  {
    GSM->SendMessage(this);
  }
 
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// DestroyMessage

  NMid::NetMessageIdType DestroyMessage::Type( void ) const
  {
    return NMid::Destroy;
  }

  INetMessage * DestroyMessage::Clone( void ) const
  {
    return new DestroyMessage(*this);
  }

  int DestroyMessage::SerializeData( DataStream &stream ) const
  {
    ///Write the object id.
    StreamWrite(stream,id);

    return 0;
  }

  void DestroyMessage::InterpretData( DataStream &stream )
  {
    //Read the object id.
    StreamRead(stream,id);
  }

  ///Sends itself out into the system.
  void DestroyMessage::SendThis( void )
  {
    GSM->SendMessage(this);
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateMessage
  NMid::NetMessageIdType UpdateMessage::Type( void ) const
  {
    return NMid::Update;
  }

  INetMessage* UpdateMessage::Clone( void ) const
  {
    return new UpdateMessage(*this);
  }

  int UpdateMessage::SerializeData( DataStream &stream ) const
  {
    ///Write the object id.
    StreamWrite(stream, id);
    StreamWrite(stream, pos);
    StreamWrite(stream, rot);

    return 0;
  }

  void UpdateMessage::InterpretData( DataStream &stream )
  {
    //Read the object id.
    StreamRead(stream, id);
    StreamRead(stream, pos);
    StreamRead(stream, rot);
  }

  ///Sends itself out into the system.
  void UpdateMessage::SendThis( void )
  {
    MessageHub->Direct(*this,id);
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// ConnectionMessage
  NMid::NetMessageIdType ConnectionMessage::Type( void ) const
  {
    return NMid::Connection;
  }

  INetMessage * ConnectionMessage::Clone( void ) const
  {
    return new ConnectionMessage(*this);
  }

  int ConnectionMessage::SerializeData( DataStream &stream ) const
  {
    ///Write the object id.
    StreamWrite(stream,name);
    StreamWrite(stream,address.ip_);
    StreamWrite(stream,address.port_);
    StreamWrite(stream,address.address);

    return 0;
  }

  void ConnectionMessage::InterpretData( DataStream &stream )
  {
    //Read the object id.
    StreamRead(stream,name);
    StreamRead(stream,address.ip_);
    StreamRead(stream,address.port_);
    StreamRead(stream,address.address);
  }

  ///Sends itself out into the system.
  void ConnectionMessage::SendThis( void )
  {
    GSM->SendMessage(this);
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// InputButtonMessage
	InputButtonMessage::InputButtonMessage(int char_, WPARAM key_) : character(char_), key(key_)
	{}

	InputButtonMessage::~InputButtonMessage( void )
	{}

	NMid::NetMessageIdType InputButtonMessage::Type( void ) const
	{
		return NMid::InputBtn;
	}

	INetMessage * InputButtonMessage::Clone( void ) const
	{
		return new InputButtonMessage(*this);
	}

	int InputButtonMessage::SerializeData( DataStream &stream ) const
	{
    StreamWrite(stream, id);
		StreamWrite(stream, character);
		StreamWrite(stream, key);

		/*StreamWrite(stream, MouseButtonIndex);
		StreamWrite(stream, ButtonIsPressed);
		StreamWrite(stream, MousePosition);*/

		return 0;
	}

	void InputButtonMessage::InterpretData( DataStream &stream )
	{
    StreamRead(stream, id);
		StreamRead(stream, character);
		StreamRead(stream, key);

		/*StreamRead(stream, MouseButtonIndex);
		StreamRead(stream, ButtonIsPressed);
		StreamRead(stream, MousePosition);*/
	}

	void InputButtonMessage::SendThis( void )
	{
    MessageHub->Direct(*this, id);
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// InputMouseMessage
	InputMouseMessage::InputMouseMessage () : MouseButtonIndex(Invalid_Id)
	{}

	InputMouseMessage::~InputMouseMessage( void )
	{}

	NMid::NetMessageIdType InputMouseMessage::Type( void ) const
	{
		return NMid::InputMouse;
	}

	INetMessage * InputMouseMessage::Clone( void ) const
	{
		return new InputMouseMessage(*this);
	}

	int InputMouseMessage::SerializeData( DataStream &stream ) const
	{
		StreamWrite(stream, MouseButtonIndex);
		StreamWrite(stream, ButtonIsPressed);
		StreamWrite(stream, MousePosition);

		return 0;
	}

	void InputMouseMessage::InterpretData( DataStream &stream )
	{
		StreamRead(stream, MouseButtonIndex);
		StreamRead(stream, ButtonIsPressed);
		StreamRead(stream, MousePosition);
	}

	void InputMouseMessage::SendThis( void )
	{}


  // Used when extracting messages from the list.
  NMid::NetMessageIdType PlayerMessage::Type( void ) const
  {
    return NMid::PlayerId;
  }

  // Creates a carbon copy of the message.
  INetMessage * PlayerMessage::Clone( void ) const
  {
    return new PlayerMessage(*this);
  }
  
	int PlayerMessage::SerializeData( DataStream &stream ) const
	{
		StreamWrite(stream, id);
    StreamWrite(stream, statsid);

		return 0;
	}

	void PlayerMessage::InterpretData( DataStream &stream )
	{
		StreamRead(stream, id);
    StreamRead(stream, statsid);
	}

  
  void PlayerMessage::SendThis( void )
  {
    GSM->SendMessage(this);
  }
}   // namespace Framework


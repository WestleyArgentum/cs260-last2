
#include "Precompiled.h"
#include "GameMessages.h"
#include "GameStateManager.h"
#include "MessageHub.h"

namespace Framework
{
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateMessage
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
		if(stream.IsWriteGood())
		{
			StreamWrite(stream, obj_type);
			StreamWrite(stream, id);
			StreamWrite(stream, pos);
		}

		return 0;
	}

	void CreateMessage::InterpretData( DataStream &stream )
	{
		if (stream.IsReadGood())
			StreamRead(stream, obj_type);
		if (stream.IsReadGood())
			StreamRead(stream, id);
		if (stream.IsReadGood())
			StreamRead(stream, pos);
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

    return 0;
  }

  void UpdateMessage::InterpretData( DataStream &stream )
  {
    //Read the object id.
    StreamRead(stream, id);
    StreamRead(stream, pos);
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
    StreamWrite(stream,address);

    return 0;
  }

  void ConnectionMessage::InterpretData( DataStream &stream )
  {
    //Read the object id.
    StreamRead(stream,name);
    StreamRead(stream,address);
  }

  ///Sends itself out into the system.
  void ConnectionMessage::SendThis( void )
  {
    GSM->SendMessage(this);
  }

}   // namespace Framework


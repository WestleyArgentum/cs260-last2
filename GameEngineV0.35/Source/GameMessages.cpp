
#include "Precompiled.h"
#include "GameMessages.h"

namespace Framework
{

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

	unsigned CreateMessage::Size( void ) const
	{
		return 0;
	}

	int CreateMessage::SerializeData( DataStream &stream ) const
	{
		return 0;
	}

	void CreateMessage::InterpretData( DataStream &stream )
	{

	}

  NMid::NetMessageIdType DestroyMessage::Type( void ) const
  {
    return NMid::Destroy;
  }

  INetMessage * DestroyMessage::Clone( void ) const
  {
    return new DestroyMessage(*this);
  }

  unsigned DestroyMessage::Size( void ) const
  {
    return 0;
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

}
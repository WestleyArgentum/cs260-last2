
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
		return NULL;
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

}
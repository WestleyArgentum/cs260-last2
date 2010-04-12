
// MessageHub.cpp : Distributes messages to registered compositions

#include "Precompiled.h"

#include <algorithm>

#include "MessageHub.h"
#include "Factory.h"

namespace Framework
{

	void Message_Hub::Post( Message& msg )
	{
		EntityIds& ids = entities_per_msgtype[msg.MessageId];
		
		for(EntityIds::iterator it = ids.begin(); it != ids.end(); ++it)
		{
			// resolve the id
			GameObjectComposition* entity = FACTORY->GetObjectWithId(*it);
			if (!entity)
				ids.erase(it);
			else
				entity->SendMessage(&msg);  // entity will send the message to the interested component
		}
	}

	void Message_Hub::Register( GOCId entity_id, Mid::MessageIdType msg_type )
	{
		EntityIds& ids = entities_per_msgtype[msg_type];

		// if the id is not in the vector already then we want to insert it
		if (std::find(ids.begin(), ids.end(), entity_id) == ids.end())
			ids.push_back(entity_id);
	}

	Message_Hub* Message_Hub::GetInstance( void )
	{
		static Message_Hub hubhub;

		return &hubhub;
	}
}
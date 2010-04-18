
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
		
    // Rob: Note...needed to restructure this slightly to properly erase entities when they weren't
    //  found.

    EntityIds::iterator it = ids.begin();

		while ( it != ids.end() )
		{
			// resolve the id
			GameObjectComposition* entity = FACTORY->GetObjectWithId(*it);

			if (!entity)
      {
        EntityIds::iterator prev = it;
        ++it;   // Step ahead before invalidating (erasing) the iterator

				ids.erase(prev);
      }
			else
      {
				entity->SendMessage(&msg);  // entity will send the message to the interested component
        ++it;
      }
		}
	}

  // Direct a message to a specific entity.
  void Message_Hub::Direct(Message& msg, GOCId id)
  {
    GOC *obj = FACTORY->GetObjectWithId(id);

    if (obj)
      obj->SendMessage(&msg);
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
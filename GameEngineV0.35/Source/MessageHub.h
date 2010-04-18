
// MessageHub.h : Distributes messages to registered compositions

#include <map>
#include <vector>

#include "Composition.h"
#include "Message.h"

namespace Framework
{

	class Message_Hub
	{
	public:
		static Message_Hub* GetInstance( void );  // Singleton

		// Post a message to all entities that are listening.
		void Post (Message& msg);

    // Direct a message to a specific entity.
    void Direct(Message& msg, GOCId id);

		// Register your entity today!
		// Components that are interested in getting messages should register their entities with the hub.
		// Don't worry about registering an entity more than once.
		void Register (GOCId entity_id, Mid::MessageIdType msg_type );

		// No unregister method is needed. The message hub will resolve each of the id's into pointers and
		// if an entity has been deleted the hub will know and will remove that id from the map.

	private:
		typedef std::vector<GOCId> EntityIds;
		typedef std::map<Mid::MessageIdType, EntityIds> MessageEntityMap;

		MessageEntityMap entities_per_msgtype;
	};

	#define MessageHub Message_Hub::GetInstance()

}
/*
base message creator.  You may need to change this to be able to handle messages that come from the 
network, although since it's a base class really be sure to ask yourself if the functionality you're adding
is truly relevant to all the derived classes.  Changing this should have a valid reason for changing each of your
derived message creators.
*/
#pragma once

#include "Message.h"


class MessageCreator
{
public:
	MessageCreator(void);
	~MessageCreator(void);
	virtual Message* CreateMessage(int message) = 0;
};


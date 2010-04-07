#include "Controller.h"
//
Controller::Controller(Model* m, View* v){

}

Controller::~Controller(void)
{
	while(messages.size() > 0)
	{
		Message* msg = messages.front();
		messages.pop_front();
		delete msg;
	}
}
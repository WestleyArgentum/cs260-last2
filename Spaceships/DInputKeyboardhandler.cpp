#include "DInputKeyboardhandler.h"




DInputKeyboardhandler::DInputKeyboardhandler(LPDIRECTINPUTDEVICE8 keyboard)
{
	this->_keyboard = keyboard;
	takeSnapshot = true;
	this->_messageCreator = NULL;
}

DInputKeyboardhandler::~DInputKeyboardhandler(void)
{
	}

Message* DInputKeyboardhandler::GetNextMessage(){

	//see if we need to get messages from the keyboard.  only do this once
	if(takeSnapshot){
		this->SnapShot();
		this->takeSnapshot = false;
	}

	if(this->messages.empty() && takeSnapshot == false){
		takeSnapshot = true;
		return NULL;
	}

	Message* m = messages.front();
	messages.pop_front();
	return m;
}

MessageCreator* DInputKeyboardhandler::GetMessageCreator(void){

	
	if(this->_messageCreator == NULL)
	{
		this->_messageCreator = new DIKeyboardMessageCreator();
		GlobalDebug::GetInstance()->DbgOut("Created DIKeyboardmessageCreator");
	}

	return this->_messageCreator;
}	
	
void DInputKeyboardhandler::PassMessage(Message* msg){

	messages.push_back(msg);	

}

void DInputKeyboardhandler::SnapShot(){
	HRESULT hr;
	hr = this->_keyboard->Acquire();

	//go through this thing and create some messages and stuff
	DIDEVICEOBJECTDATA kbData[DIKEYBOARDBUFFERSIZE];
	SecureZeroMemory(kbData, sizeof(DIDEVICEOBJECTDATA));
	DWORD bufferSize = DIKEYBOARDBUFFERSIZE;

	hr = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), kbData, &bufferSize, 0);
	while(hr == DIERR_NOTACQUIRED){
		hr = _keyboard->Acquire();
	}

	BYTE keysdown[256];
	ZeroMemory(keysdown, sizeof(keysdown));
	
	Message* temp = NULL;
	MessageCreator* creator = NULL;

	creator = this->GetMessageCreator();


	hr = _keyboard->GetDeviceState(sizeof(keysdown), (LPVOID)keysdown);
	if(hr == DI_OK){
		for(int i = 0; i < 256; i++)
			if(keysdown[i] & 0x80){
				//GlobalDebug::GetInstance()->DbgOutInt("\r\nKey Down was: ", i);
				temp = creator->CreateMessage(i);
				//GlobalDebug::GetInstance()->DbgOut("\r\nEnd Message Generation");

				if(temp != NULL)
					this->PassMessage(temp);
			}
	}
	
	

}
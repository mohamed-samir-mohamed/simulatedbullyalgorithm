#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

#include <list>
#include "GeneralDefinations.h"
#include <windows.h>
#include <windef.h>
#include "Message.h"

using namespace std;

//forward declarations.
class IMessageHandler;

class MessageRouter
{
    /************************************************************************/
    /* this class is responsible for sending messages between nodes (processes) 
    /************************************************************************/
public:
	MessageRouter();
	~MessageRouter();
	void broadCastMessage(Message fMessage);
	void sendMessageTo( Message fMessage, ID fRecieverID);


	static HWND Hwnd;//handle for the main window of this process
	static UINT UNIQUE_MESSAGE_ID;//unique ID for communicating between processes -- throw RegisterWindowMessage() method.

};
#endif // !MESSAGEROUTER_H
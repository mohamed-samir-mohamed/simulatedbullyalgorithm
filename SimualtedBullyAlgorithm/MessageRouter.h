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
   static void broadCastMessage(Message fMessage);
   static void sendMessageTo( Message fMessage, ID fRecieverID);

   static UINT BROADCAST_MESSAGE_ID;//unique ID for communicating between processes -- throw RegisterWindowMessage() method.
   static HWND Hwnd;//handle for the main window of this process
   static Message MessageToBeSent;//static message to be sent.
   static COPYDATASTRUCT mCopyStructure;//static structure to be sent.
};
#endif // !MESSAGEROUTER_H
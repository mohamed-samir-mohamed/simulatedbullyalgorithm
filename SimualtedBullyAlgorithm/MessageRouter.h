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
public:
   static void addHandler(ID fHandlerID);
   static void removeHandler(ID fID);
   static void broadCastMessage(Message fMessage);
   static void sendMessageTo( Message fMessage, ID fRecieverID);

   static UINT BROADCAST_MESSAGE_ID;
   static HWND Hwnd;

private:
   static list<ID>  mHandlersList;
};
#endif // !MESSAGEROUTER_H
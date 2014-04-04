#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

#include <map>
#include "GeneralDefinations.h"

using namespace std;

//forward declarations.
class Message;
class IMessageHandler;

class MessageRouter
{
public:
   static void addHandler(IMessageHandler* fHandler);
   static void removeHandler(ID fID);
   static void broadCastMessage(Message* fMessage);
   static void sendMessageTo(Message* fMessage, ID fRecieverID);

private:
   static map<ID, std::unique_ptr<IMessageHandler>>  mHandlersMap;
};
#endif // !MESSAGEROUTER_H
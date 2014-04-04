#include "MessageRouter.h"
#include "MessageInterfaces.h"
#include "Message.h"


void MessageRouter::addHandler(IMessageHandler* fHandler )
{
    mHandlersMap[fHandler->getID()] = std::unique_ptr<IMessageHandler>(fHandler);
}

void MessageRouter::removeHandler( ID fID )
{
    mHandlersMap.erase(fID);
}

void MessageRouter::broadCastMessage( Message* fMessage )
{
    for (auto it = mHandlersMap.begin(); it != mHandlersMap.end(); it++)
    {
        //broad cast message for all handlers except the sender itself && the message is not handled.
        if (it->second->getID() != fMessage->getSenderID() && fMessage->isMessageHandled() == false)
        {
            it->second->handleMessage(fMessage);
        }
    }
} 

void MessageRouter::sendMessageTo( Message* fMessage, ID fRecieverID )
{
   if(mHandlersMap.find(fRecieverID) != mHandlersMap.end())
   {
       mHandlersMap[fRecieverID]->handleMessage(fMessage);
   }
}

map<ID, std::unique_ptr<IMessageHandler>> MessageRouter::mHandlersMap;

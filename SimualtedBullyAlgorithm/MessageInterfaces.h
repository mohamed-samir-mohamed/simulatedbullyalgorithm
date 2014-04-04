#ifndef MESSAGEINTERFACES_H
#define MESSAGEINTERFACES_H
#include "GeneralDefinations.h"

class Message;

class IMessageHandler
{
public:
   virtual void handleMessage(Message* fMessage) = 0;
   virtual ID getID() const = 0;
};
#endif //! MESSAGEINTERFACES_H
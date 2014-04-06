#ifndef MESSAGE_H
#define MESSAGE_H
#include "GeneralDefinations.h"
#include <string>
using namespace std;
struct Message
{
    //this is the message structure.
public:
    enum MessageType
    {
        I_AM_JUST_CHECKING_COORDINATOR, 
        ELECTION_REQUEST, 
        I_AM_STILL_ALIVE, 
        I_AM_THE_COORDINATOR, 
        I_AM_GREATER_THAN_YOU
    };
    Message();
    Message(MessageType fType, ID fSenderID);

    //data...
    MessageType type;
    string getStringMessage();
    ID senderID;
    bool isHandled;
};

#endif //!MESSAGE_H
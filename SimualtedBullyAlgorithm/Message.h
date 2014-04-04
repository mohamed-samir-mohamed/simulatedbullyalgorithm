#ifndef MESSAGE_H
#define MESSAGE_H
#include "GeneralDefinations.h"

class Message
{
public:
    enum MessageType
    {
        I_AM_JUST_CHECKING_COORDINATOR, ELECTION_REQUEST, I_AM_STILL_ALIVE, I_AM_THE_COORDINATOR, I_AM_GREATER_THAN_YOU
    };

    Message(MessageType fType, ID fSenderID, void* fData = nullptr);
    MessageType getType() const;
    ID getSenderID() const;
    bool isMessageHandled();
    void setHandled();

private:
    MessageType mType;
    void* mMessageData;
    ID mSenderID;
    bool mIsHandled;
};

#endif //!MESSAGE_H
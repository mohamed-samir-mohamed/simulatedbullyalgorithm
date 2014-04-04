#include "Message.h"


Message::Message( MessageType fType, ID fSenderID, void* fData ): mType(fType), mMessageData(fData), mSenderID (fSenderID), mIsHandled (false)
{

}

Message::MessageType Message::getType() const
{
    return mType;
}

ID Message::getSenderID() const
{
    return mSenderID;
}

bool Message::isMessageHandled()
{
    return mIsHandled;
}

void Message::setHandled()
{
    mIsHandled = true;
}

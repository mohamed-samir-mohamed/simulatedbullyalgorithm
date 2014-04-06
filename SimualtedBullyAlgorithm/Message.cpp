#include "Message.h"


Message::Message( MessageType fType, ID fSenderID ): type(fType), senderID (fSenderID), isHandled (false)
{

}

Message::Message()
{

}
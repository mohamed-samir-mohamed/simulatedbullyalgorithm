#include "Message.h"


Message::Message( MessageType fType, ID fSenderID ): type(fType), senderID (fSenderID), isHandled (false)
{

}

Message::Message()
{

}

string Message::getStringMessage()
{
	//stringfy the message type for log information.
    switch(type)
    {
    case I_AM_GREATER_THAN_YOU:
        return " -- I am greater than you -- ";
    case  ELECTION_REQUEST:
        return " -- Election Request -- ";
    case I_AM_JUST_CHECKING_COORDINATOR:
        return " -- I am Checking the coordinator -- ";
    case I_AM_STILL_ALIVE:
        return " -- I am still alive -- ";
    case I_AM_THE_COORDINATOR:
        return " -- I am the coordinator -- ";
    }
	return "";
}

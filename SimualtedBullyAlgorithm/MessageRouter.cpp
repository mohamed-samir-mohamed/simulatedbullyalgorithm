#include "MessageRouter.h"
#include "MessageInterfaces.h"
#include "Message.h"
#include "GeneralDefinations.h"

#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

MessageRouter::MessageRouter()
{
	MessageToBeSent = new Message();
	mCopyStructure = new COPYDATASTRUCT();
}

void MessageRouter::broadCastMessage( Message fMessage )
{
    //log some information.
    cout<<"node "<<fMessage.senderID<<" Attempts to broadCast Message of type "<<fMessage.getStringMessage()<<endl;

    //copy the message to be sent to variable to guarantee the existence of it while the receiver access this information, 
    MessageToBeSent->type = fMessage.type;
    MessageToBeSent->senderID = fMessage.senderID;
    MessageToBeSent->isHandled = fMessage.isHandled;

    //prepare copy structure to send through WinAPI.
    mCopyStructure->cbData = sizeof(Message);
    mCopyStructure->lpData = MessageToBeSent;

    //broadcast this message to all windows with my unique created ID through RegisterWindowMessage
    SendMessage(HWND_BROADCAST, UNIQUE_MESSAGE_ID, (WPARAM)(Hwnd), (LPARAM) ( (LPVOID) mCopyStructure));

    //the problem here access denied may be returned try run as administrator. 
    DWORD dwError = GetLastError();

    if (dwError != NO_ERROR)
       cout<<"error while sending message error code "<< dwError <<endl; 
} 

void MessageRouter::sendMessageTo( Message fMessage, ID fRecieverID )
{
    //sends message to specific window by its name.
    cout<<"node "<<fMessage.senderID<<" Attempts to send Message of type "<<fMessage.getStringMessage()<<" to node "<<fRecieverID<<endl;
    
    //prepare the static structure to be sent.
    MessageToBeSent->senderID = fMessage.senderID;
    MessageToBeSent->type = fMessage.type;
    MessageToBeSent->isHandled = fMessage.isHandled;

    //find the handle for the window to be sent.
    HWND hTargetWnd = FindWindow(NULL, to_wstring(fRecieverID).c_str());
    if (hTargetWnd != NULL)
    {
        mCopyStructure->cbData = sizeof(Message);
        mCopyStructure->lpData = MessageToBeSent;

        SendMessage(hTargetWnd, UNIQUE_MESSAGE_ID, (WPARAM)(Hwnd), (LPARAM)(mCopyStructure));

        DWORD dwError = GetLastError();

        if (dwError != NO_ERROR)
            cout<<"error while sending message error code "<< dwError <<endl;
    }
}

MessageRouter::~MessageRouter()
{
	delete MessageToBeSent;
    delete mCopyStructure;
}

HWND MessageRouter::Hwnd;

UINT MessageRouter::UNIQUE_MESSAGE_ID;

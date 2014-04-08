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
}

void MessageRouter::broadCastMessage( Message fMessage )
{
    //log some information.
    cout<<"node "<<fMessage.senderID<<" Attempts to broadCast Message of type "<<fMessage.getStringMessage()<<endl;

    //prepare copy structure to send through WinAPI.
	COPYDATASTRUCT myCopyStructure;

	myCopyStructure.dwData = UNIQUE_MESSAGE_ID;//send the unique message ID to the receiver.
	myCopyStructure.cbData = sizeof(Message);
    myCopyStructure.lpData = &fMessage;

    //broadcast this message to all windows with my unique created ID through RegisterWindowMessage
    SendMessage(HWND_BROADCAST, WM_COPYDATA, (WPARAM)(Hwnd), (LPARAM)(LPVOID)&myCopyStructure);
    //the problem here access denied may be returned try run as administrator. 
    DWORD dwError = GetLastError();

    if (dwError != NO_ERROR)
       cout<<"error while sending message error code "<< dwError <<endl; 
} 

void MessageRouter::sendMessageTo( Message fMessage, ID fRecieverID )
{
    //sends message to specific window by its name.

	//log some info.
    cout<<"node "<<fMessage.senderID<<" Attempts to send Message of type "<<fMessage.getStringMessage()<<" to node "<<fRecieverID<<endl;
    
    //find the handle for the window to be sent.
    HWND hTargetWnd = FindWindow(NULL, to_wstring(fRecieverID).c_str());
    if (hTargetWnd != NULL)
    {

		COPYDATASTRUCT myCopyStructure;

		myCopyStructure.dwData = UNIQUE_MESSAGE_ID;//send the unique message ID to the receiver.
        myCopyStructure.cbData = sizeof(Message);
        myCopyStructure.lpData = &fMessage;

        SendMessage(hTargetWnd, WM_COPYDATA , (WPARAM)(Hwnd), (LPARAM)(LPVOID) &myCopyStructure);

        DWORD dwError = GetLastError();

        if (dwError != NO_ERROR)
            cout<<"error while sending message error code "<< dwError <<endl;
    }
}

MessageRouter::~MessageRouter()
{
}

HWND MessageRouter::Hwnd;

UINT MessageRouter::UNIQUE_MESSAGE_ID;

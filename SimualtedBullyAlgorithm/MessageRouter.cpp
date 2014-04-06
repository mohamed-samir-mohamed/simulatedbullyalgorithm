#include "MessageRouter.h"
#include "MessageInterfaces.h"
#include "Message.h"
#include <string>
#include <algorithm>
#include "GeneralDefinations.h"

void MessageRouter::addHandler(ID fHandlerID )
{
    mHandlersList.push_back(fHandlerID);
}

void MessageRouter::removeHandler( ID fID )
{
    std::remove(mHandlersList.begin(),mHandlersList.end(), fID);
}

void MessageRouter::broadCastMessage( Message fMessage )
{
        COPYDATASTRUCT cds;
        cds.cbData = sizeof(fMessage);
        cds.lpData = &fMessage;

        SendMessage(HWND_BROADCAST, BROADCAST_MESSAGE_ID, reinterpret_cast<WPARAM>(Hwnd), 
            reinterpret_cast<LPARAM>(&cds));
} 

void MessageRouter::sendMessageTo( Message fMessage, ID fRecieverID )
 {
     HWND hTargetWnd = FindWindow(NULL, to_wstring(fRecieverID).c_str());
     if (hTargetWnd != NULL)
     {
         COPYDATASTRUCT cds;
         cds.cbData = sizeof(fMessage);
         cds.lpData = &fMessage;

         SendMessage(hTargetWnd, WM_COPYDATA, reinterpret_cast<WPARAM>(Hwnd), 
             reinterpret_cast<LPARAM>(&cds));
     }
}

UINT MessageRouter::BROADCAST_MESSAGE_ID;

HWND MessageRouter::Hwnd;

list<ID> MessageRouter::mHandlersList;
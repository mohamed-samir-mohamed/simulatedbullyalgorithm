#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include "ElectableNode.h"
#include "MessageRouter.h"
#include <winuser.h>

/*global definitions*/
//each instance of this application composes a node class to handle the logic.
ElectableNode node;

//handle window for the text label in the window.
HWND hWndEdit = NULL;

//callback to handle the messages sent to this window.
LRESULT CALLBACK windowProc(HWND hnwd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// method for handling the received  message.
BOOL OnCopyData(HWND hWnd, HWND hwndFrom, PCOPYDATASTRUCT pcds);

//Application entry point.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR cmdLine,int cmdShow)
{
#pragma region creating console and enable it.
    AllocConsole();
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
#pragma endregion

    //create a unique ID for communicating between processes.
    MessageRouter::UNIQUE_MESSAGE_ID = RegisterWindowMessage(L"BullyAlgorithmBroadCastMessage");

	cout <<"enter the ID for this Process  (Hint !!)--> uniqueness must be guaranteed by you"<<endl;
	ID iD;
	cin>>iD;
    node.setID(iD);

#pragma region create main window and text menu.

    const wchar_t CLASS_NAME[] = L"windowClass";

    WNDCLASS wc = {};

    wc.lpfnWndProc = windowProc;
    wc.lpszClassName = CLASS_NAME;
    wc.hInstance = hInstance;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, to_wstring(iD).c_str(), WS_OVERLAPPEDWINDOW, 0, 0, 150, 110, NULL, NULL, hInstance, NULL);
    MessageRouter::Hwnd = hwnd;
    if (hwnd == 0)
        return 0;
    hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE , 0, 0, 140, 100, hwnd,(HMENU) 1, NULL, NULL);

    ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);
    cmdShow = 1;
#pragma endregion

	
	

    MSG msg = {};

    while(true)
    {
        GetMessage(&msg, NULL, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        node.update();
    }
    return 0;
}

LRESULT CALLBACK windowProc(HWND hnwd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::wstring prefix = L"Coordinator ID --> ";
	size_t size = sizeof(long long) + prefix.length() + 1;
	wchar_t* displayMessage = new wchar_t[size];
	memset(displayMessage,0, size);
	wsprintf(displayMessage , L"%s%ld" , prefix.c_str() , node.getCoordinatorID());

    switch(uMsg)
    {
	case WM_COPYDATA:
		SetWindowText(hWndEdit,displayMessage);
		if(((PCOPYDATASTRUCT)(lParam))->dwData != MessageRouter::UNIQUE_MESSAGE_ID) //check the unique message ID I have sent.
			ReplyMessage(false);
		else
			ReplyMessage(OnCopyData(hnwd, (HWND) wParam, (PCOPYDATASTRUCT)(lParam)));
		return 0;

		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        exit(0);
        return 0;

    case  WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hnwd,&ps);
            FillRect(hdc,&ps.rcPaint,(HBRUSH(COLOR_WINDOW + 5)));
            EndPaint(hnwd,&ps);
        }
        return 0;

    case WM_CLOSE:
        {
            if(MessageBox(hnwd, L"Are you Sure ?", L"EXIT",MB_OKCANCEL)== IDOK)
                DestroyWindow(hnwd);
        }
        return 0;
    }

    return DefWindowProc(hnwd,uMsg,wParam,lParam);
}

BOOL OnCopyData(HWND hWnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
    Message receivedMessage;
	receivedMessage = *((Message*) pcds->lpData);
	node.handleMessage(receivedMessage);
	return true;
}
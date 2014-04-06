#include <Windows.h>
#include "ElectableNode.h"
#include "MessageRouter.h"
#include <windowsx.h>
#include <string>

/*global definitions*/
ElectableNode node;
HWND hWndEdit = NULL;
#define CoordinatorIDText 1

LPCWSTR displayMessage;

LRESULT CALLBACK windowProc(HWND hnwd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL OnCopyData(HWND hWnd, HWND hwndFrom, PCOPYDATASTRUCT pcds);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR cmdLine,int cmdShow)
{

    MessageRouter::BROADCAST_MESSAGE_ID = RegisterWindowMessage(L"BullyAlgorithmBroadCastMessage");

    node.setID(_wtoi64(cmdLine));
    const wchar_t CLASS_NAME[] = L"windowClass";

    WNDCLASS wc = {};

    wc.lpfnWndProc = windowProc;
    wc.lpszClassName = CLASS_NAME;
    wc.hInstance = hInstance;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, cmdLine, WS_OVERLAPPEDWINDOW, 0, 0, 150, 110, NULL, NULL, hInstance, NULL);
    MessageRouter::Hwnd = hwnd;
    if (hwnd == 0)
        return 0;
    hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE , 0, 0, 140, 100, hwnd,(HMENU) CoordinatorIDText, NULL, NULL);

    ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);
    cmdShow = 1;

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
   
    if (uMsg == MessageRouter::BROADCAST_MESSAGE_ID)
    {
         OnCopyData(hnwd, (HWND) wParam, (PCOPYDATASTRUCT) lParam);
         SetWindowText(hWndEdit, to_wstring(node.getCoordinatorID()).c_str());
         ReplyMessage(TRUE);
    }

    switch(uMsg)
    {
    case WM_COPYDATA:
        OnCopyData(hnwd, (HWND) wParam, (PCOPYDATASTRUCT) lParam);
        SetWindowText(hWndEdit,  to_wstring(node.getCoordinatorID()).c_str());
        ReplyMessage(TRUE);
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
    // If the size matches
    if (pcds == NULL)
    {
        return FALSE;
    }
    if (pcds->cbData == sizeof(receivedMessage))
    {    
        memcpy_s(&receivedMessage, sizeof(receivedMessage), pcds->lpData, pcds->cbData);
    }
     
    node.handleMessage(receivedMessage);

    return TRUE;
}
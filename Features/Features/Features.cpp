// Features.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Features.h"
#include "MainWindow.h"
#include "..\..\Dui\Core\UIManager.h"

using namespace DuiLib;

CMainWindow theMain;

BOOL InitApp();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    InitApp();
    

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FEATURES));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

BOOL InitApp() {
    WCHAR buf[MAX_PATH] = { 0 };
    ::GetModuleFileName(NULL, buf, MAX_PATH);
    std::wstring path = buf;
    path = path.substr(0, path.find_last_of(L"\\"));
    path += L"\\skin";
    CPaintManagerUI::SetResourcePath(path.c_str());

    // 创建主窗口
    theMain.Create(NULL, L"Features", WS_POPUP, 0);
    HWND hWnd = theMain.GetHWND();
    if (!hWnd) {
        return FALSE;
    }

    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    RECT rect = { 0,0,0,0 };
    ::GetWindowRect(hWnd, &rect);
    int main_height = rect.bottom - rect.top;
    int main_width = rect.right - rect.left;
    int x = (screen_width - main_width) / 2;
    int y = (screen_height - main_height) / 2;
    ::SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    UpdateWindow(hWnd);

    return TRUE;
}
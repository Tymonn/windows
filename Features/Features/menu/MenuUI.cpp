#include "stdafx.h"
#include "MenuUI.h"
#include <commdlg.h>
#include "../define/msg_define.h"


CMenuUI::CMenuUI()
{
}


CMenuUI::~CMenuUI()
{
}

LPCTSTR CMenuUI::GetWindowClassName() const {
    return L"DuiImageMenu";
}

LRESULT CMenuUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    BOOL handled = FALSE;
    LRESULT result = 0;
    switch (uMsg) {
    case WM_CREATE:
        OnCreate(uMsg, wParam, lParam, result);
        break;
    }

    if (handled) {
        return result;
    }

    if (pntm_.MessageHandler(uMsg, wParam, lParam, result)) {
        return result;
    }

    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMenuUI::Notify(TNotifyUI& msg) {
    if (!msg.pSender) {
        return;
    }

    if (msg.sType == DUI_MSGTYPE_CLICK) {
        std::wstring name = msg.pSender->GetName();
        if (name == L"about") {
            
        }
        else if (name == L"exit") {
            ::PostQuitMessage(0);
        }
        else if (name == L"open_file") {
            
        }
        else if (name == L"create_sub") {
            ::PostMessage(::GetParent(m_hWnd), WM_CREATE_SUBWND, 0, 0);
        }
        else if (name == L"create_transparent") {
            ::PostMessage(::GetParent(m_hWnd), WM_CREATE_TRANSPARENT, 0, 0);
        }

        ::ShowWindow(m_hWnd, SW_HIDE);
    }
}

LRESULT CMenuUI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &result) {
    result = TRUE;
    pntm_.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI *root = builder.Create(L"MenuUI.xml", 0, NULL, &pntm_, NULL);
    if (root) {
        pntm_.AttachDialog(root);
        pntm_.AddNotifier(this);
        SetSubControls();
    }

    return 0;
}

void CMenuUI::SetSubControls() {
    about_ = static_cast<CButtonUI*>(pntm_.FindControl(L"about"));
    //if (about_) {
    //    
    //}
    exit_ = static_cast<CButtonUI*>(pntm_.FindControl(L"exit"));
}
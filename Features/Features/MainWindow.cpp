#include "stdafx.h"
#include "MainWindow.h"
#include "define\msg_define.h"

CMainWindow::CMainWindow()
{
}


CMainWindow::~CMainWindow() {
    for (auto sub : subs_) {
        delete sub;
    }
}

LPCTSTR CMainWindow::GetWindowClassName() const {
    return L"TypeApp MainWindow";
}

LRESULT CMainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    BOOL handled = FALSE;
    LRESULT result = 0;
    switch (uMsg)
    {
        case WM_CREATE: {
            result = OnCreate(uMsg, wParam, lParam, handled);
        }
            break;
        case WM_NCHITTEST: {
            result = OnNcHitTest(uMsg, wParam, lParam, handled);
        }
            break;
        case WM_RBUTTONUP: {
            POINT pt = { 0 };
            ::GetCursorPos(&pt);
            RECT rect = { 0 };
            ::GetWindowRect(m_hWnd, &rect);
            pt.x -= rect.left;
            pt.y -= rect.top;

            if (!::IsWindow(menu_.GetHWND())) {
                menu_.Create(m_hWnd, L"Feature Menu", WS_CHILD, 0);    
                ::SetWindowPos(menu_.GetHWND(), NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
            }
            else {
                ::SetWindowPos(menu_.GetHWND(), NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
            }
        }
            break;
        case WM_PAINT: {
            int m = 0;
            break;
        }
        case WM_CREATE_SUBWND: {
            POINT pt = { 0 };
            ::GetCursorPos(&pt);
            RECT rect = { 0 };
            ::GetWindowRect(m_hWnd, &rect);
            pt.x -= rect.left;
            pt.y -= rect.top;

            static int id = 0;
            ++id;
            CSubWindow *sub = new CSubWindow(id);
            subs_.push_back(sub);
            sub->Create(m_hWnd, L"Feature Sub", id % 2 == 1 ? WS_CHILD : WS_CHILD| WS_CLIPSIBLINGS, 0);
            ::SetWindowPos(sub->GetHWND(), NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        }
            break;
        default:
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

LRESULT CMainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = TRUE;

    pntm_.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI *root = builder.Create(L"Main.xml", 0, NULL, &pntm_, NULL);
    if (root) {
        pntm_.AttachDialog(root);
        FindSubCtrls();
    }

    return 0;
}

LRESULT CMainWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = TRUE;
    if (caption_) {
        short x = GET_X_LPARAM(lParam);
        short y = GET_Y_LPARAM(lParam);
        POINT pt = { x,y };

        int height = caption_->GetFixedHeight();        
        RECT rect = {0};
        ::GetWindowRect(m_hWnd, &rect);
        rect.bottom = rect.top + height;
        if (::PtInRect(&rect, pt)) {
            //FindControl的pt须是相对窗口的坐标
            pt.x -= rect.left;
            pt.y -= rect.top;
            CControlUI* ctrl = pntm_.FindControl(pt);
            if (ctrl) {
                std::wstring class_name = ctrl->GetClass();
                if (class_name == DUI_CTR_BUTTON) {
                    return HTCLIENT;
                }                
            }

            return HTCAPTION;
        }
    }

    return HTCLIENT;
}

void CMainWindow::FindSubCtrls() {
    close_ = static_cast<CButtonUI*>(pntm_.FindControl(L"close"));
    if (close_) {
        close_->Subscribe(CEventSets::EventClick, MakeDelegate(this, &CMainWindow::OnCloseClicked));
    }

    caption_ = static_cast<CHorizontalLayoutUI*>(pntm_.FindControl(L"caption"));
}

bool CMainWindow::OnCloseClicked(void *param) {
    ::PostQuitMessage(0);
    
    return true;
}
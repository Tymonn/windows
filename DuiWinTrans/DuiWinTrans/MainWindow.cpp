#include "stdafx.h"
#include "MainWindow.h"

CMainWindow::CMainWindow()
{
}


CMainWindow::~CMainWindow() {
}

LPCTSTR CMainWindow::GetWindowClassName() const {
    return L"DuiWinTrans";
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
        case WM_PAINT: {
            int m = 0;            
        }
            break; 
        case WM_LBUTTONDOWN: {
            result = OnLButtonDown(uMsg, wParam, lParam, handled);
        }
            break;
        case WM_LBUTTONUP: {
            result = OnLButtonUp(uMsg, wParam, lParam, handled);
        }
            break;
        case WM_MOVE: {
            result = OnMove(uMsg, wParam, lParam, handled);
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

        RECT top_rect = { rect.left + 2, rect.top, rect.right - 2, rect.top + 2 };
        RECT bottom_rect = { rect.left + 2, rect.bottom - 2, rect.right - 2, rect.bottom };
        RECT left_rect = { rect.left, rect.top + 2, rect.left + 2, rect.bottom - 2 };
        RECT right_rect = { rect.right - 2, rect.top + 2, rect.right, rect.bottom - 2 };

        rect.bottom = rect.top + height;
        if (::PtInRect(&top_rect, pt)) {
            return HTTOP;
        }
        else if (::PtInRect(&bottom_rect, pt)) {
            return HTBOTTOM;
        }
        else if (::PtInRect(&left_rect, pt)) {
            return HTLEFT;
        }
        else if (::PtInRect(&right_rect, pt)) {
            return HTRIGHT;
        }
        else if (::PtInRect(&rect, pt)) {
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

LRESULT CMainWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = FALSE;

    return 0;
}

LRESULT CMainWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = FALSE;
    return 0;
}

LRESULT CMainWindow::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = TRUE;
    return 0;
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
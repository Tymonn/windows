#include "stdafx.h"
#include "MainWindow.h"
#include <sstream>

CMainWindow::CMainWindow()
{
}


CMainWindow::~CMainWindow() {
}

void CMainWindow::Notify(TNotifyUI& msg) {
    if (!msg.pSender) {
        return;
    }

    if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
        if (msg.pSender == opacity_) {
            opacity_value_ = opacity_->GetValue();            
            SetWindowOpacity(wnd_under_cursor_, opacity_value_);

            std::wstringstream stream;
            stream << opacity_value_;
            opacity_->SetToolTip(stream.str().c_str());
        }
    }
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
            break;
        }            
        case WM_NCHITTEST: {
            result = OnNcHitTest(uMsg, wParam, lParam, handled);
            break;
        }
        case WM_PAINT: {
            int m = 0;            
            break;
        } 
        case WM_LBUTTONDOWN: {
            result = OnLButtonDown(uMsg, wParam, lParam, handled);
            break;
        }
        case WM_LBUTTONUP: {
            result = OnLButtonUp(uMsg, wParam, lParam, handled);
            break;
        }
        case WM_MOUSEMOVE: {
            result = OnMouseMove(uMsg, wParam, lParam, handled);
            break;
        }
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
    pntm_.AddNotifier(this);
    CDialogBuilder builder;
    CControlUI *root = builder.Create(L"Main.xml", 0, NULL, &pntm_, NULL);
    if (root) {
        pntm_.AttachDialog(root);
        FindSubCtrls();

        cursor_ = ::LoadCursorFromFile(L"skin\\mouse.cur");
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

    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    POINT pt = { xPos, yPos };
    CControlUI* ctrl = pntm_.FindControl(pt);
    if (ctrl) {
        std::wstring class_name = ctrl->GetClass();
        if (class_name != DUI_CTR_SLIDER && class_name != DUI_CTR_CHECKBOX) {
            //The system sends the mouse message to the window that contains the hot spot 
            //or to the window that is capturing mouse input
            ::SetCapture(m_hWnd);
            if (cursor_) {
                ::SetCursor(cursor_);
            }            
            tracking_ = true;
        }
    }
    
    return 0;
}

LRESULT CMainWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = FALSE;
    ::ReleaseCapture();
    tracking_ = false;

    if (wnd_under_cursor_ != m_hWnd) {
        //set opacity
        SetWindowOpacity(wnd_under_cursor_, opacity_value_);
        opacity_set_wnds_.push_back(wnd_under_cursor_);
    }    

    return 0;
}

LRESULT CMainWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = FALSE;
    if (tracking_) {
        //Get the information of the window under the cursor
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        POINT pt = { xPos, yPos };
        //pt is relative to the upper-left corner of the client area.
        //so it should be revised.
        RECT rect = { 0 };
        ::GetWindowRect(m_hWnd, &rect);
        pt.x += rect.left;
        pt.y += rect.top;
        HWND wnd = ::WindowFromPoint(pt);
        if (wnd == wnd_under_cursor_) {
            return 0; 
        }

        wnd_under_cursor_ = wnd;
        //handle
        std::wstringstream stream;
        stream << std::hex << L"0x" << wnd_under_cursor_;
        if (wnd_handle_) {
            wnd_handle_->SetText(stream.str().c_str());
        }

        stream.str(L"");
        WCHAR buffer[256] = { 0 };
        ::GetWindowText(wnd_under_cursor_,buffer,sizeof(buffer));
        stream << buffer;
        if (wnd_caption_) {
            wnd_caption_->SetText(stream.str().c_str());
        }
    }

    return 0;
}

void CMainWindow::FindSubCtrls() {
    close_ = static_cast<CButtonUI*>(pntm_.FindControl(L"close"));
    if (close_) {
        close_->Subscribe(CEventSets::EventClick, MakeDelegate(this, &CMainWindow::OnCloseClicked));
    }

    caption_ = static_cast<CHorizontalLayoutUI*>(pntm_.FindControl(L"caption"));
    reset_ = static_cast<CCheckBoxUI*>(pntm_.FindControl(L"reset"));
    if (reset_) {
        reset_->SetCheck(true);
        reset_->Subscribe(CEventSets::EventClick, MakeDelegate(this, &CMainWindow::OnResetClicked));
    }
    
    wnd_caption_ = static_cast<CLabelUI*>(pntm_.FindControl(L"wnd_caption"));
    wnd_handle_ = static_cast<CLabelUI*>(pntm_.FindControl(L"wnd_handle"));
    opacity_ = static_cast<CSliderUI*>(pntm_.FindControl(L"opacity"));
    if (opacity_) {
        opacity_->SetValue(opacity_value_);
    }
}

bool CMainWindow::OnCloseClicked(void *param) {
    if (reset_->GetCheck()) {
        for (auto wnd : opacity_set_wnds_) {
            CancelWindowOpacity(wnd);
        }
    }    

    ::PostQuitMessage(0);
    
    return true;
}

bool CMainWindow::OnResetClicked(void *param) {
    //if (!reset_) {
    //    return false;
    //}

    //bool check = reset_->GetCheck();

    return true;
}

void CMainWindow::SetWindowOpacity(HWND wnd, BYTE opacity) {
    ::SetWindowLong(wnd, GWL_EXSTYLE, ::GetWindowLong(wnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    ::SetLayeredWindowAttributes(wnd, 0, opacity, LWA_ALPHA);
    ::RedrawWindow(wnd,NULL,NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}

void CMainWindow::CancelWindowOpacity(HWND wnd) {
    ::SetWindowLong(wnd, GWL_EXSTYLE, ::GetWindowLong(wnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
    ::RedrawWindow(wnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}
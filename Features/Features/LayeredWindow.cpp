#include "stdafx.h"
#include "LayeredWindow.h"
#include <sstream>


CLayeredWindow::CLayeredWindow()
{
}


CLayeredWindow::~CLayeredWindow()
{
}

LPCTSTR CLayeredWindow::GetWindowClassName() const {
    return L"TypeApp SubWindow";
}

void CLayeredWindow::SetTitle(const std::wstring& title) {
    if (title_) {
        title_->SetText(title.c_str());
    }
}

LRESULT CLayeredWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

LRESULT CLayeredWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled) {
    handled = TRUE;
    pntm_.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI *root = builder.Create(L"Layered.xml", 0, NULL, &pntm_, NULL);
    if (root) {
        pntm_.AttachDialog(root);
        FindSubCtrls();
    }

    //SetLayeredWindowAttributes有两种方式设置窗口透明度，由dwFlags决定哪种方式
    //1、bAlpha describes the opacity (not transparency) of the layered window with a value between 0 and 255
    //2、crKey describes the transparency color(All pixels painted by the window in this color will be transparent).
    //dwFlags specify the action to be taken: use bAlpha or colorkey to achieve transparency effect
    //i.e. Use bAlpha to determine the opacity of the layered window
    //or, Use crKey as the transparency color
    SetLayeredWindowAttributes(m_hWnd, RGB(255, 0, 0), 100, LWA_ALPHA);

    return 0;
}


LRESULT CLayeredWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = TRUE;
    if (caption_) {
        short x = GET_X_LPARAM(lParam);
        short y = GET_Y_LPARAM(lParam);
        POINT pt = { x,y };

        int height = caption_->GetFixedHeight();
        RECT rect = { 0 };
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

void CLayeredWindow::FindSubCtrls() {
    close_ = static_cast<CButtonUI*>(pntm_.FindControl(L"close"));
    if (close_) {
        close_->Subscribe(CEventSets::EventClick, MakeDelegate(this, &CLayeredWindow::OnCloseClicked));
    }

    caption_ = static_cast<CHorizontalLayoutUI*>(pntm_.FindControl(L"caption"));
    title_ = static_cast<CLabelUI*>(pntm_.FindControl(L"title"));
}

bool CLayeredWindow::OnCloseClicked(void *param) {
    Close();

    return true;
}
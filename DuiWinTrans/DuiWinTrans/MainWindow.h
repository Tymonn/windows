#pragma once
#include "../../Dui/UIlib.h"
using namespace DuiLib;

class CMainWindow : public CWindowWnd, public INotifyUI
{
public:
    CMainWindow();
    ~CMainWindow();

    virtual void Notify(TNotifyUI& msg) override;

protected:
    LPCTSTR GetWindowClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    void FindSubCtrls();
    bool OnCloseClicked(void *param);
    bool OnResetClicked(void *param);
    void SetWindowOpacity(HWND wnd, BYTE opacity);
    void CancelWindowOpacity(HWND wnd);

    HCURSOR cursor_ = NULL;
    bool tracking_ = false;
    short opacity_value_ = 100;
    HWND wnd_under_cursor_ = NULL;
    std::vector<HWND> opacity_set_wnds_;
    CPaintManagerUI pntm_;
    CButtonUI *close_ = nullptr;
    CCheckBoxUI *reset_ = nullptr;
    CLabelUI *wnd_caption_ = nullptr;
    CLabelUI *wnd_handle_ = nullptr;
    CSliderUI *opacity_ = nullptr;
    CHorizontalLayoutUI *caption_ = nullptr;
};


#pragma once
#include "../../Dui/UIlib.h"
using namespace DuiLib;

class CMainWindow : public CWindowWnd
{
public:
    CMainWindow();
    ~CMainWindow();

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);

protected:
    LPCTSTR GetWindowClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    void FindSubCtrls();
    bool OnCloseClicked(void *param);

    CPaintManagerUI pntm_;
    CButtonUI *close_ = nullptr;
};


#pragma once
#include "../../Dui/UIlib.h"
#include "menu/MenuUI.h"
#include "SubWindow.h"
using namespace DuiLib;

class CMainWindow : public CWindowWnd
{
public:
    CMainWindow();
    ~CMainWindow();

protected:
    LPCTSTR GetWindowClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    void FindSubCtrls();
    bool OnCloseClicked(void *param);

    CMenuUI menu_;
    CPaintManagerUI pntm_;
    CButtonUI *close_ = nullptr;
    CHorizontalLayoutUI *caption_ = nullptr;
    std::vector<CSubWindow*> subs_;
};


// DrawingBoard.h
#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"

class CDrawingBoardApp : public CWinApp {
public:
    CDrawingBoardApp() noexcept;

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CDrawingBoardApp theApp;

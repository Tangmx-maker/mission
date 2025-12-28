// DrawingBoard.cpp
#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DrawingBoard.h"
#include "MainFrm.h"
#include "DrawingBoardDoc.h"
#include "DrawingBoardView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDrawingBoardApp, CWinApp)
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

CDrawingBoardApp::CDrawingBoardApp() noexcept {
    SetAppID(_T("DrawingBoard.AppID.NoVersion"));
}

CDrawingBoardApp theApp;

BOOL CDrawingBoardApp::InitInstance() {
    CWinApp::InitInstance();

    if (!AfxOleInit()) {
        AfxMessageBox(_T("OLE 初始化失败"));
        return FALSE;
    }

    AfxEnableControlContainer();

    SetRegistryKey(_T("DrawingBoard"));
    LoadStdProfileSettings(4);

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CDrawingBoardDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CDrawingBoardView));

    if (!pDocTemplate)
        return FALSE;

    AddDocTemplate(pDocTemplate);

    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}

int CDrawingBoardApp::ExitInstance() {
    AfxOleTerm(FALSE);
    return CWinApp::ExitInstance();
}

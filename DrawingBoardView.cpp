#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "DrawingBoard.h"
#endif
#include "DrawingBoardDoc.h"
#include "DrawingBoardView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDrawingBoardView, CView)

BEGIN_MESSAGE_MAP(CDrawingBoardView, CView)
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

    ON_COMMAND(ID_MY_UNDO, &CDrawingBoardView::OnMyUndo)
    ON_COMMAND(ID_MY_REDO, &CDrawingBoardView::OnMyRedo)
    ON_UPDATE_COMMAND_UI(ID_MY_UNDO, &CDrawingBoardView::OnUpdateMyUndo)
    ON_UPDATE_COMMAND_UI(ID_MY_REDO, &CDrawingBoardView::OnUpdateMyRedo)

    ON_COMMAND(ID_EDIT_CLEAR_ALL, &CDrawingBoardView::OnEditClearAll)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CDrawingBoardView::OnUpdateEditClearAll)

    ON_COMMAND(ID_TOOL_PEN, &CDrawingBoardView::OnToolPen)
    ON_COMMAND(ID_TOOL_LINE, &CDrawingBoardView::OnToolLine)
    ON_COMMAND(ID_TOOL_RECTANGLE, &CDrawingBoardView::OnToolRectangle)
    ON_COMMAND(ID_TOOL_ELLIPSE, &CDrawingBoardView::OnToolEllipse)
    ON_COMMAND(ID_TOOL_ERASER, &CDrawingBoardView::OnToolEraser)

    ON_UPDATE_COMMAND_UI(ID_TOOL_PEN, &CDrawingBoardView::OnUpdateToolPen)
    ON_UPDATE_COMMAND_UI(ID_TOOL_LINE, &CDrawingBoardView::OnUpdateToolLine)
    ON_UPDATE_COMMAND_UI(ID_TOOL_RECTANGLE, &CDrawingBoardView::OnUpdateToolRectangle)
    ON_UPDATE_COMMAND_UI(ID_TOOL_ELLIPSE, &CDrawingBoardView::OnUpdateToolEllipse)
    ON_UPDATE_COMMAND_UI(ID_TOOL_ERASER, &CDrawingBoardView::OnUpdateToolEraser)

    ON_COMMAND(ID_COLOR_BLACK, &CDrawingBoardView::OnColorBlack)
    ON_COMMAND(ID_COLOR_RED, &CDrawingBoardView::OnColorRed)
    ON_COMMAND(ID_COLOR_GREEN, &CDrawingBoardView::OnColorGreen)
    ON_COMMAND(ID_COLOR_BLUE, &CDrawingBoardView::OnColorBlue)
    ON_COMMAND(ID_COLOR_YELLOW, &CDrawingBoardView::OnColorYellow)

    ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CDrawingBoardView::OnUpdateColorBlack)
    ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CDrawingBoardView::OnUpdateColorRed)
    ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CDrawingBoardView::OnUpdateColorGreen)
    ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CDrawingBoardView::OnUpdateColorBlue)
    ON_UPDATE_COMMAND_UI(ID_COLOR_YELLOW, &CDrawingBoardView::OnUpdateColorYellow)

    ON_COMMAND(ID_SIZE_THIN, &CDrawingBoardView::OnSizeThin)
    ON_COMMAND(ID_SIZE_MEDIUM, &CDrawingBoardView::OnSizeMedium)
    ON_COMMAND(ID_SIZE_THICK, &CDrawingBoardView::OnSizeThick)

    ON_UPDATE_COMMAND_UI(ID_SIZE_THIN, &CDrawingBoardView::OnUpdateSizeThin)
    ON_UPDATE_COMMAND_UI(ID_SIZE_MEDIUM, &CDrawingBoardView::OnUpdateSizeMedium)
    ON_UPDATE_COMMAND_UI(ID_SIZE_THICK, &CDrawingBoardView::OnUpdateSizeThick)

    ON_COMMAND(ID_FILE_EXPORT_PNG, &CDrawingBoardView::OnFileExportPng)

    ON_COMMAND(ID_LANGUAGE_CHINESE, &CDrawingBoardView::OnLanguageChinese)
    ON_COMMAND(ID_LANGUAGE_ENGLISH, &CDrawingBoardView::OnLanguageEnglish)
    ON_COMMAND(ID_LANGUAGE_KOREAN, &CDrawingBoardView::OnLanguageKorean)

    ON_UPDATE_COMMAND_UI(ID_LANGUAGE_CHINESE, &CDrawingBoardView::OnUpdateLanguageChinese)
    ON_UPDATE_COMMAND_UI(ID_LANGUAGE_ENGLISH, &CDrawingBoardView::OnUpdateLanguageEnglish)
    ON_UPDATE_COMMAND_UI(ID_LANGUAGE_KOREAN, &CDrawingBoardView::OnUpdateLanguageKorean)

    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CDrawingBoardView::CDrawingBoardView() noexcept
    : m_currentTool(ToolType::PEN)
    , m_currentColor(RGB(0, 0, 0))
    , m_currentPenWidth(2)
    , m_bDrawing(FALSE)
    , m_bBufferInitialized(FALSE)
    , m_gdiplusToken(0)
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CDrawingBoardView::~CDrawingBoardView()
{
    if (m_bufferBitmap.GetSafeHandle())
    {
        m_bufferDC.SelectObject((CBitmap*)NULL);
        m_bufferBitmap.DeleteObject();
        m_bufferDC.DeleteDC();
    }

    if (m_gdiplusToken)
    {
        GdiplusShutdown(m_gdiplusToken);
    }
}

BOOL CDrawingBoardView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

void CDrawingBoardView::OnDraw(CDC* pDC)
{
    CDrawingBoardDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    OutputDebugString(_T("OnDraw 开始\n"));

    if (!m_bBufferInitialized)
    {
        OutputDebugString(_T("OnDraw: 缓冲区未初始化\n"));
        return;
    }

    if (m_bufferDC.GetSafeHdc() == NULL)
    {
        OutputDebugString(_T("OnDraw: 缓冲区 DC 无效\n"));
        return;
    }

    CRect rect;
    GetClientRect(&rect);

    CString msg;
    msg.Format(_T("OnDraw: BitBlt (%d x %d)\n"), rect.Width(), rect.Height());
    OutputDebugString(msg);

    BOOL result = pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
        &m_bufferDC, 0, 0, SRCCOPY);

    if (!result)
    {
        OutputDebugString(_T("OnDraw: BitBlt 失败！\n"));
    }
    else
    {
        OutputDebugString(_T("OnDraw: BitBlt 成功\n"));
    }
}



BOOL CDrawingBoardView::OnPreparePrinting(CPrintInfo* pInfo)
{
    return DoPreparePrinting(pInfo);
}

void CDrawingBoardView::OnBeginPrinting(CDC*, CPrintInfo*)
{
}

void CDrawingBoardView::OnEndPrinting(CDC*, CPrintInfo*)
{
}

#ifdef _DEBUG
void CDrawingBoardView::AssertValid() const
{
    CView::AssertValid();
}

void CDrawingBoardView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CDrawingBoardDoc* CDrawingBoardView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawingBoardDoc)));
    return (CDrawingBoardDoc*)m_pDocument;
}
#endif

void CDrawingBoardView::InitializeBuffer(int width, int height)
{
    // 清理旧缓冲区
    if (m_bufferBitmap.GetSafeHandle())
    {
        m_bufferDC.SelectObject((CBitmap*)NULL);
        m_bufferBitmap.DeleteObject();
        m_bufferDC.DeleteDC();
    }

    // 创建新缓冲区
    CClientDC dc(this);
    m_bufferDC.CreateCompatibleDC(&dc);
    m_bufferBitmap.CreateCompatibleBitmap(&dc, width, height);
    m_bufferDC.SelectObject(&m_bufferBitmap);

    // 填充白色背景
    m_bufferDC.FillSolidRect(0, 0, width, height, RGB(255, 255, 255));

    m_bBufferInitialized = TRUE;

    // 重绘所有命令
    RedrawAllCommands();
}


void CDrawingBoardView::RedrawAllCommands()
{
    // 🔍 诊断 1：检查缓冲区
    if (!m_bBufferInitialized)
    {
        AfxMessageBox(_T("RedrawAllCommands: 缓冲区未初始化！"));
        return;
    }

    if (m_bufferDC.GetSafeHdc() == NULL)
    {
        AfxMessageBox(_T("RedrawAllCommands: 缓冲区 DC 无效！"));
        return;
    }

    // 1. 清空缓冲区
    CRect rect;
    GetClientRect(&rect);

    CString msg;
    msg.Format(_T("RedrawAllCommands: 清空缓冲区 (%d x %d), 命令数=%d"),
        rect.Width(), rect.Height(), m_drawCommands.size());
    OutputDebugString(msg);

    m_bufferDC.FillSolidRect(&rect, RGB(255, 255, 255));

    // 2. 重绘所有命令
    for (size_t i = 0; i < m_drawCommands.size(); i++)
    {
        msg.Format(_T("  [%d] 工具=%d, 起点(%d,%d), 终点(%d,%d)"),
            i,
            (int)m_drawCommands[i].tool,
            m_drawCommands[i].startPoint.x, m_drawCommands[i].startPoint.y,
            m_drawCommands[i].endPoint.x, m_drawCommands[i].endPoint.y);
        OutputDebugString(msg);

        ExecuteCommand(m_drawCommands[i], &m_bufferDC);
    }

    OutputDebugString(_T("RedrawAllCommands: 完成\n"));
}





void CDrawingBoardView::ExecuteCommand(const DrawCommand& cmd, CDC* pDC)
{
    // 🔍 调试：检查 pDC 是否有效
    if (pDC == NULL || pDC->GetSafeHdc() == NULL)
    {
        AfxMessageBox(_T("ExecuteCommand: DC 无效！"));
        return;
    }

    // 创建画笔
    CPen pen;
    if (cmd.tool == ToolType::ERASER)
    {
        pen.CreatePen(PS_SOLID, cmd.penWidth, RGB(255, 255, 255));
    }
    else
    {
        pen.CreatePen(PS_SOLID, cmd.penWidth, cmd.color);
    }

    CPen* pOldPen = pDC->SelectObject(&pen);

    // 🔑 关键：设置画刷为空心
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    switch (cmd.tool)
    {
    case ToolType::PEN:
        if (cmd.points.size() >= 2)
        {
            pDC->MoveTo(cmd.points[0]);
            for (size_t i = 1; i < cmd.points.size(); i++)
            {
                pDC->LineTo(cmd.points[i]);
            }
        }
        break;

    case ToolType::LINE:
        pDC->MoveTo(cmd.startPoint);
        pDC->LineTo(cmd.endPoint);
        break;

    case ToolType::RECTANGLE:
    {
        // 🔍 调试：显示矩形坐标
        CString msg;
        msg.Format(_T("绘制矩形：(%d,%d) -> (%d,%d)"),
            cmd.startPoint.x, cmd.startPoint.y,
            cmd.endPoint.x, cmd.endPoint.y);
        OutputDebugString(msg);

        pDC->Rectangle(cmd.startPoint.x, cmd.startPoint.y,
            cmd.endPoint.x, cmd.endPoint.y);
    }
    break;

    case ToolType::ELLIPSE:
    {
        // 🔍 调试：显示圆形坐标
        CString msg;
        msg.Format(_T("绘制圆形：(%d,%d) -> (%d,%d)"),
            cmd.startPoint.x, cmd.startPoint.y,
            cmd.endPoint.x, cmd.endPoint.y);
        OutputDebugString(msg);

        // 🔑 确保坐标顺序正确
        int left = min(cmd.startPoint.x, cmd.endPoint.x);
        int top = min(cmd.startPoint.y, cmd.endPoint.y);
        int right = max(cmd.startPoint.x, cmd.endPoint.x);
        int bottom = max(cmd.startPoint.y, cmd.endPoint.y);

        pDC->Ellipse(left, top, right, bottom);
    }
    break;

    case ToolType::ERASER:
        if (cmd.points.size() >= 2)
        {
            pDC->MoveTo(cmd.points[0]);
            for (size_t i = 1; i < cmd.points.size(); i++)
            {
                pDC->LineTo(cmd.points[i]);
            }
        }
        break;
    }

    // 恢复原画笔和画刷
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}




void CDrawingBoardView::DrawToBuffer(const DrawCommand& cmd)
{
    if (m_bBufferInitialized)
    {
        ExecuteCommand(cmd, &m_bufferDC);
    }
}

void CDrawingBoardView::UpdateAllMenus()
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    if (!pMainFrame) return;

    CMenu* pMenu = pMainFrame->GetMenu();
    if (!pMenu) return;

    struct TopMenuInfo {
        int index;
        CString text;
    };

    TopMenuInfo topMenus[] = {
        {0, m_langManager.GetTopLevelMenuText(0)},
        {1, m_langManager.GetTopLevelMenuText(1)},
        {2, m_langManager.GetTopLevelMenuText(2)},
        {3, m_langManager.GetTopLevelMenuText(3)},
        {4, m_langManager.GetTopLevelMenuText(4)},
        {5, m_langManager.GetTopLevelMenuText(5)},
        {6, m_langManager.GetTopLevelMenuText(6)},
        {7, m_langManager.GetTopLevelMenuText(7)}
    };

    for (const auto& menu : topMenus)
    {
        if (!menu.text.IsEmpty())
        {
            MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
            mii.fMask = MIIM_STRING;
            mii.dwTypeData = (LPTSTR)(LPCTSTR)menu.text;
            pMenu->SetMenuItemInfo(menu.index, &mii, TRUE);
        }
    }

    UINT fileMenuIDs[] = {
        ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_SAVE, ID_FILE_SAVE_AS,
        ID_FILE_EXPORT_PNG, ID_APP_EXIT
    };

    UINT editMenuIDs[] = {
        ID_MY_UNDO, ID_MY_REDO, ID_EDIT_CLEAR_ALL
    };

    UINT toolMenuIDs[] = {
        ID_TOOL_PEN, ID_TOOL_LINE, ID_TOOL_RECTANGLE,
        ID_TOOL_ELLIPSE, ID_TOOL_ERASER
    };

    UINT colorMenuIDs[] = {
        ID_COLOR_BLACK, ID_COLOR_RED, ID_COLOR_GREEN,
        ID_COLOR_BLUE, ID_COLOR_YELLOW
    };

    UINT sizeMenuIDs[] = {
        ID_SIZE_THIN, ID_SIZE_MEDIUM, ID_SIZE_THICK
    };

    UINT languageMenuIDs[] = {
        ID_LANGUAGE_CHINESE, ID_LANGUAGE_ENGLISH, ID_LANGUAGE_KOREAN
    };

    UINT viewMenuIDs[] = {
        ID_VIEW_TOOLBAR, ID_VIEW_STATUS_BAR
    };

    UINT helpMenuIDs[] = {
        ID_APP_ABOUT
    };

    auto updateMenuItems = [this, pMenu](UINT* ids, int count) {
        for (int i = 0; i < count; i++)
        {
            CString text = m_langManager.GetMenuText(ids[i]);
            if (!text.IsEmpty())
            {
                MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
                mii.fMask = MIIM_STRING;
                mii.dwTypeData = (LPTSTR)(LPCTSTR)text;
                pMenu->SetMenuItemInfo(ids[i], &mii, FALSE);
            }
        }
        };

    updateMenuItems(fileMenuIDs, _countof(fileMenuIDs));
    updateMenuItems(editMenuIDs, _countof(editMenuIDs));
    updateMenuItems(toolMenuIDs, _countof(toolMenuIDs));
    updateMenuItems(colorMenuIDs, _countof(colorMenuIDs));
    updateMenuItems(sizeMenuIDs, _countof(sizeMenuIDs));
    updateMenuItems(languageMenuIDs, _countof(languageMenuIDs));
    updateMenuItems(viewMenuIDs, _countof(viewMenuIDs));
    updateMenuItems(helpMenuIDs, _countof(helpMenuIDs));

    pMainFrame->DrawMenuBar();
}

int CDrawingBoardView::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;
    UINT size = 0;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }

    free(pImageCodecInfo);
    return -1;
}

void CDrawingBoardView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrawing = TRUE;
    m_startPoint = point;
    m_endPoint = point;
    m_points.clear();
    m_points.push_back(point);

    SetCapture();
    CView::OnLButtonDown(nFlags, point);
}

void CDrawingBoardView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        m_bDrawing = FALSE;
        m_endPoint = point;

        // 创建命令
        DrawCommand cmd;
        cmd.tool = m_currentTool;
        cmd.color = m_currentColor;
        cmd.penWidth = m_currentPenWidth;
        cmd.startPoint = m_startPoint;
        cmd.endPoint = m_endPoint;

        // 深拷贝 points
        cmd.points.clear();
        for (size_t i = 0; i < m_points.size(); i++)
        {
            cmd.points.push_back(m_points[i]);
        }

        // 使用深拷贝添加到命令历史
        m_drawCommands.push_back(DeepCopyCommand(cmd));

        // 清空重做栈
        m_redoCommands.clear();

        // 重绘所有命令
        RedrawAllCommands();

        // 强制刷新
        Invalidate(TRUE);
        UpdateWindow();

        ReleaseCapture();
    }

    CView::OnLButtonUp(nFlags, point);
}



void CDrawingBoardView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        m_endPoint = point;

        if (m_currentTool == ToolType::PEN || m_currentTool == ToolType::ERASER)
        {
            // 添加点到轨迹
            m_points.push_back(point);

            if (m_bBufferInitialized && m_points.size() >= 2)
            {
                // 创建画笔
                CPen pen;
                if (m_currentTool == ToolType::ERASER)
                {
                    pen.CreatePen(PS_SOLID, m_currentPenWidth, RGB(255, 255, 255));
                }
                else
                {
                    pen.CreatePen(PS_SOLID, m_currentPenWidth, m_currentColor);
                }

                // 绘制到缓冲区
                CPen* pOldPen = m_bufferDC.SelectObject(&pen);
                m_bufferDC.MoveTo(m_points[m_points.size() - 2]);
                m_bufferDC.LineTo(m_points[m_points.size() - 1]);
                m_bufferDC.SelectObject(pOldPen);

                // 复制到屏幕
                CClientDC dc(this);
                CRect rect;
                GetClientRect(&rect);
                dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_bufferDC, 0, 0, SRCCOPY);
            }
        }
        else
        {
            // 形状工具：实时预览
            if (m_bBufferInitialized)
            {
                CClientDC dc(this);

                // 先复制缓冲区到屏幕
                CRect rect;
                GetClientRect(&rect);
                dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_bufferDC, 0, 0, SRCCOPY);

                // 在屏幕上绘制预览
                CPen pen;
                pen.CreatePen(PS_SOLID, m_currentPenWidth, m_currentColor);
                CPen* pOldPen = dc.SelectObject(&pen);
                dc.SelectStockObject(NULL_BRUSH);

                switch (m_currentTool)
                {
                case ToolType::LINE:
                    dc.MoveTo(m_startPoint);
                    dc.LineTo(m_endPoint);
                    break;
                case ToolType::RECTANGLE:
                    dc.Rectangle(m_startPoint.x, m_startPoint.y,
                        m_endPoint.x, m_endPoint.y);
                    break;
                case ToolType::ELLIPSE:
                    dc.Ellipse(m_startPoint.x, m_startPoint.y,
                        m_endPoint.x, m_endPoint.y);
                    break;
                }

                dc.SelectObject(pOldPen);
            }
        }
    }

    CView::OnMouseMove(nFlags, point);
}



void CDrawingBoardView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if (cx > 0 && cy > 0)
    {
        InitializeBuffer(cx, cy);
        Invalidate(TRUE);
        UpdateWindow();
    }
}


BOOL CDrawingBoardView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CDrawingBoardView::OnToolPen()
{
    m_currentTool = ToolType::PEN;
}

void CDrawingBoardView::OnToolLine()
{
    m_currentTool = ToolType::LINE;
}

void CDrawingBoardView::OnToolRectangle()
{
    m_currentTool = ToolType::RECTANGLE;
}

void CDrawingBoardView::OnToolEllipse()
{
    m_currentTool = ToolType::ELLIPSE;
}

void CDrawingBoardView::OnToolEraser()
{
    m_currentTool = ToolType::ERASER;
}

void CDrawingBoardView::OnUpdateToolPen(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentTool == ToolType::PEN);
}

void CDrawingBoardView::OnUpdateToolLine(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentTool == ToolType::LINE);
}

void CDrawingBoardView::OnUpdateToolRectangle(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentTool == ToolType::RECTANGLE);
}

void CDrawingBoardView::OnUpdateToolEllipse(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentTool == ToolType::ELLIPSE);
}

void CDrawingBoardView::OnUpdateToolEraser(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentTool == ToolType::ERASER);
}

void CDrawingBoardView::OnColorBlack()
{
    m_currentColor = RGB(0, 0, 0);
}

void CDrawingBoardView::OnColorRed()
{
    m_currentColor = RGB(255, 0, 0);
}

void CDrawingBoardView::OnColorGreen()
{
    m_currentColor = RGB(0, 255, 0);
}

void CDrawingBoardView::OnColorBlue()
{
    m_currentColor = RGB(0, 0, 255);
}

void CDrawingBoardView::OnColorYellow()
{
    m_currentColor = RGB(255, 255, 0);
}

void CDrawingBoardView::OnUpdateColorBlack(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentColor == RGB(0, 0, 0));
}

void CDrawingBoardView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentColor == RGB(255, 0, 0));
}

void CDrawingBoardView::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentColor == RGB(0, 255, 0));
}

void CDrawingBoardView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentColor == RGB(0, 0, 255));
}

void CDrawingBoardView::OnUpdateColorYellow(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentColor == RGB(255, 255, 0));
}

void CDrawingBoardView::OnSizeThin()
{
    m_currentPenWidth = 1;
}

void CDrawingBoardView::OnSizeMedium()
{
    m_currentPenWidth = 3;
}

void CDrawingBoardView::OnSizeThick()
{
    m_currentPenWidth = 5;
}

void CDrawingBoardView::OnUpdateSizeThin(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentPenWidth == 1);
}

void CDrawingBoardView::OnUpdateSizeMedium(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentPenWidth == 3);
}

void CDrawingBoardView::OnUpdateSizeThick(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_currentPenWidth == 5);
}

void CDrawingBoardView::OnMyUndo()
{
    if (m_drawCommands.empty())
    {
        AfxMessageBox(_T("没有可撤销的操作"), MB_ICONINFORMATION);
        return;
    }

    // 深拷贝最后一个命令
    DrawCommand lastCmd = DeepCopyCommand(m_drawCommands.back());

    // 从命令历史移除
    m_drawCommands.pop_back();

    // 深拷贝到重做栈
    m_redoCommands.push_back(DeepCopyCommand(lastCmd));

    // 重绘所有命令
    RedrawAllCommands();

    // 强制刷新
    Invalidate(TRUE);
    UpdateWindow();
}




void CDrawingBoardView::OnMyRedo()
{
    if (m_redoCommands.empty())
    {
        AfxMessageBox(_T("没有可重做的操作"), MB_ICONINFORMATION);
        return;
    }

    DrawCommand redoCmd = DeepCopyCommand(m_redoCommands.back());
    m_redoCommands.pop_back();
    m_drawCommands.push_back(DeepCopyCommand(redoCmd));

    RedrawAllCommands();

    // 🔥 暴力刷新
    CClientDC dc(this);
    OnDraw(&dc);  // 直接调用绘制
}





DrawCommand CDrawingBoardView::DeepCopyCommand(const DrawCommand& src)
{
    DrawCommand dst;

    dst.tool = src.tool;
    dst.color = src.color;
    dst.penWidth = src.penWidth;

    dst.startPoint.x = src.startPoint.x;
    dst.startPoint.y = src.startPoint.y;

    dst.endPoint.x = src.endPoint.x;
    dst.endPoint.y = src.endPoint.y;

    // 深拷贝 points
    dst.points.clear();
    dst.points.reserve(src.points.size());
    for (size_t i = 0; i < src.points.size(); i++)
    {
        CPoint pt(src.points[i].x, src.points[i].y);
        dst.points.push_back(pt);
    }

    return dst;
}



void CDrawingBoardView::OnUpdateMyUndo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!m_drawCommands.empty());
}

void CDrawingBoardView::OnUpdateMyRedo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!m_redoCommands.empty());
}

void CDrawingBoardView::OnEditClearAll()
{
    if (m_drawCommands.empty())
    {
        CString msg = m_langManager.GetString(_T("MSG_BOARD_EMPTY"));
        AfxMessageBox(msg, MB_ICONINFORMATION);
        return;
    }

    CString msg = m_langManager.GetString(_T("MSG_CLEAR_CONFIRM"));
    if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        m_drawCommands.clear();
        m_redoCommands.clear();

        if (m_bBufferInitialized)
        {
            CRect rect;
            GetClientRect(&rect);
            m_bufferDC.FillSolidRect(&rect, RGB(255, 255, 255));
        }

        Invalidate(FALSE);

        CString successMsg = m_langManager.GetString(_T("MSG_CLEAR_SUCCESS"));
        AfxMessageBox(successMsg, MB_ICONINFORMATION);
    }
}

void CDrawingBoardView::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!m_drawCommands.empty());
}

void CDrawingBoardView::OnFileExportPng()
{
    if (!m_bBufferInitialized)
    {
        CString msg = m_langManager.GetString(_T("MSG_BUFFER_NOT_INIT"));
        AfxMessageBox(msg, MB_ICONWARNING);
        return;
    }

    CString defaultName = m_langManager.GetString(_T("DEFAULT_FILENAME"));
    CString filter = m_langManager.GetString(_T("FILE_FILTER"));

    CFileDialog dlg(FALSE, _T("png"), defaultName,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        filter);

    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();

        HBITMAP hBitmap = (HBITMAP)m_bufferBitmap.GetSafeHandle();
        Bitmap* pBitmap = Bitmap::FromHBITMAP(hBitmap, NULL);

        CLSID pngClsid;
        if (GetEncoderClsid(L"image/png", &pngClsid) == -1)
        {
            CString msg = m_langManager.GetString(_T("MSG_NO_PNG_ENCODER"));
            AfxMessageBox(msg, MB_ICONERROR);
            delete pBitmap;
            return;
        }

        Status status = pBitmap->Save(filePath, &pngClsid, NULL);
        delete pBitmap;

        if (status == Ok)
        {
            CString msg;
            CString format = m_langManager.GetString(_T("MSG_EXPORT_SUCCESS"));
            msg.Format(format, filePath);
            AfxMessageBox(msg, MB_ICONINFORMATION);
        }
        else
        {
            CString msg = m_langManager.GetString(_T("MSG_EXPORT_FAILED"));
            AfxMessageBox(msg, MB_ICONERROR);
        }
    }
}

void CDrawingBoardView::OnLanguageChinese()
{
    m_langManager.SetLanguage(Language::CHINESE);
    UpdateAllMenus();

    CString msg = m_langManager.GetString(_T("MSG_LANGUAGE_CHANGED"));
    AfxMessageBox(msg, MB_ICONINFORMATION);
}

void CDrawingBoardView::OnLanguageEnglish()
{
    m_langManager.SetLanguage(Language::ENGLISH);
    UpdateAllMenus();

    CString msg = m_langManager.GetString(_T("MSG_LANGUAGE_CHANGED"));
    AfxMessageBox(msg, MB_ICONINFORMATION);
}

void CDrawingBoardView::OnLanguageKorean()
{
    m_langManager.SetLanguage(Language::KOREAN);
    UpdateAllMenus();

    CString msg = m_langManager.GetString(_T("MSG_LANGUAGE_CHANGED"));
    AfxMessageBox(msg, MB_ICONINFORMATION);
}

void CDrawingBoardView::OnUpdateLanguageChinese(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_langManager.GetLanguage() == Language::CHINESE);
}

void CDrawingBoardView::OnUpdateLanguageEnglish(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_langManager.GetLanguage() == Language::ENGLISH);
}

void CDrawingBoardView::OnUpdateLanguageKorean(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_langManager.GetLanguage() == Language::KOREAN);
}

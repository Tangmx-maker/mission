#pragma once
#include <vector>
#include <gdiplus.h>
#include "LanguageManager.h"

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

enum class ToolType
{
    PEN,
    LINE,
    RECTANGLE,
    ELLIPSE,
    ERASER
};

struct DrawCommand
{
    ToolType tool;
    COLORREF color;
    int penWidth;
    CPoint startPoint;
    CPoint endPoint;
    std::vector<CPoint> points;

    // 默认构造函数
    DrawCommand()
        : tool(ToolType::PEN)
        , color(RGB(0, 0, 0))
        , penWidth(1)
        , startPoint(0, 0)
        , endPoint(0, 0)
    {
    }

    // 拷贝构造函数
    DrawCommand(const DrawCommand& other)
        : tool(other.tool)
        , color(other.color)
        , penWidth(other.penWidth)
        , startPoint(other.startPoint)
        , endPoint(other.endPoint)
    {
        // 深拷贝 points
        points.clear();
        for (size_t i = 0; i < other.points.size(); i++)
        {
            points.push_back(other.points[i]);
        }
    }

    // 赋值操作符
    DrawCommand& operator=(const DrawCommand& other)
    {
        if (this != &other)
        {
            tool = other.tool;
            color = other.color;
            penWidth = other.penWidth;
            startPoint = other.startPoint;
            endPoint = other.endPoint;

            // 深拷贝 points
            points.clear();
            for (size_t i = 0; i < other.points.size(); i++)
            {
                points.push_back(other.points[i]);
            }
        }
        return *this;
    }
};



class CDrawingBoardView : public CView
{
protected:
    CDrawingBoardView() noexcept;
    DECLARE_DYNCREATE(CDrawingBoardView)

private:
    // 深拷贝辅助函数
    DrawCommand DeepCopyCommand(const DrawCommand& src);

public:
    CDrawingBoardDoc* GetDocument() const;

    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
    virtual ~CDrawingBoardView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    ToolType m_currentTool;
    COLORREF m_currentColor;
    int m_currentPenWidth;

    BOOL m_bDrawing;
    CPoint m_startPoint;
    CPoint m_endPoint;
    std::vector<CPoint> m_points;

    std::vector<DrawCommand> m_drawCommands;
    std::vector<DrawCommand> m_redoCommands;

    CDC m_bufferDC;
    CBitmap m_bufferBitmap;
    BOOL m_bBufferInitialized;

    LanguageManager m_langManager;

    ULONG_PTR m_gdiplusToken;

protected:
    void InitializeBuffer(int width, int height);
    void DrawToBuffer(const DrawCommand& cmd);
    void ExecuteCommand(const DrawCommand& cmd, CDC* pDC);
    void RedrawAllCommands();
    void UpdateAllMenus();
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg void OnToolPen();
    afx_msg void OnToolLine();
    afx_msg void OnToolRectangle();
    afx_msg void OnToolEllipse();
    afx_msg void OnToolEraser();

    afx_msg void OnUpdateToolPen(CCmdUI* pCmdUI);
    afx_msg void OnUpdateToolLine(CCmdUI* pCmdUI);
    afx_msg void OnUpdateToolRectangle(CCmdUI* pCmdUI);
    afx_msg void OnUpdateToolEllipse(CCmdUI* pCmdUI);
    afx_msg void OnUpdateToolEraser(CCmdUI* pCmdUI);

    afx_msg void OnColorBlack();
    afx_msg void OnColorRed();
    afx_msg void OnColorGreen();
    afx_msg void OnColorBlue();
    afx_msg void OnColorYellow();

    afx_msg void OnUpdateColorBlack(CCmdUI* pCmdUI);
    afx_msg void OnUpdateColorRed(CCmdUI* pCmdUI);
    afx_msg void OnUpdateColorGreen(CCmdUI* pCmdUI);
    afx_msg void OnUpdateColorBlue(CCmdUI* pCmdUI);
    afx_msg void OnUpdateColorYellow(CCmdUI* pCmdUI);

    afx_msg void OnSizeThin();
    afx_msg void OnSizeMedium();
    afx_msg void OnSizeThick();

    afx_msg void OnUpdateSizeThin(CCmdUI* pCmdUI);
    afx_msg void OnUpdateSizeMedium(CCmdUI* pCmdUI);
    afx_msg void OnUpdateSizeThick(CCmdUI* pCmdUI);

    afx_msg void OnMyUndo();
    afx_msg void OnMyRedo();
    afx_msg void OnUpdateMyUndo(CCmdUI* pCmdUI);
    afx_msg void OnUpdateMyRedo(CCmdUI* pCmdUI);

    afx_msg void OnEditClearAll();
    afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);

    afx_msg void OnFileExportPng();

    afx_msg void OnLanguageChinese();
    afx_msg void OnLanguageEnglish();
    afx_msg void OnLanguageKorean();

    afx_msg void OnUpdateLanguageChinese(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLanguageEnglish(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLanguageKorean(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CDrawingBoardDoc* CDrawingBoardView::GetDocument() const
{
    return reinterpret_cast<CDrawingBoardDoc*>(m_pDocument);
}
#endif

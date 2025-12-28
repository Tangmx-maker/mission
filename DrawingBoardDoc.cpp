#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "DrawingBoard.h"
#endif

#include "DrawingBoardDoc.h"
#include <propkey.h>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDrawingBoardDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrawingBoardDoc, CDocument)
END_MESSAGE_MAP()

// ========== 构造和析构 ==========
CDrawingBoardDoc::CDrawingBoardDoc() noexcept
{
}

CDrawingBoardDoc::~CDrawingBoardDoc()
{
}

BOOL CDrawingBoardDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    m_strokes.clear();
    m_redoStack.clear();

    return TRUE;
}

// ========== 序列化 ==========
void CDrawingBoardDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // 保存笔画数量
        int count = (int)m_strokes.size();
        ar << count;

        // 保存每个笔画
        for (const auto& stroke : m_strokes)
        {
            // 保存点的数量
            int pointCount = (int)stroke.points.size();
            ar << pointCount;

            // 保存所有点
            for (const auto& point : stroke.points)
            {
                ar << point;
            }

            // 保存颜色和宽度
            ar << stroke.color;
            ar << stroke.penWidth;
        }
    }
    else
    {
        // 清空现有数据
        m_strokes.clear();
        m_redoStack.clear();

        // 读取笔画数量
        int count;
        ar >> count;

        // 读取每个笔画
        for (int i = 0; i < count; i++)
        {
            Stroke stroke;

            // 读取点的数量
            int pointCount;
            ar >> pointCount;

            // 读取所有点
            for (int j = 0; j < pointCount; j++)
            {
                CPoint point;
                ar >> point;
                stroke.points.push_back(point);
            }

            // 读取颜色和宽度
            ar >> stroke.color;
            ar >> stroke.penWidth;

            m_strokes.push_back(stroke);
        }
    }
}

#ifdef SHARED_HANDLERS
void CDrawingBoardDoc::InitializeSearchContent()
{
    CString strSearchContent;
    SetSearchContent(strSearchContent);
}

void CDrawingBoardDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl* pChunk = nullptr;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != nullptr)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}
#endif

// ========== 绘图操作 ==========
void CDrawingBoardDoc::AddStroke(const std::vector<CPoint>& points, COLORREF color, int penWidth)
{
    if (points.empty())
        return;

    Stroke stroke(points, color, penWidth);
    m_strokes.push_back(stroke);

    // 添加新笔画后清空重做栈
    m_redoStack.clear();

    SetModifiedFlag();
}

// ========== 撤销/重做 ==========
void CDrawingBoardDoc::Undo()
{
    if (!m_strokes.empty())
    {
        // 将最后一个笔画移到重做栈
        m_redoStack.push_back(m_strokes.back());
        m_strokes.pop_back();
        SetModifiedFlag();
    }
}

void CDrawingBoardDoc::Redo()
{
    if (!m_redoStack.empty())
    {
        // 从重做栈恢复笔画
        m_strokes.push_back(m_redoStack.back());
        m_redoStack.pop_back();
        SetModifiedFlag();
    }
}

// ========== 清空画布 ==========
void CDrawingBoardDoc::Clear()
{
    m_strokes.clear();
    m_redoStack.clear();
    SetModifiedFlag();
}

// ========== 文件操作 ==========
bool CDrawingBoardDoc::SaveToFile(const CString& filename)
{
    try
    {
        CFile file;
        if (!file.Open(filename, CFile::modeCreate | CFile::modeWrite))
            return false;

        CArchive ar(&file, CArchive::store);
        Serialize(ar);
        ar.Close();
        file.Close();

        SetModifiedFlag(FALSE);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool CDrawingBoardDoc::LoadFromFile(const CString& filename)
{
    try
    {
        CFile file;
        if (!file.Open(filename, CFile::modeRead))
            return false;

        CArchive ar(&file, CArchive::load);
        Serialize(ar);
        ar.Close();
        file.Close();

        SetModifiedFlag(FALSE);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

// ========== 调试函数 ==========
#ifdef _DEBUG
void CDrawingBoardDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CDrawingBoardDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

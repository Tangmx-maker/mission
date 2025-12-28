#pragma once
#include <vector>
#include <deque>

// 笔画结构体
struct Stroke
{
    std::vector<CPoint> points;   // 笔画的点集合
    COLORREF color;               // 颜色
    int penWidth;                 // 画笔宽度

    // 构造函数
    Stroke() : color(RGB(0, 0, 0)), penWidth(1) {}

    Stroke(const std::vector<CPoint>& pts, COLORREF clr, int width)
        : points(pts), color(clr), penWidth(width) {
    }
};

class CDrawingBoardDoc : public CDocument
{
protected:
    CDrawingBoardDoc() noexcept;
    DECLARE_DYNCREATE(CDrawingBoardDoc)

public:
    virtual ~CDrawingBoardDoc();
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif

    // 绘图操作接口
    void AddStroke(const std::vector<CPoint>& points, COLORREF color, int penWidth);
    const std::vector<Stroke>& GetStrokes() const { return m_strokes; }

    // 撤销/重做
    void Undo();
    void Redo();
    bool CanUndo() const { return !m_strokes.empty(); }
    bool CanRedo() const { return !m_redoStack.empty(); }

    // 清空画布
    void Clear();

    // 文件操作
    bool SaveToFile(const CString& filename);
    bool LoadFromFile(const CString& filename);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    void SetSearchContent(const CString& value);
#endif

private:
    std::vector<Stroke> m_strokes;        // 所有笔画
    std::deque<Stroke> m_redoStack;       // 重做栈
};

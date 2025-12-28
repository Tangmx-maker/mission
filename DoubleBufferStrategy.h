// DoubleBufferStrategy.h
#pragma once
#include "IRenderStrategy.h"

class DoubleBufferStrategy : public IRenderStrategy {
private:
    CDC m_memDC;
    CBitmap m_memBitmap;
    CBitmap* m_pOldBitmap;
    CRect m_rect;
    bool m_initialized;

public:
    DoubleBufferStrategy();
    virtual ~DoubleBufferStrategy();

    void Initialize(CDC* pDC, const CRect& rect);
    void Render(CDC* pDC) override;
    CDC* GetMemoryDC() { return &m_memDC; }
};

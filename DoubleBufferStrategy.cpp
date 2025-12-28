// DoubleBufferStrategy.cpp
#include "pch.h"
#include "DoubleBufferStrategy.h"

DoubleBufferStrategy::DoubleBufferStrategy()
    : m_pOldBitmap(nullptr)
    , m_initialized(false) {
}

DoubleBufferStrategy::~DoubleBufferStrategy() {
    if (m_pOldBitmap) {
        m_memDC.SelectObject(m_pOldBitmap);
    }
}

void DoubleBufferStrategy::Initialize(CDC* pDC, const CRect& rect) {
    if (!m_initialized || m_rect != rect) {
        if (m_pOldBitmap) {
            m_memDC.SelectObject(m_pOldBitmap);
            m_memBitmap.DeleteObject();
        }

        m_rect = rect;
        m_memDC.CreateCompatibleDC(pDC);
        m_memBitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
        m_pOldBitmap = m_memDC.SelectObject(&m_memBitmap);
        m_initialized = true;
    }
}

void DoubleBufferStrategy::Render(CDC* pDC) {
    if (m_initialized) {
        pDC->BitBlt(0, 0, m_rect.Width(), m_rect.Height(),
            &m_memDC, 0, 0, SRCCOPY);
    }
}

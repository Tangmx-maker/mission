// IRenderStrategy.h
#pragma once
#include <afxwin.h>

// äÖÈ¾²ßÂÔ½Ó¿Ú
class IRenderStrategy {
public:
    virtual ~IRenderStrategy() = default;
    virtual void Render(CDC* pDC) = 0;
};

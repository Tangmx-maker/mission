#include "pch.h"
#include "LanguageManager.h"
#include "resource.h"

LanguageManager::LanguageManager()
    : m_currentLanguage(Language::CHINESE)
{
    InitializeStrings();
    InitializeMenuTexts();
    InitializeTopLevelMenus();
}

LanguageManager::~LanguageManager()
{
}

void LanguageManager::SetLanguage(Language lang)
{
    m_currentLanguage = lang;
}

CString LanguageManager::GetString(const CString& key) const
{
    auto it = m_strings.find(key);
    if (it != m_strings.end())
    {
        auto langIt = it->second.find(m_currentLanguage);
        if (langIt != it->second.end())
        {
            return langIt->second;
        }
    }
    return _T("");
}

CString LanguageManager::GetMenuText(UINT menuID) const
{
    auto it = m_menuTexts.find(menuID);
    if (it != m_menuTexts.end())
    {
        auto langIt = it->second.find(m_currentLanguage);
        if (langIt != it->second.end())
        {
            return langIt->second;
        }
    }
    return _T("");
}

CString LanguageManager::GetTopLevelMenuText(int menuIndex) const
{
    auto it = m_topLevelMenus.find(menuIndex);
    if (it != m_topLevelMenus.end())
    {
        auto langIt = it->second.find(m_currentLanguage);
        if (langIt != it->second.end())
        {
            return langIt->second;
        }
    }
    return _T("");
}

void LanguageManager::InitializeTopLevelMenus()
{
    m_topLevelMenus[0][Language::CHINESE] = _T("文件(&F)");
    m_topLevelMenus[0][Language::ENGLISH] = _T("&File");
    m_topLevelMenus[0][Language::KOREAN] = _T("파일(&F)");

    m_topLevelMenus[1][Language::CHINESE] = _T("编辑(&E)");
    m_topLevelMenus[1][Language::ENGLISH] = _T("&Edit");
    m_topLevelMenus[1][Language::KOREAN] = _T("편집(&E)");

    m_topLevelMenus[2][Language::CHINESE] = _T("视图(&V)");
    m_topLevelMenus[2][Language::ENGLISH] = _T("&View");
    m_topLevelMenus[2][Language::KOREAN] = _T("보기(&V)");

    m_topLevelMenus[3][Language::CHINESE] = _T("帮助(&H)");
    m_topLevelMenus[3][Language::ENGLISH] = _T("&Help");
    m_topLevelMenus[3][Language::KOREAN] = _T("도움말(&H)");

    m_topLevelMenus[4][Language::CHINESE] = _T("工具(&T)");
    m_topLevelMenus[4][Language::ENGLISH] = _T("&Tools");
    m_topLevelMenus[4][Language::KOREAN] = _T("도구(&T)");

    m_topLevelMenus[5][Language::CHINESE] = _T("颜色(&C)");
    m_topLevelMenus[5][Language::ENGLISH] = _T("&Color");
    m_topLevelMenus[5][Language::KOREAN] = _T("색상(&C)");

    m_topLevelMenus[6][Language::CHINESE] = _T("粗细(&W)");
    m_topLevelMenus[6][Language::ENGLISH] = _T("&Width");
    m_topLevelMenus[6][Language::KOREAN] = _T("두께(&W)");

    m_topLevelMenus[7][Language::CHINESE] = _T("语言(&L)");
    m_topLevelMenus[7][Language::ENGLISH] = _T("&Language");
    m_topLevelMenus[7][Language::KOREAN] = _T("언어(&L)");
}

void LanguageManager::InitializeStrings()
{
    m_strings[_T("MSG_CLEAR_CONFIRM")][Language::CHINESE] =
        _T("确定要清空画板吗？此操作不可恢复！");
    m_strings[_T("MSG_CLEAR_CONFIRM")][Language::ENGLISH] =
        _T("Are you sure to clear the drawing board? This action cannot be undone!");
    m_strings[_T("MSG_CLEAR_CONFIRM")][Language::KOREAN] =
        _T("그림판을 지우시겠습니까? 이 작업은 취소할 수 없습니다!");

    m_strings[_T("MSG_CLEAR_SUCCESS")][Language::CHINESE] =
        _T("画板已清空");
    m_strings[_T("MSG_CLEAR_SUCCESS")][Language::ENGLISH] =
        _T("Drawing board has been cleared");
    m_strings[_T("MSG_CLEAR_SUCCESS")][Language::KOREAN] =
        _T("그림판이 지워졌습니다");

    m_strings[_T("MSG_BOARD_EMPTY")][Language::CHINESE] =
        _T("画板已经是空的");
    m_strings[_T("MSG_BOARD_EMPTY")][Language::ENGLISH] =
        _T("Drawing board is already empty");
    m_strings[_T("MSG_BOARD_EMPTY")][Language::KOREAN] =
        _T("그림판이 이미 비어 있습니다");

    m_strings[_T("MSG_LANGUAGE_CHANGED")][Language::CHINESE] =
        _T("语言已切换");
    m_strings[_T("MSG_LANGUAGE_CHANGED")][Language::ENGLISH] =
        _T("Language has been changed");
    m_strings[_T("MSG_LANGUAGE_CHANGED")][Language::KOREAN] =
        _T("언어가 변경되었습니다");

    m_strings[_T("MSG_BUFFER_NOT_INIT")][Language::CHINESE] =
        _T("缓冲区未初始化");
    m_strings[_T("MSG_BUFFER_NOT_INIT")][Language::ENGLISH] =
        _T("Buffer not initialized");
    m_strings[_T("MSG_BUFFER_NOT_INIT")][Language::KOREAN] =
        _T("버퍼가 초기화되지 않았습니다");

    m_strings[_T("MSG_EXPORT_FAILED")][Language::CHINESE] =
        _T("导出失败");
    m_strings[_T("MSG_EXPORT_FAILED")][Language::ENGLISH] =
        _T("Export failed");
    m_strings[_T("MSG_EXPORT_FAILED")][Language::KOREAN] =
        _T("내보내기 실패");

    m_strings[_T("MSG_EXPORT_SUCCESS")][Language::CHINESE] =
        _T("成功导出到：%s");
    m_strings[_T("MSG_EXPORT_SUCCESS")][Language::ENGLISH] =
        _T("Successfully exported to: %s");
    m_strings[_T("MSG_EXPORT_SUCCESS")][Language::KOREAN] =
        _T("성공적으로 내보냈습니다: %s");

    m_strings[_T("MSG_NO_PNG_ENCODER")][Language::CHINESE] =
        _T("找不到PNG编码器");
    m_strings[_T("MSG_NO_PNG_ENCODER")][Language::ENGLISH] =
        _T("PNG encoder not found");
    m_strings[_T("MSG_NO_PNG_ENCODER")][Language::KOREAN] =
        _T("PNG 인코더를 찾을 수 없습니다");

    m_strings[_T("DEFAULT_FILENAME")][Language::CHINESE] =
        _T("我的画板");
    m_strings[_T("DEFAULT_FILENAME")][Language::ENGLISH] =
        _T("MyDrawing");
    m_strings[_T("DEFAULT_FILENAME")][Language::KOREAN] =
        _T("내그림");

    m_strings[_T("FILE_FILTER")][Language::CHINESE] =
        _T("PNG文件(*.png)|*.png|所有文件(*.*)|*.*||");
    m_strings[_T("FILE_FILTER")][Language::ENGLISH] =
        _T("PNG Files(*.png)|*.png|All Files(*.*)|*.*||");
    m_strings[_T("FILE_FILTER")][Language::KOREAN] =
        _T("PNG 파일(*.png)|*.png|모든 파일(*.*)|*.*||");
}

void LanguageManager::InitializeMenuTexts()
{
    m_menuTexts[ID_FILE_NEW][Language::CHINESE] = _T("新建(&N)\tCtrl+N");
    m_menuTexts[ID_FILE_NEW][Language::ENGLISH] = _T("&New\tCtrl+N");
    m_menuTexts[ID_FILE_NEW][Language::KOREAN] = _T("새로 만들기(&N)\tCtrl+N");

    m_menuTexts[ID_FILE_OPEN][Language::CHINESE] = _T("打开(&O)\tCtrl+O");
    m_menuTexts[ID_FILE_OPEN][Language::ENGLISH] = _T("&Open\tCtrl+O");
    m_menuTexts[ID_FILE_OPEN][Language::KOREAN] = _T("열기(&O)\tCtrl+O");

    m_menuTexts[ID_FILE_SAVE][Language::CHINESE] = _T("保存(&S)\tCtrl+S");
    m_menuTexts[ID_FILE_SAVE][Language::ENGLISH] = _T("&Save\tCtrl+S");
    m_menuTexts[ID_FILE_SAVE][Language::KOREAN] = _T("저장(&S)\tCtrl+S");

    m_menuTexts[ID_FILE_SAVE_AS][Language::CHINESE] = _T("另存为(&A)");
    m_menuTexts[ID_FILE_SAVE_AS][Language::ENGLISH] = _T("Save &As");
    m_menuTexts[ID_FILE_SAVE_AS][Language::KOREAN] = _T("다른 이름으로 저장(&A)");

    m_menuTexts[ID_FILE_EXPORT_PNG][Language::CHINESE] = _T("导出为PNG(&P)");
    m_menuTexts[ID_FILE_EXPORT_PNG][Language::ENGLISH] = _T("Export to &PNG");
    m_menuTexts[ID_FILE_EXPORT_PNG][Language::KOREAN] = _T("PNG로 내보내기(&P)");

    m_menuTexts[ID_APP_EXIT][Language::CHINESE] = _T("退出(&X)");
    m_menuTexts[ID_APP_EXIT][Language::ENGLISH] = _T("E&xit");
    m_menuTexts[ID_APP_EXIT][Language::KOREAN] = _T("종료(&X)");

    m_menuTexts[ID_MY_UNDO][Language::CHINESE] = _T("撤销(&U)\tCtrl+Z");
    m_menuTexts[ID_MY_UNDO][Language::ENGLISH] = _T("&Undo\tCtrl+Z");
    m_menuTexts[ID_MY_UNDO][Language::KOREAN] = _T("실행 취소(&U)\tCtrl+Z");

    m_menuTexts[ID_MY_REDO][Language::CHINESE] = _T("重做(&R)\tCtrl+Y");
    m_menuTexts[ID_MY_REDO][Language::ENGLISH] = _T("&Redo\tCtrl+Y");
    m_menuTexts[ID_MY_REDO][Language::KOREAN] = _T("다시 실행(&R)\tCtrl+Y");

    m_menuTexts[ID_EDIT_CLEAR_ALL][Language::CHINESE] = _T("清空画板(&C)");
    m_menuTexts[ID_EDIT_CLEAR_ALL][Language::ENGLISH] = _T("&Clear All");
    m_menuTexts[ID_EDIT_CLEAR_ALL][Language::KOREAN] = _T("모두 지우기(&C)");

    m_menuTexts[ID_TOOL_PEN][Language::CHINESE] = _T("铅笔(&P)");
    m_menuTexts[ID_TOOL_PEN][Language::ENGLISH] = _T("&Pen");
    m_menuTexts[ID_TOOL_PEN][Language::KOREAN] = _T("연필(&P)");

    m_menuTexts[ID_TOOL_LINE][Language::CHINESE] = _T("直线(&L)");
    m_menuTexts[ID_TOOL_LINE][Language::ENGLISH] = _T("&Line");
    m_menuTexts[ID_TOOL_LINE][Language::KOREAN] = _T("선(&L)");

    m_menuTexts[ID_TOOL_RECTANGLE][Language::CHINESE] = _T("矩形(&R)");
    m_menuTexts[ID_TOOL_RECTANGLE][Language::ENGLISH] = _T("&Rectangle");
    m_menuTexts[ID_TOOL_RECTANGLE][Language::KOREAN] = _T("사각형(&R)");

    m_menuTexts[ID_TOOL_ELLIPSE][Language::CHINESE] = _T("椭圆(&E)");
    m_menuTexts[ID_TOOL_ELLIPSE][Language::ENGLISH] = _T("&Ellipse");
    m_menuTexts[ID_TOOL_ELLIPSE][Language::KOREAN] = _T("타원(&E)");

    m_menuTexts[ID_TOOL_ERASER][Language::CHINESE] = _T("橡皮擦(&X)");
    m_menuTexts[ID_TOOL_ERASER][Language::ENGLISH] = _T("Eraser(&X)");
    m_menuTexts[ID_TOOL_ERASER][Language::KOREAN] = _T("지우개(&X)");

    m_menuTexts[ID_COLOR_BLACK][Language::CHINESE] = _T("黑色(&B)");
    m_menuTexts[ID_COLOR_BLACK][Language::ENGLISH] = _T("&Black");
    m_menuTexts[ID_COLOR_BLACK][Language::KOREAN] = _T("검정(&B)");

    m_menuTexts[ID_COLOR_RED][Language::CHINESE] = _T("红色(&R)");
    m_menuTexts[ID_COLOR_RED][Language::ENGLISH] = _T("&Red");
    m_menuTexts[ID_COLOR_RED][Language::KOREAN] = _T("빨강(&R)");

    m_menuTexts[ID_COLOR_GREEN][Language::CHINESE] = _T("绿色(&G)");
    m_menuTexts[ID_COLOR_GREEN][Language::ENGLISH] = _T("&Green");
    m_menuTexts[ID_COLOR_GREEN][Language::KOREAN] = _T("초록(&G)");

    m_menuTexts[ID_COLOR_BLUE][Language::CHINESE] = _T("蓝色(&L)");
    m_menuTexts[ID_COLOR_BLUE][Language::ENGLISH] = _T("B&lue");
    m_menuTexts[ID_COLOR_BLUE][Language::KOREAN] = _T("파랑(&L)");

    m_menuTexts[ID_COLOR_YELLOW][Language::CHINESE] = _T("黄色(&Y)");
    m_menuTexts[ID_COLOR_YELLOW][Language::ENGLISH] = _T("&Yellow");
    m_menuTexts[ID_COLOR_YELLOW][Language::KOREAN] = _T("노랑(&Y)");

    m_menuTexts[ID_SIZE_THIN][Language::CHINESE] = _T("细(&T)");
    m_menuTexts[ID_SIZE_THIN][Language::ENGLISH] = _T("&Thin");
    m_menuTexts[ID_SIZE_THIN][Language::KOREAN] = _T("가늘게(&T)");

    m_menuTexts[ID_SIZE_MEDIUM][Language::CHINESE] = _T("中(&M)");
    m_menuTexts[ID_SIZE_MEDIUM][Language::ENGLISH] = _T("&Medium");
    m_menuTexts[ID_SIZE_MEDIUM][Language::KOREAN] = _T("중간(&M)");

    m_menuTexts[ID_SIZE_THICK][Language::CHINESE] = _T("粗(&K)");
    m_menuTexts[ID_SIZE_THICK][Language::ENGLISH] = _T("Thic&k");
    m_menuTexts[ID_SIZE_THICK][Language::KOREAN] = _T("굵게(&K)");

    m_menuTexts[ID_LANGUAGE_CHINESE][Language::CHINESE] = _T("中文(&C)");
    m_menuTexts[ID_LANGUAGE_CHINESE][Language::ENGLISH] = _T("&Chinese");
    m_menuTexts[ID_LANGUAGE_CHINESE][Language::KOREAN] = _T("중국어(&C)");

    m_menuTexts[ID_LANGUAGE_ENGLISH][Language::CHINESE] = _T("English(&E)");
    m_menuTexts[ID_LANGUAGE_ENGLISH][Language::ENGLISH] = _T("&English");
    m_menuTexts[ID_LANGUAGE_ENGLISH][Language::KOREAN] = _T("영어(&E)");

    m_menuTexts[ID_LANGUAGE_KOREAN][Language::CHINESE] = _T("한국어(&K)");
    m_menuTexts[ID_LANGUAGE_KOREAN][Language::ENGLISH] = _T("&Korean");
    m_menuTexts[ID_LANGUAGE_KOREAN][Language::KOREAN] = _T("한국어(&K)");

    m_menuTexts[ID_VIEW_TOOLBAR][Language::CHINESE] = _T("工具栏(&T)");
    m_menuTexts[ID_VIEW_TOOLBAR][Language::ENGLISH] = _T("&Toolbar");
    m_menuTexts[ID_VIEW_TOOLBAR][Language::KOREAN] = _T("도구 모음(&T)");

    m_menuTexts[ID_VIEW_STATUS_BAR][Language::CHINESE] = _T("状态栏(&S)");
    m_menuTexts[ID_VIEW_STATUS_BAR][Language::ENGLISH] = _T("&Status Bar");
    m_menuTexts[ID_VIEW_STATUS_BAR][Language::KOREAN] = _T("상태 표시줄(&S)");

    m_menuTexts[ID_APP_ABOUT][Language::CHINESE] = _T("关于(&A)");
    m_menuTexts[ID_APP_ABOUT][Language::ENGLISH] = _T("&About");
    m_menuTexts[ID_APP_ABOUT][Language::KOREAN] = _T("정보(&A)");
}

#pragma once
#include <map>
#include <string>

enum class Language
{
    CHINESE,
    ENGLISH,
    KOREAN
};

class LanguageManager
{
public:
    LanguageManager();
    ~LanguageManager();

    void SetLanguage(Language lang);
    Language GetLanguage() const { return m_currentLanguage; }

    CString GetString(const CString& key) const;
    CString GetMenuText(UINT menuID) const;
    CString GetTopLevelMenuText(int menuIndex) const;

private:
    void InitializeStrings();
    void InitializeMenuTexts();
    void InitializeTopLevelMenus();

    Language m_currentLanguage;
    std::map<CString, std::map<Language, CString>> m_strings;
    std::map<UINT, std::map<Language, CString>> m_menuTexts;
    std::map<int, std::map<Language, CString>> m_topLevelMenus;
};

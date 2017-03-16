#pragma once
class CLanguageInterface
{
public:
	CLanguageInterface(void);
	~CLanguageInterface(void);
public:
	//////////////////////////////////////////////////////////////////////////
	//Singlonģʽ
	static void ExitInstance();
	static CLanguageInterface* Instance();
	static CLanguageInterface* _instance;
public:
	CString GetStr(unsigned int uiResourceID,int iMode = 0);
	void   InitMainMenu(CMenu* pMenu);
	void   InitRightMenu(CMenu* pMenu,int nMenuPos);
	BOOL	LoadLanauage();
	void   InitDialog(UINT uiDlgID,CDialog* pDlg);
	void   InitToolBar(UINT uiToolBar,CMFCToolBar* pToolBar);
private:
	map<int,CString> m_mapLanguage;
	map<int,CString> m_mapMenuLanguage;
	map<int,CString> m_mapTrade;
	CString GetMenuStr(unsigned int uiResourceID);
};


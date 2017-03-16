
// KOAClient.h : KOAClient 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
//#include "DlgMainWnd.h"
#include "wininet.h"
// CKOAClientApp:
// 有关此类的实现，请参阅 KOAClient.cpp
//

class CKOAClientApp : public CWinAppEx
{
public:
	CKOAClientApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	//afx_msg void OnFileNew();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	BOOL InitAfterLinked();

	void InitAfterLogin(int nLoginResult);
	BOOL AskLoginDlg();
private:
	//自动升级使用
// 	CString GetModifyTime(CString appname);
// 	CString GetFileVersion(CString strFileName);
	BOOL	CheckUpdate();//检查升级
	//CDlgMainWnd *m_pdlgMainWnd;
	ULONG_PTR m_pGdiToken;

	BOOL Get(CString &csResponse,//返回的内容
		const char *szServer,//服务器
		INTERNET_PORT& nPort,//端口
		const char* szObject,//URI
		DWORD& dwHttpStatus,
		DWORD dwOutTm = 8000);//结果
};

extern CKOAClientApp theApp;

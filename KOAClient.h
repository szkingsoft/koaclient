
// KOAClient.h : KOAClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
//#include "DlgMainWnd.h"
#include "wininet.h"
// CKOAClientApp:
// �йش����ʵ�֣������ KOAClient.cpp
//

class CKOAClientApp : public CWinAppEx
{
public:
	CKOAClientApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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
	//�Զ�����ʹ��
// 	CString GetModifyTime(CString appname);
// 	CString GetFileVersion(CString strFileName);
	BOOL	CheckUpdate();//�������
	//CDlgMainWnd *m_pdlgMainWnd;
	ULONG_PTR m_pGdiToken;

	BOOL Get(CString &csResponse,//���ص�����
		const char *szServer,//������
		INTERNET_PORT& nPort,//�˿�
		const char* szObject,//URI
		DWORD& dwHttpStatus,
		DWORD dwOutTm = 8000);//���
};

extern CKOAClientApp theApp;

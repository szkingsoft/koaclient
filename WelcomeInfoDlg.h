#if !defined(AFX_WELCOMEINFODLG_H__30A87CA2_3BFF_47F9_8432_2F10E6D51083__INCLUDED_)
#define AFX_WELCOMEINFODLG_H__30A87CA2_3BFF_47F9_8432_2F10E6D51083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WelcomeInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWelcomeInfoDlg dialog
#include "HtmlCtrl.h"

class CWelcomeInfoDlg : public CDialog
{
// Construction
public:
	CWelcomeInfoDlg(int nSystemUpdateType,CString strSystemMessage,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWelcomeInfoDlg)
	enum { IDD = IDD_WELCOMEDLG };
	CButton	m_chkNotice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcomeInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWelcomeInfoDlg)
	virtual void OnCancel();
	afx_msg void OnNoticechk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    int m_nSystemUpdateType;

	CHtmlCtrl	m_ctrlNoticeInfo;//通知信息控件
	CString	m_szNoticeInfo;//显示信息
public:
	afx_msg void OnBnClickedBtnSysupdate();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOMEINFODLG_H__30A87CA2_3BFF_47F9_8432_2F10E6D51083__INCLUDED_)

#if !defined(AFX_SYSPROTYDLG_H__C7A1F003_8021_40DF_AB6E_7CC2FF4F2289__INCLUDED_)
#define AFX_SYSPROTYDLG_H__C7A1F003_8021_40DF_AB6E_7CC2FF4F2289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysProtyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysProtyDlg dialog
#include "SysSetDlg.h"
class CSysProtyDlg : public CDialog
{
// Construction
public:
	void SetActvPage(int nPage);//设置活动页
	void SetSchemeName(CString szName);
	CSysProtyDlg(CWnd* pParent = NULL);   // standard constructor

	CSysSetDlg     sysPage; //外观设置页
	CPropertySheet m_sheet;

// Dialog Data
	//{{AFX_DATA(CSysProtyDlg)
	enum { IDD = IDD_SYSPROTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysProtyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSysProtyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nPageType;     //页面索引
	CString m_szAppPath; //应用程序路径
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSPROTYDLG_H__C7A1F003_8021_40DF_AB6E_7CC2FF4F2289__INCLUDED_)

#if !defined(AFX_LOGINDLG_H__B2F69E57_B660_41F8_9858_31917D9692FA__INCLUDED_)
#define AFX_LOGINDLG_H__B2F69E57_B660_41F8_9858_31917D9692FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog
//#include "pictureex.h"
#include "WDialog.h"
#include "resource.h"
#include "afxwin.h"
#include "IdentifyCode.h"
#include "HyperlinkStatic.h"
#include "TranslucentBtn.h"

#define  EVENT_CONNECT_OUT     1000
#define  EVENT_CONNECT_OUT_TM  1000*30
class CLoginDlg : public WDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
public:
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGIN };
//	CPictureEx	m_ctrlGifTitle;
	CString	m_strUserName;
	CString	m_strPassword;
   
	//HBITMAP m_hLoginbitmap;
	//HBITMAP m_hCancelbitmap;
	//HBITMAP m_hExitbitmap;


	BOOL	m_bSavePassword;
	//}}AFX_DATA
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	DECLARE_MESSAGE_MAP()
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnSetTextMsg( WPARAM wParam, LPARAM lParam);

// Implementation
public:   
 
	void ReturnStart();

  	void Open(int nMinPos = 0 ,int nMaxPos = 100);
	
	void Close();
	
	// 设置进度范围
	BOOL SetRange(int nMinPos, int nMaxPos);
	
	// 设置当前进度位置
	void SetPos(int nCurPos,const char *szPrompt = NULL);

	
protected:
	void SetButtonStat(int nCount,int *pEnableBtnIDList);
	
	void InitCtrl();
private:
	UINT m_uiConnectTimer;//连接超时计数器
   	int m_nMinPos, m_nMaxPos;  //进度的范围
	int m_nCurPos;
	int m_nMovePos;
	int	m_bPwdChanged;
	CString m_strMemPwd;
	BOOL	m_bLoginStatus;//登陆状态
	// 验证码图片
	CIdentifyCode m_stcCheckCode;
	CHyperlinkStatic m_stcDemoLink;
	CComboBox m_commServerList;
	CProgressCtrl m_progressStep;
	CEdit         m_editPrompt;
	CTranslucentBtn	m_LoginBtn;
	CTranslucentBtn	m_ExitBtn;

	CStatic m_statServer;
	CStatic m_statAccount;
	CStatic m_statPwd;
	CStatic m_statCheckCode;
	CButton m_btnReadWarnInfo;
	CButton m_btnRadio1;
	CButton m_btnRadio2;
	CEdit m_edtUserName;
	CEdit m_edtPassWord;
	CEdit m_edtCheckCode;
	CButton m_btnSaveAccount;
	CButton m_btnSavePassWord;
	CTranslucentBtn m_btnReadWarn;
	CStatic m_statVersion;
	CStatic m_statVersionName;
public:
	
	CString m_strValidateCode;
	afx_msg void OnEnChangePasswordLogin();
	int m_nServerSel;
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonReadwarn();
	BOOL m_bCheckWarnInfo;
	afx_msg void OnBnClickedCheckReadwarninfo();
	BOOL m_bSaveAccount;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStcCheckcode();
	afx_msg void OnStnDblclickStcCheckcode();
	afx_msg void OnStnClickedCc();

	afx_msg LRESULT OnTradeMessage(WPARAM wParam,LPARAM lParam);

	int m_nNeedCFCA;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__B2F69E57_B660_41F8_9858_31917D9692FA__INCLUDED_)

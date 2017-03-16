#include "afxwin.h"
#if !defined(AFX_INPUT_H__62610E74_A0BC_11D3_BF45_0080C8EAED17__INCLUDED_)
#define AFX_INPUT_H__62610E74_A0BC_11D3_BF45_0080C8EAED17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Input.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CInput dialog

#define KEYGHOST_STOCK 0//键盘精灵发出证券代码
#define KEYGHOST_COMMAND 2//键盘精灵发出的命令
#define KEYGHOST_FORMULA 3//键盘精灵公式命令


class  CInput : public CDialog
{
	
// Construction
public:
	void Reposition();
	void SendKeyborad(UINT nChar, UINT nRepCnt, UINT nFlags);
	CInput(CWnd* pParent = NULL);   // standard constructor
	char  cKey;
	UINT m_uiMarket;
	CBitmapButton  button_close;

	
// Dialog Data
	//{{AFX_DATA(CInput)
	enum { IDD = IDD_INPUT_DIALOG };
	CEdit	m_CodeEdit;
	CString m_szStockCode;
	CListBox	m_Name;
	CWnd* m_PosWnd;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInput)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnChangeInputEdit();
	afx_msg void OnDblclkIndexList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stcIcon;
	CStatic m_stcTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUT_H__62610E74_A0BC_11D3_BF45_0080C8EAED17__INCLUDED_)

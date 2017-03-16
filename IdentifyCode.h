#if !defined(AFX_IDETIFYCODE_H__E9B4593F_833C_476A_804B_90BD3600328C__INCLUDED_)
#define AFX_IDETIFYCODE_H__E9B4593F_833C_476A_804B_90BD3600328C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IdetifyCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIdentifyCode window

class CIdentifyCode : public CStatic
{
// Construction
public:
	CIdentifyCode();

// Attributes
public:

// Operations
public:
	char m_pRandStr[5];
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdentifyCode)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawCIdentifyCode(CDC *pDc);
	void CreateIdentifyCode();
	virtual ~CIdentifyCode();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIdentifyCode)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_cOdds[4];
	void DrawCIdentifyCode(CDC *pDc,int *cOdds);


public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnStnClicked();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDETIFYCODE_H__E9B4593F_833C_476A_804B_90BD3600328C__INCLUDED_)

#if !defined(AFX_INDEXBAR_H__73C32594_CA01_469B_B7BD_EC274CD283EB__INCLUDED_)
#define AFX_INDEXBAR_H__73C32594_CA01_469B_B7BD_EC274CD283EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CIndexBar window

class CIndexBar : public CStatusBar
{
// Construction
public:
	CIndexBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	//////////////////////////////////////////////////////////////////////////
	//函数功能:设置昨日收盘价格
	//函数参数:iLastPrice 两个指数的昨收价
	BOOL SetLastPrice(int ID,long iLastPrice);
	//////////////////////////////////////////////////////////////////////////
	//函数功能:重画指数条
	//函数参数: ID		修改的位置
	//          pData	数据包
	//          dwDataSize 数据大小
	BOOL RedrawIndex(int ID, LPVOID pData, DWORD dwDataSize);

	virtual ~CIndexBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIndexBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CRect		m_ScollTextRect;//两个指数的区域
	CRect		m_TotalRect;//总区域
	CRect		m_ImgRect[2];//两个状态信息图像区域

	CImageList	m_ImageList;//钻台信息图像列表

	UINT			m_uiRedrawTimer;//重画时间
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXBAR_H__73C32594_CA01_469B_B7BD_EC274CD283EB__INCLUDED_)

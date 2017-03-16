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
	//��������:�����������̼۸�
	//��������:iLastPrice ����ָ�������ռ�
	BOOL SetLastPrice(int ID,long iLastPrice);
	//////////////////////////////////////////////////////////////////////////
	//��������:�ػ�ָ����
	//��������: ID		�޸ĵ�λ��
	//          pData	���ݰ�
	//          dwDataSize ���ݴ�С
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
	CRect		m_ScollTextRect;//����ָ��������
	CRect		m_TotalRect;//������
	CRect		m_ImgRect[2];//����״̬��Ϣͼ������

	CImageList	m_ImageList;//��̨��Ϣͼ���б�

	UINT			m_uiRedrawTimer;//�ػ�ʱ��
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXBAR_H__73C32594_CA01_469B_B7BD_EC274CD283EB__INCLUDED_)

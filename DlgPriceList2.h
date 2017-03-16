#pragma once
#include "PriceListWnd.h"

// CDlgPriceList dialog

class CDlgPriceList : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceList)

public:
	CDlgPriceList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPriceList();

// Dialog Data
	enum { IDD = IDD_DLG_PRICELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void InitCtrl();
	void SetWndPos(CRect &rcWnd);
	CPriceListWnd	m_PriceListWnd;

public:
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParm);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

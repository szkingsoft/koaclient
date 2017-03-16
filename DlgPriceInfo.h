#pragma once
#include "PriceInfoWnd.h"
#include "UScrollBar.h"
#include "DlgPriceInfoWnd.h"

class CDlgPriceInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceInfo)

public:
	CDlgPriceInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPriceInfo();

// Dialog Data
	enum { IDD = IDD_DLG_PRICEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	void InitCtrl();
	void SetPriceInfoWndPos(CRect &rcWnd);
	void SetScrollBarPos1(CRect &rcWnd);
	void SetScrollBarInfo1();
	void SetScrollBarInfo2();
	//CPriceInfoWnd	m_PriceInfoWnd;	
	CUScrollBar m_uVScrollBarCtrl;

	CDlgPriceInfoWnd m_dlgPriceInfoWnd;
public:
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParm);
	afx_msg LRESULT OnUpdateCtrl(WPARAM wParam,LPARAM lParm);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
};

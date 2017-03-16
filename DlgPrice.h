#pragma once
#include "DlgPriceList.h"
#include "DlgPriceInfoPack.h"
// CDlgPrice dialog
  
class CDlgPrice : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrice)

public:
	CDlgPrice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPrice();

// Dialog Data
	enum { IDD = IDD_DLG_PRICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetWndPos(CRect &rcWnd);

private:
	void InitCtrl();
	
	//void SetPriceInfoPos(CRect &rcWnd);
	void SetPriceInfoPackPos(CRect &rcWnd);
	void SetPriceLstPos(CRect &rcWnd);
	CDlgPriceList m_DlgPriceLst;
	CDlgPriceInfoPack m_DlgPriceInfoPack;

	float m_fWndRatio;
	int m_iListWith;

	void DrawBottomHide(int iStat = 0);
	void DrawTopHide(int iStat = 0);
	CRect m_rcShowBottomHide;
	CRect m_rcShowTopHide;
	int m_iBottomHight;
	int m_iWndReArrayType;
	void ReArrayCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnWndSizeChange(WPARAM wParam,LPARAM lParm);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnWndReArray(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnReLayout(WPARAM wPar,LPARAM lPar);
};

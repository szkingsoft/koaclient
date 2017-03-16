#pragma once
#include "DlgPrice.h"
#include "DlgTradeInfo.h"
#include "TabLst.h"
//#include "DlgTrendChart.h"
// CDlgMetalsMarket dialog
#define IDB_TAB_ONE        (WM_USER + 211)
#define IDB_TAB_TOW        (WM_USER + 212)
#define IDB_TAB_THREE      (WM_USER + 213)
#define IDB_TAB_FOUR       (WM_USER + 214)
class CDlgMetalsMarket : public CDialog
{
	DECLARE_DYNAMIC(CDlgMetalsMarket)

public:
	CDlgMetalsMarket(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMetalsMarket();

// Dialog Data
	enum { IDD = IDD_DLG_METALSMARKET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetDrawLineCallBcakFun();
private:
	void InitCtrl();
	void InitTradeWnd();
	void SetTradeWndPos(CRect &rcWnd);
	CTabLst m_statTab;

	CDlgPrice   m_DlgPrice;
	HWND	m_hTradeWnd;
	CDlgTradeInfo m_dlgTradeInfo;

	float m_fWndRatio;
	int m_iPriceHight;

	int m_iWndReArrayType;
	void ReArrayCtrl();
	//CDlgTrendChart m_dlgTrendChart;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	/////
	afx_msg LRESULT OnWndSizeChange(WPARAM wParam,LPARAM lParm);
	//////
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnWndReArray(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnReLayout(WPARAM wPar,LPARAM lPar);
};

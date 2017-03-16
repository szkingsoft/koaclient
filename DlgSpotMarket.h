#pragma once
#include "DlgTradeInfo.h"
#include "DlgSpotInfo.h"
#include "DlgTrendChartSpot.h "
#include "DlgTrendChart_D.h"
#include "DlgTradeInfoPop.h"

#include "DlgShowUrl.h"
// CDlgSpotMarket 对话框

class CDlgSpotMarket : public CDialog
{
	DECLARE_DYNAMIC(CDlgSpotMarket)

public:
	CDlgSpotMarket(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSpotMarket();

// 对话框数据
	enum { IDD = IDD_DLG_SPOTMARKET };
public:
	void SetDrawLineCallBcakFun();
private:
	void InitCtrl();
	void InitTradeWnd();
	void SetTradeWndPos(CRect &rcWnd);
	void ReArrayWnd(int iMode = 0,int iType = 0);

	CDlgSpotInfo m_dlgSpotInfo;
	CDlgTradeInfo m_dlgTradeInfo;
	//CDlgTradeInfoPop m_dlgTradeInfo_Pop;
	//CDlgTrendChartSpot m_dlgTrendChartSpot;
	CDlgTrendChart_D m_dlgTrendChart_D;
	CDlgShowUrl m_dlgShowUrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnRetLst(WPARAM wPar,LPARAM lPar);
	afx_msg void OnNormalOrder();
	afx_msg void OnSysWndNotify(UINT uiID);
	afx_msg void OnComRange(UINT Iid);
	afx_msg LRESULT OnShowUrl(WPARAM wPar,LPARAM lPar);
	LRESULT OnNotifyKOASys(WPARAM wPar,LPARAM lPar);
	LRESULT OnWndSizeChange(WPARAM wPar,LPARAM lPar);
};

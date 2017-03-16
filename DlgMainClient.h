#pragma once
#include "resource.h"
#include "afxwin.h"
#include "TabLst.h"
#include "GraphBtn.h"
#include "DlgMetalsMarket.h"
#include "DlgTopToolbar.h"
#include "DlgSpotMarket.h"
#include "DlgBinManagePack.h"
#include "DlgFundInfoPack.h"
#include "DlgStatus.h"
#include "DlgDrawTool.h"

#include "DlgWarnInfoEx.h"
// CDlgMainClient dialog
#define  WM_SHOWTOOLBAR   WM_USER + 4999
#define WM_DATEMANAGE     WM_USER + 5000

class CDlgMainClient : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainClient)

public:
	CDlgMainClient(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMainClient();

// Dialog Data
	enum { IDD = IDD_DLG_MIANCLIENT };
public:
	void ReArrayCtrl();
	int GetCurMarket();
	void NotifySpotMarket(UINT uiMsg,WPARAM wPar,LPARAM lPar);
	void UpdateToolBarStatus();
private:
	void InitCtrl();
	void ResetCtrl();
	void AccessCtrl();
	void SetFundCtrlDate(FUNDINFO & stcFund);
	CTabLst m_statTab;
	CGraphBtn m_BtnFund;

	CDlgTopToolbar m_DlgTopToolbar;
	CDlgMetalsMarket m_DlgMetalsMarket;
	CDlgSpotMarket   m_dlgSpotMarket;
	CDlgBinManagePack m_dlgBinManagePack;
	CDlgFundInfoPack m_dlgFundInfoPack;

	CDlgStatus m_dlgStatus;

	CDlgDrawTool m_dlgDrawTool;

	CDlgWarnInfoEx m_dlgWarn;
	int m_iClientType;
	int m_iFundPos;

	int m_iWndReArrayType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTabOne();
	afx_msg void OnTabTow();
	afx_msg void OnTabThree();
	afx_msg void OnTabFour();

	void ShowTab();
public:
	afx_msg LRESULT OnDateManage(WPARAM wPar,LPARAM lPar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnLoginSuccess(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnTradeNotify(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnOpenDrawTool(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnUpdateDrawTool(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnReLayout(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnNotifyQuoteSys(WPARAM wPar,LPARAM lPar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnNotifyKOASys(WPARAM wPar,LPARAM lPar);
};

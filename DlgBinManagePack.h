#pragma once
#include "DlgTradeInfo.h"

// CDlgBinManagePack 对话框

class CDlgBinManagePack : public CDialog
{
	DECLARE_DYNAMIC(CDlgBinManagePack)

public:
	CDlgBinManagePack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBinManagePack();

// 对话框数据
	enum { IDD = IDD_DLG_BINMANAGEPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void NotifyTradeBin(UINT iMsg,WPARAM wPar, LPARAM lPar);
private:
	void InitCtrl();
	void InitTradeWnd();
	void SetTradeWndPos(CRect &rcWnd);

	CDlgTradeInfo m_dlgTradeInfo;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

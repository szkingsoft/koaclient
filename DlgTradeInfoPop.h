#pragma once
#include "DlgTradeInfo.h"

// CDlgTradeInfoPop 对话框

class CDlgTradeInfoPop : public CDialog
{
	DECLARE_DYNAMIC(CDlgTradeInfoPop)

public:
	CDlgTradeInfoPop(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTradeInfoPop();

// 对话框数据
	enum { IDD = IDD_DLG_TRADEINFO_POP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CDlgTradeInfo m_dlgTradeInfo;

	void InitTradeWnd();
	void InitCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

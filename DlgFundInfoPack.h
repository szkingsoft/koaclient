#pragma once
#include "DlgTradeInfo.h"

// CDlgFundInfoPack 对话框

class CDlgFundInfoPack : public CDialog
{
	DECLARE_DYNAMIC(CDlgFundInfoPack)

public:
	CDlgFundInfoPack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFundInfoPack();

// 对话框数据
	enum { IDD = IDD_DLG_FUNDINFOPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void InitCtrl();
	void InitTradeWnd();
	void SetTradeWndPos(CRect &rcWnd);

	CDlgTradeInfo m_dlgTradeInfo;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

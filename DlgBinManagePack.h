#pragma once
#include "DlgTradeInfo.h"

// CDlgBinManagePack �Ի���

class CDlgBinManagePack : public CDialog
{
	DECLARE_DYNAMIC(CDlgBinManagePack)

public:
	CDlgBinManagePack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBinManagePack();

// �Ի�������
	enum { IDD = IDD_DLG_BINMANAGEPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

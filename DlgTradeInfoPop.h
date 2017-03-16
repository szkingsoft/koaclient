#pragma once
#include "DlgTradeInfo.h"

// CDlgTradeInfoPop �Ի���

class CDlgTradeInfoPop : public CDialog
{
	DECLARE_DYNAMIC(CDlgTradeInfoPop)

public:
	CDlgTradeInfoPop(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTradeInfoPop();

// �Ի�������
	enum { IDD = IDD_DLG_TRADEINFO_POP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CDlgTradeInfo m_dlgTradeInfo;

	void InitTradeWnd();
	void InitCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

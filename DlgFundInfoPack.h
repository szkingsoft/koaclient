#pragma once
#include "DlgTradeInfo.h"

// CDlgFundInfoPack �Ի���

class CDlgFundInfoPack : public CDialog
{
	DECLARE_DYNAMIC(CDlgFundInfoPack)

public:
	CDlgFundInfoPack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFundInfoPack();

// �Ի�������
	enum { IDD = IDD_DLG_FUNDINFOPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

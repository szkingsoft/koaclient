#pragma once
#include "afxwin.h"
//#include "afxwin.h"
// CDlgLockMain �Ի���

class CDlgLockMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgLockMain)

public:
	CDlgLockMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLockMain();

// �Ի�������
	enum { IDD = IDD_DLG_LOCKMAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_statTradePW;
	CEdit m_edtTradePW;
	CButton m_btnOk;
	CStatic m_statTips;

	void ReArrayCtrl();
	void InitCtrl();
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnOk();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

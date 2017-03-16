#pragma once
#include "afxwin.h"
#include "TransparentStat.h"

// CDlgStatus �Ի���

class CDlgStatus : public CDialog
{
	DECLARE_DYNAMIC(CDlgStatus)

public:
	CDlgStatus(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStatus();

// �Ի�������
	enum { IDD = IDD_DLG_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetTradeStatus(BOOL bLink = FALSE);
	void SetQuotationStatus(BOOL bLink = FALSE);
private:
	CTransparentStat m_statConnectInfo;
	CTransparentStat m_statTradeStatus;
	CTransparentStat m_statQuotationStatus;

	CTransparentStat m_statUserInfo;
	CString m_strImgLink;
	CString m_strImgUnLink;
	BOOL m_TradeLink;
	BOOL m_QuotationLink;

	
	void ReArrayCtrl();
	void InitCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnUpdateLinkStatus(WPARAM Wpar,LPARAM lPar);
	afx_msg LRESULT OnUpdateUserInfo(WPARAM Wpar,LPARAM lPar);
	afx_msg LRESULT OnSkinChange(WPARAM Wpar,LPARAM lPar);
	afx_msg LRESULT OnUpdateUserSGRN(WPARAM Wpar,LPARAM lPar);
	afx_msg void OnPaint();
};

#pragma once
#include "afxwin.h"
#include "TranslucentBtn.h"
#include "TransparentStat.h"
// CDlgTopToolbar 对话框

class CDlgTopToolbar : public CDialog
{
	DECLARE_DYNAMIC(CDlgTopToolbar)

public:
	CDlgTopToolbar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTopToolbar();

// 对话框数据
	enum { IDD = IDD_DLG_TOPTOOLBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetShowMode(int iMode = 0);
	void UpdateButtonStatus();

private:
	void IntiCtrl();
	void CalculateWndStatus(CWnd *pWnd,BOOL iShow);
	void ArrayCtrl();
	CTranslucentBtn m_BtnCalculator;
	CTranslucentBtn m_btnDeliveryReport;
	CTranslucentBtn m_btnApplyGold;
	CTranslucentBtn m_btnApplyGolden;
	CTranslucentBtn m_btnModifyPW;
	CTranslucentBtn m_btnBossKey;
	CTranslucentBtn m_btnRefresh;
	CTranslucentBtn m_btnFullScreen;
	CTranslucentBtn m_btnMall;
	CTranslucentBtn m_btnUser;
	CTransparentStat m_statUser;
	////
	Image * m_pImgBk;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnBnClickedBtnCalculator();
	afx_msg void OnBnClickedBtnRange(UINT iId);
	afx_msg LRESULT OnUpdateUserInfo(WPARAM Wpar,LPARAM lPar);
	afx_msg void OnBnClickedBtnDeliveryreport();
	afx_msg LRESULT OnUpdateUserSGRN(WPARAM Wpar,LPARAM lPar);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
	afx_msg void OnBnClickedBtnMall();
};

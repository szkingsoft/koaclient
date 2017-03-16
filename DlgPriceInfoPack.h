#pragma once
#include "DlgPriceInfo.h"

#include "DlgTrendChart.h"
// CDlgPriceInfoPack dialog

class CDlgPriceInfoPack : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceInfoPack)

public:
	CDlgPriceInfoPack(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPriceInfoPack();

// Dialog Data
	enum { IDD = IDD_DLG_PRICEINFO_PACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	void InitCtrl();
	void SetPriceInfoPos(CRect &rcWnd);
	CDlgPriceInfo m_DlgPriceInfo;

	CDlgTrendChart m_dlgTrendChart;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
};

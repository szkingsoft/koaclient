#pragma once
#include "StatLst.h"


// CDlgSpotInfo 对话框

class CDlgSpotInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgSpotInfo)

public:
	CDlgSpotInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSpotInfo();

// 对话框数据
	enum { IDD = IDD_DLG_SPOTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void InitCtrl();
	DWORD GetItemColor(CString strStandardVl, CString strCurVl);
	DWORD GetItemColor(float fCurP);
	CString IsZeroPrice(CString strFormat,CString strCurPirce);
	BOOL IsZeroPrice(CString strFormat,float fCurPirce);
	void SetLstDate();
	CStatLst m_statSpotLst;

	void UpdateReportItem(int nDisplayIndex,int nIndex,const STUNowDataZYQ &tCurRow,int iUpDateItem=0);
public:	
	afx_msg LRESULT OnDataArrive(WPARAM wPar,LPARAM lPar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnDbClickItem(WPARAM wParam,LPARAM lParm);
	afx_msg LRESULT OnClickItem(WPARAM wParam,LPARAM lParm);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
};

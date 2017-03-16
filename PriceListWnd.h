#pragma once


// CPriceListWnd
#include "ReportTable.h"
class CPriceListWnd : public CListCtrl
{
	DECLARE_DYNAMIC(CPriceListWnd)

public:
	CPriceListWnd();
	virtual ~CPriceListWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void RButtonDown(CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);	
	BOOL	UpdateData(char* pData,int nDataLen);
	SCodeInfo*	GetCurSel();
	void	Redraw();
private:
	CReportTable	m_ReportTable;
	BOOL			m_bShowUpdateTime;
	BOOL			m_bShowHighLow;
	BOOL			m_bShowOtherSymbol;//是否显示其他商品
public:
	BOOL			GetShowUpdateTimeStatus(){return m_bShowUpdateTime;};
	BOOL			GetShowHighLowStatus(){return m_bShowHighLow;};
	BOOL			GetShowOtherSymbol(){return m_bShowOtherSymbol;};
	void			ShowHideUpdateTime();
	void			ShowHideHighLow();
	void			ShowHideOtherSymbol();
};



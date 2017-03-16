#pragma once


// CPriceInfoWnd
#include "PriceInfoObj.h"
class CPriceInfoWnd : public CListCtrl
{
	DECLARE_DYNAMIC(CPriceInfoWnd)

public:
	CPriceInfoWnd();
	virtual ~CPriceInfoWnd();

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
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	BOOL	UpdateData(char* pData,int nDataLen);
	SCodeInfo*	GetCurSel();

	//////
	UINT GetLastHight() {return m_PriceInfoObj.GetHight();}
	void SetNotifyWnd(HWND hWnd) {m_PriceInfoObj.SetNotifyWnd(hWnd);}
private:
	CPriceInfoObj	m_PriceInfoObj;
public:

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};



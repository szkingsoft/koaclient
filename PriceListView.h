#pragma once
#include "PriceListWnd.h"
class CPriceListView : public CDockablePane
{
// 构造
public:
	CPriceListView();
	void AdjustLayout();
// 特性
protected:
// 实现
public:
	virtual ~CPriceListView();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParm);
	afx_msg LRESULT OnRedrawMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CPriceListWnd	m_PriceListWnd;
public:
	virtual void RequestData();
	afx_msg void OnClose();
};


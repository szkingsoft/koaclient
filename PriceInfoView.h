
#pragma once
#include "PriceInfoWnd.h"
class CPriceInfoView : public CDockablePane
{
public:
	CPriceInfoView();
	virtual ~CPriceInfoView();

	void AdjustLayout();
// 重写

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRedrawMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CPriceInfoWnd	m_PriceInfoWnd;		
// 	int				m_nTotalLine;		//共显示行数
// 	int				m_nDisplayLine;		//当前屏幕可显示行数
// 	int				m_nBeginLine;		//当前开始行
public:

	afx_msg void OnClose();
};


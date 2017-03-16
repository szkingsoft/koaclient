
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口
class CTradeWnd : public CDockablePane
{
// 构造
public:
	CTradeWnd();
// 特性
protected:
//	CMFCTabCtrl	m_wndTabs;
// 实现
public:
	virtual ~CTradeWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg LRESULT OnChangeActiveTab(WPARAM,LPARAM);
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnClosTradeDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStockInfoMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStockNameMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	DECLARE_MESSAGE_MAP()
public:	
	void OnUserCmd(UINT uiCmd);
	void LoadDllInfo();
private:
	HWND	m_hClientDlg;

};


#pragma once

// CDlgTradeInfo 对话框

class CDlgTradeInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgTradeInfo)

public:
	CDlgTradeInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTradeInfo();

// 对话框数据
	enum { IDD = IDD_DLG_TRADEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void InitCtrl();
	void InitTradeWnd();
	void SetTradeWndPos(CRect &rcWnd);

	StuRecvQuoteInfo* m_pQuote;
	int m_iIndex;
	void AnalyMetalsQuete(WPARAM wParam,LPARAM lParm);
	void AnalySpotQuete(WPARAM wParam,LPARAM lParm);
	void GetQuoteSymbolAttrib(mapQSA &mapOut);
public:
	void SetIndex(int iIndex) {m_iIndex = iIndex;}
public:
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParm);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

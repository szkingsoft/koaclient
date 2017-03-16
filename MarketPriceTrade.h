#pragma once


// CMarketPriceTrade 对话框

class CMarketPriceTrade : public CDialog
{
	DECLARE_DYNAMIC(CMarketPriceTrade)

public:
	CMarketPriceTrade(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMarketPriceTrade();

// 对话框数据
	enum { IDD = IDD_DLG_MARKETPRICETRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush				m_bkBrush;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

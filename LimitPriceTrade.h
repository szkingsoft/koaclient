#pragma once


// CLimitPriceTrade 对话框

class CLimitPriceTrade : public CDialog
{
	DECLARE_DYNAMIC(CLimitPriceTrade)

public:
	CLimitPriceTrade(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLimitPriceTrade();

// 对话框数据
	enum { IDD = IDD_DLG_LIMITPRICETRADE };

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

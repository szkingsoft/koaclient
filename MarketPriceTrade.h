#pragma once


// CMarketPriceTrade �Ի���

class CMarketPriceTrade : public CDialog
{
	DECLARE_DYNAMIC(CMarketPriceTrade)

public:
	CMarketPriceTrade(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMarketPriceTrade();

// �Ի�������
	enum { IDD = IDD_DLG_MARKETPRICETRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush				m_bkBrush;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#pragma once


// CLimitPriceTrade �Ի���

class CLimitPriceTrade : public CDialog
{
	DECLARE_DYNAMIC(CLimitPriceTrade)

public:
	CLimitPriceTrade(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLimitPriceTrade();

// �Ի�������
	enum { IDD = IDD_DLG_LIMITPRICETRADE };

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

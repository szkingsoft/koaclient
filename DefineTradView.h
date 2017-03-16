#pragma once
#include "LimitPriceTrade.h"
#include "MarketPriceTrade.h"
#include "PriceInfoWnd.h"
#include "PriceListWnd.h"

// CDefineTradView 窗体视图

class CDefineTradView : public CFormView
{
	DECLARE_DYNCREATE(CDefineTradView)

protected:
	CDefineTradView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDefineTradView();

public:
	enum { IDD = IDD_DLG_TRADEFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	CLimitPriceTrade*	m_pLimitPriceTrade;
	CMarketPriceTrade*	m_pMarketPriceTrade;
	CBrush				m_bkBrush;
private:
	CPriceListWnd*	m_pPriceListWnd;
	CPriceInfoWnd*	m_pPriceInfoWnd;
	CRect			m_rectSplitter;
	CRect			m_rectTotal;
	CRect			m_rectLeft;
	CRect			m_rectRight;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg LRESULT	OnDataMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRedrawMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnDraw(CDC* /*pDC*/);
};



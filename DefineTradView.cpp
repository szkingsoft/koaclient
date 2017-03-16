// DefineTradView.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DefineTradView.h"


// CDefineTradView

IMPLEMENT_DYNCREATE(CDefineTradView, CFormView)

CDefineTradView::CDefineTradView()
	: CFormView(CDefineTradView::IDD)
{
	m_pLimitPriceTrade = NULL;
	m_pMarketPriceTrade = NULL;
	m_pPriceInfoWnd = NULL;
	m_pPriceListWnd = NULL;
}

CDefineTradView::~CDefineTradView()
{
	if(m_pMarketPriceTrade)
	{
		delete m_pMarketPriceTrade;
		m_pMarketPriceTrade = NULL;
	}
	if(m_pLimitPriceTrade)
	{
		delete m_pLimitPriceTrade;
		m_pLimitPriceTrade= NULL;
	}
	if(m_pPriceInfoWnd)
	{
		delete m_pPriceInfoWnd;
		m_pPriceInfoWnd = NULL;
	}
	if(m_pPriceListWnd)
	{
		delete m_pPriceListWnd;
		m_pPriceListWnd;
	}
}

void CDefineTradView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDefineTradView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_REDRAWWND,OnRedrawMsg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDefineTradView 诊断

#ifdef _DEBUG
void CDefineTradView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDefineTradView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDefineTradView 消息处理程序


int CDefineTradView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CPublic::Instance()->RegistWnd(VIEWINFO_DEFTRADE,this);

	if(1 == CPublic::Instance()->GetMainViewMode())
	{
		m_pLimitPriceTrade = new CLimitPriceTrade;
		m_pLimitPriceTrade->Create(CLimitPriceTrade::IDD,this);
		m_pLimitPriceTrade->ShowWindow(SW_SHOW);

		m_pMarketPriceTrade = new CMarketPriceTrade;
		m_pMarketPriceTrade->Create(CMarketPriceTrade::IDD,this);
		m_pMarketPriceTrade->ShowWindow(SW_SHOW);
	}
	else if(2 == CPublic::Instance()->GetMainViewMode())
	{
		m_pPriceListWnd = new CPriceListWnd;
		const DWORD dwViewStyle =  WS_CHILD | WS_VISIBLE|LVS_LIST| WS_CLIPCHILDREN|WM_NOTIFY;
		CRect rectDummy;
		rectDummy.SetRectEmpty();
		m_pPriceListWnd->Create(dwViewStyle, rectDummy, this, 4);
		m_pPriceListWnd->ShowWindow(SW_SHOW);

		m_pPriceInfoWnd = new CPriceInfoWnd;
		m_pPriceInfoWnd->Create(dwViewStyle,rectDummy,this,4);
		m_pPriceInfoWnd->ShowWindow(SW_SHOW);
	}
	m_bkBrush.CreateSolidBrush(CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR));
	return 0;
}


void CDefineTradView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(cx ==0||cy == 0)return;
	CFormView::ShowScrollBar(SB_VERT,FALSE);

	if(1 == CPublic::Instance()->GetMainViewMode())
	{
		CRect leftrect,rightrect;
		CRect leftOrgin,righOrgin;
		m_pMarketPriceTrade->GetWindowRect(leftOrgin);
		m_pLimitPriceTrade->GetWindowRect(righOrgin);

		int nxSplideSize = (cx-leftOrgin.Width()-righOrgin.Width())/3;
		int nySplideSize = (cy-leftOrgin.Height())/2;
		nxSplideSize = nxSplideSize>0?nxSplideSize:0;
		nySplideSize = nySplideSize>0?nySplideSize:0;

		leftrect.left= nxSplideSize;
		leftrect.right = leftrect.left+leftOrgin.Width();
		leftrect.top = nySplideSize;
		leftrect.bottom = leftrect.top +leftOrgin.Height();

		rightrect.left = leftrect.right+nxSplideSize;
		rightrect.right= rightrect.left+ righOrgin.Width();
		rightrect.top = nySplideSize;
		rightrect.bottom = rightrect.top +righOrgin.Height();

		m_pMarketPriceTrade->MoveWindow(leftrect);
		m_pLimitPriceTrade->MoveWindow(rightrect);
	}
	else if(2 == CPublic::Instance()->GetMainViewMode())
	{

		m_rectSplitter= CRect(642,0,645,cy);

		m_rectLeft = CRect(0,0,642,cy);
		m_rectRight = CRect(645,0,cx,cy);

		m_pPriceListWnd->MoveWindow(m_rectLeft);
		m_pPriceInfoWnd->MoveWindow(m_rectRight);
	}
}


HBRUSH CDefineTradView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	return (HBRUSH)hbr;
}


void CDefineTradView::OnClose()
{
	CPublic::Instance()->UnRegistWnd(VIEWINFO_DEFTRADE,this);
	CFormView::OnClose();
}

LRESULT	CDefineTradView::OnDataMessage(WPARAM wParam,LPARAM lParam)
{
	if(m_pPriceInfoWnd == NULL || m_pPriceListWnd == NULL)
		return 0;

	m_pPriceInfoWnd->UpdateData((char*)wParam,(int)lParam);
	m_pPriceListWnd->UpdateData((char*)wParam,(int)lParam);
	return 0;
}

LRESULT CDefineTradView::OnRedrawMsg(WPARAM wParam,LPARAM lParam)
{
	if(m_pPriceInfoWnd == NULL || m_pPriceListWnd == NULL)
		return 0;

	m_pPriceInfoWnd->RedrawWindow();
	m_pPriceListWnd->Redraw();
	return 1;
}

void CDefineTradView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFormView::OnMouseMove(nFlags, point);
}


void CDefineTradView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFormView::OnLButtonUp(nFlags, point);
}


void CDefineTradView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFormView::OnLButtonDown(nFlags, point);
}


void CDefineTradView::OnDraw(CDC* pDC)
{
	pDC->FillRect(m_rectTotal,&m_bkBrush);
}

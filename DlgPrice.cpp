// DlgPrice.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPrice.h"
#include "afxdialogex.h"


// CDlgPrice dialog

IMPLEMENT_DYNAMIC(CDlgPrice, CDialog)

CDlgPrice::CDlgPrice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrice::IDD, pParent)
{
	m_iListWith/*m_fWndRatio*/ = CPublic::Instance()->GetTrendChartRatio();//0.5;
	//m_iListWith = 650;

	m_iWndReArrayType = 0;

	m_iBottomHight = 11;
}

CDlgPrice::~CDlgPrice()
{
}

void CDlgPrice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPrice, CDialog)
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_WNDSIZECHANGE,OnWndSizeChange)
	ON_MESSAGE(WM_WNDREARRAY,OnWndReArray)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
	ON_MESSAGE(WM_RELAYOUT,OnReLayout)
END_MESSAGE_MAP()


// CDlgPrice message handlers


BOOL CDlgPrice::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPrice::InitCtrl()
{
	CRect rcClt;
	GetClientRect(&rcClt);

	m_DlgPriceLst.Create(CDlgPriceList::IDD,this);
	SetPriceLstPos(rcClt);
	m_DlgPriceLst.ShowWindow(SW_SHOW);

	m_DlgPriceInfoPack.Create(CDlgPriceInfoPack::IDD,this);
	SetPriceInfoPackPos(rcClt);
	m_DlgPriceInfoPack.ShowWindow(SW_SHOW);

	//m_DlgPriceLst.ShowWindow(SW_HIDE);
	//m_DlgPriceInfoPack.ShowWindow(SW_HIDE);
}

void CDlgPrice::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	ReArrayCtrl();

}

//void CDlgPrice::SetPriceInfoPos(CRect &rcWnd)
//{
//	if (IsWindow(m_DlgPriceInfo.GetSafeHwnd()))
//	{
//		m_DlgPriceInfo.SetWindowPos(NULL, rcWnd.left, rcWnd.top,
//			rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
//	}	
//}

void CDlgPrice::SetPriceInfoPackPos(CRect &rcWnd)
{
	if (IsWindow(m_DlgPriceInfoPack.GetSafeHwnd()))
	{
		m_DlgPriceInfoPack.SetWindowPos(NULL, rcWnd.left, rcWnd.top,
			rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}

void CDlgPrice::SetPriceLstPos(CRect &rcWnd)
{
	if (IsWindow(m_DlgPriceLst.GetSafeHwnd()))
	{
		m_DlgPriceLst.SetWindowPos(NULL, rcWnd.left, rcWnd.top,
			rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}

void CDlgPrice::SetWndPos(CRect &rcWnd)
{
	if (IsWindow(GetSafeHwnd()))
	{
		this->SetWindowPos(NULL, rcWnd.left, rcWnd.top,
			rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}

BOOL CDlgPrice::OnEraseBkgnd(CDC* pDC)
{
	//CDialog::OnEraseBkgnd(pDC);
	CRect rcClt;
	GetClientRect(&rcClt);
	Graphics gpDraw(pDC->GetSafeHdc());
	gpDraw.SetSmoothingMode(SmoothingModeHighQuality);
	
	CDateManage::Instance()->DrawBkRect(gpDraw,rcClt);
	CSkinAttribute_Gap *pSkin = (CSkinAttribute_Gap *)CSkinTransform::Instance()->GetSkinAttribute(2);

	//Point points[] ={ Point(rcClt.left,rcClt.bottom-m_iBottomHight),Point(rcClt.left+rcClt.Width()/2,rcClt.bottom-m_iBottomHight),Point(rcClt.right,rcClt.bottom/*-m_iBottomHight*/)};//三角形顶点
	//PathGradientBrush pathbrush(points,3);
	//Color colors[] = {pSkin->clrBk1,pSkin->clrBk2,pSkin->clrBk3};
	//float pos[] = {0.0f,0.5f,1.0f};
	//pathbrush.SetInterpolationColors(colors,pos,3);
	//pathbrush.set
	Gdiplus::LinearGradientBrush lineGr(Point(rcClt.left,rcClt.bottom-m_iBottomHight-1),Point(rcClt.left,rcClt.bottom),
		pSkin->clrBk1,pSkin->clrBk2);
	Gdiplus::Pen penline(Color(255,22,22,22));

	gpDraw.FillRectangle(&lineGr,Rect(rcClt.left,rcClt.bottom-m_iBottomHight-2,rcClt.Width(),m_iBottomHight+2));
	//gpDraw.DrawLine(&penline,rcClt.left,rcClt.bottom-1,rcClt.right,rcClt.bottom-1);
	//SolidBrush sbr(pSkin->clrBk1);
	//gpDraw.FillRectangle(&sbr,Rect(rcClt.left,rcClt.bottom-m_iBottomHight,rcClt.Width(),m_iBottomHight));
	//CRect Rc1(rcClt.left+rcClt.Width()/2-4,rcClt.bottom-9,rcClt.left+rcClt.Width()/2+4,rcClt.bottom+1);
	CRect Rc1(rcClt.left+rcClt.Width()/2 - 42 -4,rcClt.bottom-m_iBottomHight+1,rcClt.left+rcClt.Width()/2-4,rcClt.bottom-1);
	CRect Rc2(rcClt.left+rcClt.Width()/2 + 4,rcClt.bottom-m_iBottomHight+1,rcClt.left+rcClt.Width()/2+42+4,rcClt.bottom-1);
	m_rcShowBottomHide = Rc1;
	m_rcShowTopHide = Rc2;

	DrawBottomHide();
	DrawTopHide();
	

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CDlgPrice::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//return CDialog::OnNcHitTest(point);
	CRect rect,rcBottomHide,rcTopHide;  
	GetWindowRect(&rect);  
	CRect rect1 = rect;  
	rect1.DeflateRect(10, 10, -10, -10);  
	rect1.NormalizeRect();  

	rcBottomHide = m_rcShowBottomHide;
	rcTopHide = m_rcShowTopHide;
	ClientToScreen(&rcBottomHide);
	ClientToScreen(&rcTopHide);
	if (rcTopHide.PtInRect(point))
	{
		DrawTopHide(1);
		return CWnd::OnNcHitTest(point);  
	}
	if (rcBottomHide.PtInRect(point))
	{
		DrawBottomHide(1);
		return CWnd::OnNcHitTest(point);  
	}
	DrawBottomHide();
	DrawTopHide();
	if (point.y >= rect.bottom-m_iBottomHight-2)  
			return HTBOTTOM;  
	else  
	{  
		return CWnd::OnNcHitTest(point);  
	}  
	return 0;  
}
void CDlgPrice::DrawTopHide(int iStat)
{
	CDateManage::Instance()->DrawTopHide(this,m_rcShowTopHide,iStat);
}
void CDlgPrice::DrawBottomHide(int iStat)
{
	CDateManage::Instance()->DrawBottomHide(this,m_rcShowBottomHide,iStat);
}
void CDlgPrice::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTTOP)  
	{         
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));  
	}  
	else if (nHitTest == HTBOTTOM)  
	{
		//SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);  
		//SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));  
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
		::SendMessage(GetParent()->GetSafeHwnd(),WM_WNDSIZECHANGE,0,0);
		Invalidate();
	}
	else if (nHitTest == HTLEFT)  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));  
	else if (nHitTest == HTRIGHT)  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));  
	else if (nHitTest == HTTOPLEFT)  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));  
	else if (nHitTest == HTTOPRIGHT)  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));  
	else if (nHitTest == HTBOTTOMLEFT)  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));  
	else if (nHitTest == HTBOTTOMRIGHT)  
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));  
	else if (nHitTest==HTCAPTION)  
		SendMessage(WM_SYSCOMMAND, SC_MOVE | 4, MAKELPARAM(point.x, point.y));  
}


void CDlgPrice::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnNcLButtonUp(nHitTest, point);
}


void CDlgPrice::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	BOOL bResh = FALSE;
	do 
	{
		if (m_rcShowBottomHide.PtInRect(point))
		{
			bResh = TRUE;
			GetParent()->PostMessage(WM_WNDREARRAY,1,0);
			break;
		}
		if (m_rcShowTopHide.PtInRect(point))
		{
			bResh = TRUE;
			GetParent()->PostMessage(WM_WNDREARRAY,2,0);
			break;
		}
	} while (0);
	
	if (bResh)
	{
		Invalidate();
	}
	CDialog::OnLButtonUp(nFlags, point);
}

LRESULT CDlgPrice::OnWndSizeChange(WPARAM wParam,LPARAM lParm)
{
	int iRet = 0;
	CRect rcPriceLst,rcPriceInfo,rcClt;
	GetClientRect(&rcClt);
	int cx = rcClt.Width();
	int cy = rcClt.Height();
	rcClt.InflateRect(0,0,0,-5);
	int iPriceLstWith = 0;
	if (wParam == 1)
	{
		m_DlgPriceLst.GetClientRect(&rcPriceLst);
		//rcPriceInfo = rcClt;
		//rcPriceInfo.left = rcPriceLst.right;
		//SetPriceInfoPackPos(rcPriceInfo);

		iPriceLstWith = rcPriceLst.Width();
		SetPriceLstPos(CRect(0,0,iPriceLstWith/*-1*/,cy-m_iBottomHight));
		SetPriceInfoPackPos(CRect(iPriceLstWith/*+1*/,0,cx,cy-m_iBottomHight));

		//m_fWndRatio = (iPriceLstWith*1.0)/rcClt.Width();
		m_iListWith = iPriceLstWith;
		CPublic::Instance()->WriteTrendChartRatio(m_iListWith);
	}
	else if (wParam == 2)
	{
		m_DlgPriceInfoPack.GetClientRect(&rcPriceInfo);
		//rcPriceLst = rcClt;
		//rcPriceLst.right = rcClt.right - rcPriceInfo.Width();
		//SetPriceLstPos(rcPriceLst);
		iPriceLstWith = cx - rcPriceInfo.Width();
		SetPriceLstPos(CRect(0,0,iPriceLstWith/*-1*/,cy-m_iBottomHight));
		SetPriceInfoPackPos(CRect(iPriceLstWith/*+1*/,0,cx,cy-m_iBottomHight));

		//m_fWndRatio = (iPriceLstWith*1.0)/rcClt.Width();
		m_iListWith = iPriceLstWith;
		CPublic::Instance()->WriteTrendChartRatio(m_iListWith);
	}
	m_iWndReArrayType = 0;
	m_DlgPriceLst.Invalidate();
	return iRet;
}

BOOL CDlgPrice::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CDlgPrice::OnWndReArray(WPARAM wPar,LPARAM lPar)
{
	int iType = int(wPar);
	do 
	{
		if (m_iWndReArrayType == 0)
		{
			m_iWndReArrayType = iType;
			ReArrayCtrl();
			break;
		}
		if (m_iWndReArrayType != iType)
		{
			m_iWndReArrayType = 0;
			ReArrayCtrl();
			break;
		}
	} while (0);
	return 1;
}
void CDlgPrice::ReArrayCtrl()
{
	CRect rectClient;
	GetClientRect(&rectClient);
	int iPriceLstWith = m_iListWith;//cx*m_fWndRatio;
	if (m_iWndReArrayType == 1)iPriceLstWith = 11;
	if (m_iWndReArrayType == 2)iPriceLstWith = rectClient.Width();
	//rectClient.InflateRect(0,0,-cx/2,-6);
	SetPriceLstPos(CRect(0,0,iPriceLstWith,rectClient.Height()-m_iBottomHight));

	SetPriceInfoPackPos(CRect(iPriceLstWith,0,rectClient.Width(),rectClient.Height()-m_iBottomHight));
	//SetPriceInfoPos(CRect(cx/2+2,0,cx,rectClient.bottom));
}
LRESULT CDlgPrice::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	m_DlgPriceLst.PostMessage(WM_SKINCHANGE,0,0);
	m_DlgPriceInfoPack.PostMessage(WM_SKINCHANGE,0,0);
	return 1;
}
LRESULT CDlgPrice::OnReLayout(WPARAM wPar,LPARAM lPar)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	m_iListWith = rectClient.Width()*0.5;
	m_iWndReArrayType = 0;
	ReArrayCtrl();
	return 1;
}
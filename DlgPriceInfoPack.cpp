// DlgPriceInfoPack.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPriceInfoPack.h"
#include "afxdialogex.h"


// CDlgPriceInfoPack dialog

IMPLEMENT_DYNAMIC(CDlgPriceInfoPack, CDialog)

CDlgPriceInfoPack::CDlgPriceInfoPack(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPriceInfoPack::IDD, pParent)
{

}

CDlgPriceInfoPack::~CDlgPriceInfoPack()
{
}

void CDlgPriceInfoPack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriceInfoPack, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
END_MESSAGE_MAP()


// CDlgPriceInfoPack message handlers


BOOL CDlgPriceInfoPack::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPriceInfoPack::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	SetPriceInfoPos(CRect(2,2,cx-2,cy-2));
	if (IsWindow(m_dlgTrendChart.GetSafeHwnd()))
	{
		m_dlgTrendChart.SetWndPos(CRect(2,2,cx-2,cy-2));
	}
}

void CDlgPriceInfoPack::InitCtrl()
{
	CRect rcClt;
	GetClientRect(&rcClt);

	if (CPublic::Instance()->IsTrendChart())
	{
		m_dlgTrendChart.Create(CDlgTrendChart::IDD,this);
		m_dlgTrendChart.ShowWindow(SW_SHOW);
	}
	else
	{
		m_DlgPriceInfo.Create(CDlgPriceInfo::IDD,this);
		SetPriceInfoPos(rcClt);
		m_DlgPriceInfo.ShowWindow(SW_SHOW);
	}
}

void CDlgPriceInfoPack::SetPriceInfoPos(CRect &rcWnd)
{
	if (IsWindow(m_DlgPriceInfo.GetSafeHwnd()))
	{
		m_DlgPriceInfo.SetWindowPos(NULL, rcWnd.left, rcWnd.top,
			rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}

BOOL CDlgPriceInfoPack::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClt;
	GetClientRect(&rcClt);
	Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	Graphics graphicsDraw(pDC->m_hDC);
	Graphics graphics(&cachBmp);
	CSkinAttribute_FrameLine * pSkin = (CSkinAttribute_FrameLine *)CSkinTransform::Instance()->GetSkinAttribute(3);
	CDateManage::Instance()->DrawBkRect(graphics,rcClt);
	Pen penBrd(pSkin->clrBkLineOut);
	graphics.DrawRectangle(&penBrd,Rect(rcClt.left,rcClt.top,rcClt.Width()-1,rcClt.Height()-1));
	Pen penBrd2(pSkin->clrBkLineIn);
	graphics.DrawRectangle(&penBrd2,Rect(rcClt.left+1,rcClt.top+1,rcClt.Width()-3,rcClt.Height()-3));
	graphicsDraw.DrawImage(&cachBmp,0,0/*rcClt.top*/);
	graphicsDraw.ReleaseHDC(pDC->m_hDC);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


LRESULT CDlgPriceInfoPack::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CRect rect;  
	//GetWindowRect(&rect);
	//if (point.x <= rect.left+3)  
	//	return HTLEFT;  
	//else  
	//{  
	//	return CWnd::OnNcHitTest(point);  
	//}  
	//return 0;  
	return CDialog::OnNcHitTest(point);
}


void CDlgPriceInfoPack::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//if (nHitTest == HTLEFT)  
	//{
	//	SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y)); 
	//	::SendMessage(GetParent()->GetSafeHwnd(),WM_WNDSIZECHANGE,2,0);
	//}
	//CDialog::OnNcLButtonDown(nHitTest, point);
}



BOOL CDlgPriceInfoPack::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CDlgPriceInfoPack::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	if (IsWindow(m_DlgPriceInfo.GetSafeHwnd()))m_DlgPriceInfo.PostMessage(WM_SKINCHANGE);
	if (IsWindow(m_dlgTrendChart.GetSafeHwnd()))m_dlgTrendChart.PostMessage(WM_SKINCHANGE);
	return 1;
}
// DlgPriceList.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPriceList.h"
#include "afxdialogex.h"


// CDlgPriceList dialog

IMPLEMENT_DYNAMIC(CDlgPriceList, CDialog)

CDlgPriceList::CDlgPriceList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPriceList::IDD, pParent)
{

}

CDlgPriceList::~CDlgPriceList()
{
}

void CDlgPriceList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriceList, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CDlgPriceList message handlers


void CDlgPriceList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rcClt(0,0,cx-2,cy);
	SetWndPos(rcClt);
}


BOOL CDlgPriceList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPriceList::InitCtrl()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE|LVS_LIST| WS_CLIPCHILDREN|WM_NOTIFY;;
	if (!m_PriceListWnd.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return ;      // 未能创建
	}

	CRect rcClt;
	GetClientRect(&rcClt);
	SetWndPos(rcClt);
	CPublic::Instance()->RegistWnd(DLG_PRICELST,this);
}

void CDlgPriceList::SetWndPos(CRect &rcWnd)
{
	if (IsWindow(m_PriceListWnd.GetSafeHwnd()))
	{
		m_PriceListWnd.SetWindowPos(NULL, rcWnd.left , rcWnd.top , rcWnd.Width(), rcWnd.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		m_PriceListWnd.SetFocus();
	}
}

LRESULT CDlgPriceList::OnDataMessage(WPARAM wParam,LPARAM lParm)
{
	int iRet = 1;
	m_PriceListWnd.UpdateData((char*)wParam,(int)lParm);
	return iRet;
}

BOOL CDlgPriceList::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//CRect rcClt;
	//GetClientRect(&rcClt);
	//pDC->FillSolidRect(&rcClt,RGB(255,0,0));
	//return TRUE;
	CDialog::OnEraseBkgnd(pDC);
	CRect rcClt;
	GetClientRect(&rcClt);
	CRect rcBorde(rcClt.right-2,rcClt.top,rcClt.right,rcClt.bottom);

	Color clrTop(255,211,211,211);
	Color clrMid(255,188,188,188);
	Graphics gpDraw(pDC->GetSafeHdc());
	Gdiplus::LinearGradientBrush   linGrBrush(Rect(rcBorde.left,rcBorde.top,rcBorde.Width(),
		rcBorde.Height()),clrTop,clrMid,(Gdiplus::REAL)(0));

	gpDraw.FillRectangle(&linGrBrush,Rect(rcBorde.left,rcBorde.top,rcBorde.Width(),rcBorde.Height()));
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


LRESULT CDlgPriceList::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;  
	GetWindowRect(&rect);
	if (point.x >= rect.right-3)  
		return HTRIGHT;  
	else  
	{  
		return CWnd::OnNcHitTest(point);  
	}  
	return 0;  
	//return CDialog::OnNcHitTest(point);
}


void CDlgPriceList::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTRIGHT)  
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y)); 
		::SendMessage(GetParent()->GetSafeHwnd(),WM_WNDSIZECHANGE,1,0);
	}
	//CDialog::OnNcLButtonDown(nHitTest, point);
}


void CDlgPriceList::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CRect rcClt;
	GetClientRect(&rcClt);
	if(NULL == pWnd) return ; //防止指针空	
	CPoint ptClient;
	ptClient = point;
	ScreenToClient(&ptClient);
	rcClt.InflateRect(-5,-5,-5,-5);
	if (!rcClt.PtInRect(ptClient))
	{
		return;
	}
	CMenu popMenu;
	m_PriceListWnd.RButtonDown(ptClient);
	popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
	CMenu* pMenu;
	pMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_REPORTTABLE);
	CLanguageInterface::Instance()->InitRightMenu(pMenu,POPSUBMENU_INDEX_REPORTTABLE);
	pMenu->CheckMenuItem(ID_QUOTE_SHOWHIGHLOW,m_PriceListWnd.GetShowHighLowStatus()?MF_CHECKED:MF_UNCHECKED);
	pMenu->CheckMenuItem(ID_QUOTE_SHOWUPDATETIME,m_PriceListWnd.GetShowUpdateTimeStatus()?MF_CHECKED:MF_UNCHECKED);
	pMenu->CheckMenuItem(ID_QUOTE_SHOWOTHERQUOTE,m_PriceListWnd.GetShowOtherSymbol()?MF_CHECKED:MF_UNCHECKED);
	UINT uiCmd;
	uiCmd = pMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN|TPM_RETURNCMD
		,point.x,point.y,AfxGetMainWnd()); //弹出菜单
	popMenu.DestroyMenu(); //销毁菜单

	if((uiCmd == ID_QUOTE_SHOWUPDATETIME))
	{
		m_PriceListWnd.ShowHideUpdateTime();
	}
	else if(uiCmd == ID_QUOTE_SHOWHIGHLOW)
	{
		m_PriceListWnd.ShowHideHighLow();
	}
	else if(uiCmd == ID_QUOTE_SHOWOTHERQUOTE)
	{
		m_PriceListWnd.ShowHideOtherSymbol();
	}
	else
	{
		SCodeInfo* pCode;
		pCode = m_PriceListWnd.GetCurSel();
		CPublic::Instance()->CallStockMenu(uiCmd,pCode);
	}
	m_PriceListWnd.RedrawWindow();
}


BOOL CDlgPriceList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

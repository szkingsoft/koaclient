// DlgPriceInfo.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPriceInfo.h"
#include "afxdialogex.h"


// CDlgPriceInfo dialog

IMPLEMENT_DYNAMIC(CDlgPriceInfo, CDialog)

CDlgPriceInfo::CDlgPriceInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPriceInfo::IDD, pParent)
{

}

CDlgPriceInfo::~CDlgPriceInfo()
{
}

void CDlgPriceInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriceInfo, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_WM_VSCROLL()
	ON_MESSAGE(WM_UPDATECTRL,OnUpdateCtrl)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
END_MESSAGE_MAP()


// CDlgPriceInfo message handlers


BOOL CDlgPriceInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPriceInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	SetPriceInfoWndPos(CRect(0,0,cx-20,cy));

	CRect rcParant;
	GetParent()->GetClientRect(&rcParant);
	SetScrollBarPos1(CRect(cx-20,0,cx,rcParant.Height()));
}

void CDlgPriceInfo::InitCtrl()
{

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle =  WS_CHILD | WS_VISIBLE|LVS_LIST| WS_CLIPCHILDREN|WM_NOTIFY;

	//if (!m_PriceInfoWnd.Create(dwViewStyle, rectDummy, this, 4))
	//{
	//	TRACE0("未能创建文件视图\n");
	//	return ;      // 未能创建
	//}
	//m_PriceInfoWnd.SetNotifyWnd(this->GetSafeHwnd());

	CRect rcClt;
	GetClientRect(&rcClt);
	SetPriceInfoWndPos(rcClt);
	CPublic::Instance()->RegistWnd(7,this);

	///////
	CPoint ptOrg(0,0);
	CRect rcVScroll(ptOrg.x,ptOrg.y,20,200);
	m_uVScrollBarCtrl.m_ListWnd=m_hWnd;

	m_uVScrollBarCtrl.CReateScrollBar(WS_CHILD | WS_VISIBLE | SBS_VERT,rcVScroll, m_hWnd,1234);
	m_uVScrollBarCtrl.SetWith(20);

	SetScrollBarPos1(CRect(rcClt.right-20,rcClt.top,rcClt.right,rcClt.bottom));
	m_uVScrollBarCtrl.ShowWindow(SW_HIDE);

	m_dlgPriceInfoWnd.Create(CDlgPriceInfoWnd::IDD,this);
	m_dlgPriceInfoWnd.ShowWindow(SW_SHOW);
	//m_PriceInfoWnd.ShowWindow(SW_HIDE);
}

void CDlgPriceInfo::SetScrollBarInfo2()
{
	if (!IsWindow(m_uVScrollBarCtrl.GetSafeHwnd()))
	{
		return;
	}
	CRect rcWnd,rcClt,rcParant;
	m_uVScrollBarCtrl.GetClientRect(&rcWnd);
	GetClientRect(&rcClt);
	GetParent()->GetClientRect(&rcParant);

	SCROLLINFO m_scrollInfo;
	m_uVScrollBarCtrl.GetScrollInfo(&m_scrollInfo,0);
	m_scrollInfo.cbSize = sizeof(SCROLLINFO);
	m_scrollInfo.fMask = SIF_ALL;
	m_scrollInfo.nMax = (m_dlgPriceInfoWnd.GetLastHight() <= rcParant.Height())?(rcParant.Height()):m_dlgPriceInfoWnd.GetLastHight();
	m_scrollInfo.nMin = 0;
	m_scrollInfo.nPage = rcParant.Height();
	//m_scrollInfo.nPos = 0;
	m_scrollInfo.nTrackPos = 0;

	m_uVScrollBarCtrl.SetScrollInfo(&m_scrollInfo);
}

void CDlgPriceInfo::SetScrollBarInfo1()
{
	//if (!IsWindow(m_uVScrollBarCtrl.GetSafeHwnd()))
	//{
	//	return;
	//}
	//CRect rcWnd,rcClt,rcParant;
	//m_uVScrollBarCtrl.GetClientRect(&rcWnd);
	//GetClientRect(&rcClt);
	//GetParent()->GetClientRect(&rcParant);

	//SCROLLINFO m_scrollInfo;
	//m_scrollInfo.cbSize = sizeof(SCROLLINFO);
	//m_scrollInfo.fMask = SIF_ALL;
	//m_scrollInfo.nMax = (m_PriceInfoWnd.GetLastHight() <= rcParant.Height())?(rcParant.Height()):m_PriceInfoWnd.GetLastHight();
	//m_scrollInfo.nMin = 0;
	//m_scrollInfo.nPage = rcParant.Height();
	//m_scrollInfo.nPos = 0;
	//m_scrollInfo.nTrackPos = 0;

	//m_uVScrollBarCtrl.SetScrollInfo(&m_scrollInfo);
}

void CDlgPriceInfo::SetScrollBarPos1(CRect &rcWnd)
{
	if (!IsWindow(m_uVScrollBarCtrl.GetSafeHwnd()))
	{
		return;

	}
	m_uVScrollBarCtrl.MoveWindow(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());
}

void CDlgPriceInfo::SetPriceInfoWndPos(CRect &rcWnd)
{
	if (IsWindow(m_dlgPriceInfoWnd.GetSafeHwnd()))
	{
		m_dlgPriceInfoWnd.SetWindowPos(NULL, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
		m_dlgPriceInfoWnd.SetFocus();
	}
	//if (IsWindow(m_PriceInfoWnd.GetSafeHwnd()))
	//{
	//	m_PriceInfoWnd.SetWindowPos(NULL, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	//	m_PriceInfoWnd.SetFocus();
	//}
}

LRESULT CDlgPriceInfo::OnDataMessage(WPARAM wParam,LPARAM lParm)
{
	int iRet = 1;
	//m_PriceInfoWnd.UpdateData((char*)wParam,(int)lParm);
	m_dlgPriceInfoWnd.UpdateData((char*)wParam,(int)lParm);
	return iRet;
}

void CDlgPriceInfo::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int iPos = 0;
	CRect rcWnd;
	GetClientRect(&rcWnd);
	////////////////
	if (!IsWindow(m_uVScrollBarCtrl.GetSafeHwnd())
		||!IsWindow(m_dlgPriceInfoWnd/*m_PriceInfoWnd*/.GetSafeHwnd()))
	{
		return;
	}
	SCROLLINFO SscrollInfo;
	m_uVScrollBarCtrl.GetScrollInfo(&SscrollInfo,0);
	iPos = SscrollInfo.nPos; 
	if (nSBCode == SB_PAGEDOWN)
	{
		if ((SscrollInfo.nPos + SscrollInfo.nPage) > SscrollInfo.nMax)
		{
			SscrollInfo.nPos = SscrollInfo.nMax;
		}
		else if ((SscrollInfo.nPos + SscrollInfo.nPage*2) <= SscrollInfo.nMax)
		{
			SscrollInfo.nPos += SscrollInfo.nPage;
		}
		else
		{
			SscrollInfo.nPos = SscrollInfo.nMax - SscrollInfo.nPage;
		}
		m_uVScrollBarCtrl.SetScrollInfo(&SscrollInfo);
		iPos = SscrollInfo.nPos;
	}
	else if (nSBCode == SB_PAGEUP)
	{
		int iSudPos = (SscrollInfo.nPos - SscrollInfo.nPage);
		if (iSudPos < SscrollInfo.nMin)
		{
			SscrollInfo.nPos = SscrollInfo.nMin;
		}
		else
		{
			SscrollInfo.nPos -= SscrollInfo.nPage;
		}
		m_uVScrollBarCtrl.SetScrollInfo(&SscrollInfo);
		iPos = SscrollInfo.nPos;
	}
	else if (nSBCode == SB_LINEUP)
	{
		if ((SscrollInfo.nPos - 1) < SscrollInfo.nMin)
		{
			SscrollInfo.nPos = SscrollInfo.nMin;
		}
		else
		{
			SscrollInfo.nPos -= 1;
		}
		m_uVScrollBarCtrl.SetScrollInfo(&SscrollInfo);
		iPos = SscrollInfo.nPos;
	}
	else if (nSBCode == SB_LINEDOWN)
	{
		if ((SscrollInfo.nPos + 1) > SscrollInfo.nMax)
		{
			SscrollInfo.nPos = SscrollInfo.nMax;
		}
		else
		{
			SscrollInfo.nPos += 1;
		}
		m_uVScrollBarCtrl.SetScrollInfo(&SscrollInfo);
		iPos = SscrollInfo.nPos;
	}
	else if (nSBCode == SB_BOTTOM || nSBCode == SB_TOP)
	{
	}
	else if (nSBCode == SB_ENDSCROLL)
	{
		iPos = SscrollInfo.nPos;
	}
	else if (nSBCode != SB_ENDSCROLL)
	{
		CRect rcParant;
		GetParent()->GetClientRect(&rcParant);

		SCROLLINFO m_scrollInfo;
		m_scrollInfo.cbSize = sizeof(SCROLLINFO);
		m_scrollInfo.fMask = SIF_ALL;
		m_scrollInfo.nMax = (m_dlgPriceInfoWnd/*m_PriceInfoWnd*/.GetLastHight() < (rcWnd.Height()))?(rcWnd.Height()):m_dlgPriceInfoWnd/*m_PriceInfoWnd*/.GetLastHight();
		m_scrollInfo.nMin = 0;
		m_scrollInfo.nPage = rcParant.Height()/*rcWnd.Height()*/;
		m_scrollInfo.nPos = ((nPos+2) < 4)? nPos: nPos;



		m_uVScrollBarCtrl.SetScrollInfo(&m_scrollInfo);
		iPos = nPos;
	}
	///////////////

	//int icount = m_pChatInfo->GetHight();
	//m_pChatInfo->MoveWindow(1,-iPos,rcWnd.Width()-21,iPos + rcWnd.Height()-100-m_iBottomHight);
	m_dlgPriceInfoWnd/*m_PriceInfoWnd*/.MoveWindow(0,-iPos,rcWnd.Width()-22,iPos+rcWnd.Height());
	//m_dlgPriceInfoWnd/*m_PriceInfoWnd*/.Invalidate();
	//CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CDlgPriceInfo::OnUpdateCtrl(WPARAM wParam,LPARAM lParm)
{
	int iRet = 0;
	CRect rcWnd,rcParant;
	GetWindowRect(&rcWnd);
	GetParent()->GetClientRect(&rcParant);
	/////////////
	int iOffHight = m_dlgPriceInfoWnd.GetLastHight();
	if (rcParant.Height() >= m_dlgPriceInfoWnd.GetLastHight())
	{
		iOffHight = rcParant.Height();
	}
	rcWnd.bottom = rcWnd.top + iOffHight;
	GetParent()->ScreenToClient(&rcWnd);
	//MoveWindow(&rcWnd);

	SetScrollBarInfo2();
	//SetScrollBarInfo1();

	//int iOffHight = m_PriceInfoWnd.GetLastHight();
	//if (rcParant.Height() >= m_PriceInfoWnd.GetLastHight())
	//{
	//	iOffHight = rcParant.Height();
	//}
	//rcWnd.bottom = rcWnd.top + iOffHight;
	//GetParent()->ScreenToClient(&rcWnd);
	//MoveWindow(&rcWnd);

	//SetScrollBarInfo1();
	/////////////////////
	return iRet;
}


void CDlgPriceInfo::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CDlgPriceInfo::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	int iPos = 0;
	CRect rcWnd;
	GetClientRect(&rcWnd);

	if (zDelta > 0)
	{
		SCROLLINFO SscrollInfo;
		m_uVScrollBarCtrl.GetScrollInfo(&SscrollInfo,0);
		if ((SscrollInfo.nPos - 10) < SscrollInfo.nMin)
		{
			SscrollInfo.nPos = SscrollInfo.nMin;
		}
		else
		{
			SscrollInfo.nPos -= 10;
		}
		m_uVScrollBarCtrl.SetScrollInfo(&SscrollInfo);
		iPos = SscrollInfo.nPos;
	}
	else if (zDelta < 0)
	{
		SCROLLINFO SscrollInfo;
		m_uVScrollBarCtrl.GetScrollInfo(&SscrollInfo,0);
		if (m_uVScrollBarCtrl.IsBotton())
		{
		}
		else if ((SscrollInfo.nPos + 10) > SscrollInfo.nMax)
		{
			SscrollInfo.nPos = SscrollInfo.nMax;
		}
		else
		{
			SscrollInfo.nPos += 10;
		}
		m_uVScrollBarCtrl.SetScrollInfo(&SscrollInfo);
		iPos = SscrollInfo.nPos;
	}
	else
	{
	}
	m_uVScrollBarCtrl.Invalidate();
	m_dlgPriceInfoWnd/*m_PriceInfoWnd*/.MoveWindow(0,-iPos,rcWnd.Width()-22,iPos+rcWnd.Height());

	return 1;
	//return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CDlgPriceInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcClt;
	GetClientRect(&rcClt);
	//rcClt.InflateRect(-2,-2,-2,-2);
	
	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


void CDlgPriceInfo::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	//CRect rcClt;
	//GetClientRect(&rcClt);
	//if(NULL == pWnd) return ; //防止指针空	
	//CPoint ptClient;
	//ptClient = point;
	//ScreenToClient(&ptClient);
	//rcClt.InflateRect(-5,-5,-30,-5);
	//if (!rcClt.PtInRect(ptClient))
	//{
	//	return;
	//}
	//CMenu popMenu;
	//if(NULL == pWnd) return ; //防止指针空	
	//popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
	//CMenu* pMenu;
	//pMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_REPORTTABLE);
	//CLanguageInterface::Instance()->InitRightMenu(pMenu,POPSUBMENU_INDEX_REPORTTABLE);
	//UINT uiCmd;
	//uiCmd = pMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN|TPM_RETURNCMD
	//	,point.x,point.y,AfxGetMainWnd()); //弹出菜单
	//popMenu.DestroyMenu(); //销毁菜单

	//SCodeInfo* pCode;
	////pCode = m_PriceInfoWnd.GetCurSel();
	//CPublic::Instance()->CallStockMenu(uiCmd,pCode);
}


BOOL CDlgPriceInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CDlgPriceInfo::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	if (IsWindow(m_uVScrollBarCtrl.GetSafeHwnd()))m_uVScrollBarCtrl.Invalidate();
	m_dlgPriceInfoWnd.PostMessage(WM_SKINCHANGE);
	return 1;
}
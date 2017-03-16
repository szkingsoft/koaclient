// DlgSpotMarket.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgSpotMarket.h"


// CDlgSpotMarket 对话框

IMPLEMENT_DYNAMIC(CDlgSpotMarket, CDialog)

CDlgSpotMarket::CDlgSpotMarket(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpotMarket::IDD, pParent)
{

}

CDlgSpotMarket::~CDlgSpotMarket()
{
}

void CDlgSpotMarket::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSpotMarket, CDialog)
	ON_WM_SIZE()
//	ON_WM_SETCURSOR()
ON_WM_ERASEBKGND()
ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
ON_MESSAGE(WM_RETLST,OnRetLst)
ON_COMMAND(ID_NORMALORDER, &CDlgSpotMarket::OnNormalOrder)
ON_COMMAND_RANGE(0,5,OnSysWndNotify)
ON_COMMAND_RANGE(ID_ANALYSIST_BEGIN,ID_ANALYSIST_END,OnComRange)
ON_MESSAGE(WM_SHOWURL,OnShowUrl)
ON_MESSAGE(WM_NOTIFY_KOA_SYS,OnNotifyKOASys)
ON_MESSAGE(WM_WNDSIZECHANGE,OnWndSizeChange)
END_MESSAGE_MAP()


// CDlgSpotMarket 消息处理程序


void CDlgSpotMarket::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//int iPriceHight = cy/**0.8*/;
	//int iLstWith = 1;
	//if (IsWindow(m_dlgSpotInfo.GetSafeHwnd()))
	//{
	//	m_dlgSpotInfo.MoveWindow(0,0,iLstWith,iPriceHight);
	//	//m_dlgSpotInfo.MoveWindow(0,0,100,iPriceHight);
	//}
	//if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))
	//{
	//	m_dlgTrendChart_D.MoveWindow(iLstWith,0,cx-iLstWith,iPriceHight);
	//}

	//CRect rcTrade(0,cy/2-20/*iPriceHight*/,cx,cy);
	//CRect rcTrade1(0,0/*iPriceHight*/,0,0);
	//SetTradeWndPos(rcTrade);

	ReArrayWnd();
}

LRESULT CDlgSpotMarket::OnWndSizeChange(WPARAM wPar,LPARAM lPar)
{
	CRect rcClt,rcQuote;
	GetClientRect(&rcClt);
	m_dlgTrendChart_D.GetClientRect(&rcQuote);

	int iPriceHight = rcQuote.Height();
	if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))
	{
		m_dlgTrendChart_D.MoveWindow(0,0,rcClt.Width(),iPriceHight);
		//m_dlgTrendChart_D.ShowWindow(SW_HIDE);
	}

	CRect rcTrade(0,iPriceHight,rcClt.Width(),rcClt.Height());
	SetTradeWndPos(rcTrade);
	if (IsWindow(m_dlgShowUrl.GetSafeHwnd()))
	{
		m_dlgShowUrl.MoveWindow(rcTrade);
	}

	if (IsWindow(m_dlgTradeInfo.GetSafeHwnd())&&IsWindow(m_dlgShowUrl.GetSafeHwnd()))
	{
		if (!m_dlgShowUrl.IsWindowVisible())
		{
			m_dlgTradeInfo.ShowWindow(SW_SHOW);
		}
	}
	return 1;
}

void CDlgSpotMarket::ReArrayWnd(int iMode,int iType)
{
	CRect rcClt;
	GetClientRect(&rcClt);
	if (IsWindow(m_dlgSpotInfo.GetSafeHwnd()))
	{
		m_dlgSpotInfo.MoveWindow(0,0,0,0);
		//m_dlgSpotInfo.MoveWindow(0,0,100,iPriceHight);
	}

	int iPriceHight = rcClt.Height();
	if (iMode == 1||iMode == 4)
	{
		if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))
		{
			m_dlgTrendChart_D.MoveWindow(0,0,rcClt.Width(),iPriceHight);
			m_dlgTrendChart_D.ShowWindow(SW_SHOW);
		}

		CRect rcTrade(0,iPriceHight,rcClt.Width(),rcClt.Height());
		SetTradeWndPos(rcTrade);
		if (IsWindow(m_dlgShowUrl.GetSafeHwnd()))
		{
			m_dlgShowUrl.MoveWindow(rcTrade);
		}

		if (IsWindow(m_dlgTradeInfo.GetSafeHwnd())&&IsWindow(m_dlgShowUrl.GetSafeHwnd()))
		{
			m_dlgTradeInfo.ShowWindow(SW_HIDE);
			m_dlgShowUrl.ShowWindow(SW_HIDE);
		}
		
	}
	else if (iMode == 2)
	{
		iPriceHight = 0;
		if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))
		{
			m_dlgTrendChart_D.MoveWindow(0,0,rcClt.Width(),iPriceHight);
			m_dlgTrendChart_D.ShowWindow(SW_HIDE);
		}

		CRect rcTrade(0,iPriceHight,rcClt.Width(),rcClt.Height());
		SetTradeWndPos(rcTrade);
		if (IsWindow(m_dlgShowUrl.GetSafeHwnd()))
		{
			m_dlgShowUrl.MoveWindow(rcTrade);
		}

		if (IsWindow(m_dlgTradeInfo.GetSafeHwnd())&&IsWindow(m_dlgShowUrl.GetSafeHwnd()))
		{
			m_dlgTradeInfo.ShowWindow(iType==0?SW_SHOW:SW_HIDE);
			m_dlgShowUrl.ShowWindow(iType==0?SW_HIDE:SW_SHOW);
		}
	}
	else
	{
		iPriceHight /= 2;
		if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))
		{
			m_dlgTrendChart_D.MoveWindow(0,0,rcClt.Width(),iPriceHight);
			m_dlgTrendChart_D.ShowWindow(SW_SHOW);
		}

		CRect rcTrade(0,iPriceHight,rcClt.Width(),rcClt.Height());
		SetTradeWndPos(rcTrade);
		if (IsWindow(m_dlgShowUrl.GetSafeHwnd()))
		{
			m_dlgShowUrl.MoveWindow(rcTrade);
		}
		if (IsWindow(m_dlgTradeInfo.GetSafeHwnd())&&IsWindow(m_dlgShowUrl.GetSafeHwnd()))
		{
			m_dlgTradeInfo.ShowWindow(iType==0?SW_SHOW:SW_HIDE);
			m_dlgShowUrl.ShowWindow(iType==0?SW_HIDE:SW_SHOW);
		}
	}
	if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))
	{
		m_dlgTrendChart_D.Invalidate();
	}
}

BOOL CDlgSpotMarket::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSpotMarket::InitCtrl()
{
	m_dlgSpotInfo.Create(CDlgSpotInfo::IDD,this);
	m_dlgSpotInfo.ShowWindow(SW_SHOW);

	m_dlgTrendChart_D.Create(CDlgTrendChart_D::IDD,this);
	m_dlgTrendChart_D.ShowWindow(SW_SHOW);

	m_dlgShowUrl.Create(CDlgShowUrl::IDD,this);
	m_dlgShowUrl.ShowWindow(SW_HIDE);
	InitTradeWnd();
}

void CDlgSpotMarket::InitTradeWnd()
{
	m_dlgTradeInfo.SetIndex(1);
	m_dlgTradeInfo.Create(CDlgTradeInfo::IDD,this);
	m_dlgTradeInfo.ShowWindow(SW_HIDE);

	//m_dlgTradeInfo_Pop.Create(CDlgTradeInfoPop::IDD,this);
	//m_dlgTradeInfo_Pop.ShowWindow(SW_HIDE);
}

void CDlgSpotMarket::SetTradeWndPos(CRect &rcWnd)
{
	//CPublic::Instance()->GetTradeDll()->m_pMoveDlg(rcWnd);
	if (!IsWindow(m_dlgTradeInfo.GetSafeHwnd())) return;

	m_dlgTradeInfo.MoveWindow(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());
}

BOOL CDlgSpotMarket::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


//BOOL CDlgSpotMarket::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	SetCursor(LoadCursor(NULL,IDC_SIZEWE)); 
//	return TRUE;;
//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
//}


BOOL CDlgSpotMarket::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	
	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
LRESULT CDlgSpotMarket::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	if (IsWindow(m_dlgSpotInfo.GetSafeHwnd()))m_dlgSpotInfo.PostMessage(WM_SKINCHANGE);
	if (IsWindow(m_dlgTrendChart_D.GetSafeHwnd()))m_dlgTrendChart_D.PostMessage(WM_SKINCHANGE);
	return 1;
}
LRESULT CDlgSpotMarket::OnRetLst(WPARAM wPar,LPARAM lPar)
{
	//if (wPar == 0)
	//{
	//	m_dlgSpotInfo.ShowWindow(SW_SHOW);
	//	m_dlgTradeInfo.ShowWindow(SW_SHOW);
	//	//m_dlgTrendChart_D.ShowWindow(SW_HIDE);
	//}
	//else
	//{
	//	m_dlgSpotInfo.ShowWindow(SW_HIDE);
	//	m_dlgTradeInfo.ShowWindow(SW_HIDE);
	//	//m_dlgTrendChart_D.ShowWindow(SW_SHOW);
	//}
	return 1;
}

void CDlgSpotMarket::OnNormalOrder()
{
	//if(IsWindow(m_dlgTradeInfo_Pop.GetSafeHwnd()))
	//{
	//	m_dlgTradeInfo_Pop.ShowWindow(SW_SHOW);
	//}
	//////////////
	CRect rcClt;
	GetClientRect(&rcClt);
	int iPriceHight;// = rcClt.Height()/2;
	if (!m_dlgTradeInfo.IsWindowVisible())
	{
		ReArrayWnd();
	}
	else
	{
		//ReArrayWnd(4);
	}
}
void CDlgSpotMarket::OnSysWndNotify(UINT uiID)
{
	ReArrayWnd(uiID);
}
LRESULT CDlgSpotMarket::OnNotifyKOASys(WPARAM wPar,LPARAM lPar)
{
	ReArrayWnd(wPar,lPar);
	return 1;
}
void CDlgSpotMarket::OnComRange(UINT Iid)
{
	switch(Iid)
	{
	case ID_TECHANALY_1MINLINE://1分钟线
	case ID_TECHANALY_5MINLINE: //5分钟线
	case ID_TECHANALY_10MINLINE: //10分钟线
	case ID_TECHANALY_15MINLINE: //15分钟线
	case ID_TECHANALY_30MINLINE: //30分钟线
	case ID_TECHANALY_60MINLINE: //60分钟线
	case ID_TECHANALY_DAYLINE: //日线
	case ID_TECHANALY_WEEKLINE: //周线
	case ID_TECHANALY_MONLINE: //月线

		{
			m_dlgTrendChart_D.OnComRange(Iid);
		}
		break;	
	default:
		{
			if((Iid >=ID_DRAWLINE_BEGINTAG_Z)&&(Iid <=ID_DRAWLINE_ENDTAG_Z))
			{
				m_dlgTrendChart_D.OnComRange(Iid);
			}
		}
		break;
	}
}

LRESULT CDlgSpotMarket::OnShowUrl(WPARAM wPar,LPARAM lPar)
{
	ReArrayWnd(0,1);
	TCHAR *pChar = (TCHAR *)lPar;
	if (pChar)
	{
		m_dlgShowUrl.ShowUrl(pChar);
		delete pChar;
		pChar = NULL;
	}	

	return 0;
}
void CDlgSpotMarket::SetDrawLineCallBcakFun()
{
	CDlgTrendChart_D * pWnd = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	if(pWnd)pWnd->SetDrawLineCallBcakFun();
}
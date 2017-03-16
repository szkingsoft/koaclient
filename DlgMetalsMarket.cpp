// DlgMetalsMarket.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgMetalsMarket.h"
#include "afxdialogex.h"


// CDlgMetalsMarket dialog

IMPLEMENT_DYNAMIC(CDlgMetalsMarket, CDialog)

CDlgMetalsMarket::CDlgMetalsMarket(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMetalsMarket::IDD, pParent)
{
	m_fWndRatio = 0.5;
	m_iPriceHight = CPublic::Instance()->GetPriceHight();

	m_iWndReArrayType = 0;
}

CDlgMetalsMarket::~CDlgMetalsMarket()
{
}

void CDlgMetalsMarket::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_TAB, m_statTab);
}


BEGIN_MESSAGE_MAP(CDlgMetalsMarket, CDialog)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_MESSAGE(WM_WNDSIZECHANGE,OnWndSizeChange)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_WNDREARRAY,OnWndReArray)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
	ON_MESSAGE(WM_RELAYOUT,OnReLayout)
END_MESSAGE_MAP()


// CDlgMetalsMarket message handlers


BOOL CDlgMetalsMarket::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here


	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMetalsMarket::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	ReArrayCtrl();
}


void CDlgMetalsMarket::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: Add your message handler code here
}

void CDlgMetalsMarket::InitCtrl()
{
	InitTradeWnd();

	m_DlgPrice.Create(CDlgPrice::IDD,this);
	m_DlgPrice.ShowWindow(SW_SHOW);

	//m_dlgTrendChart.Create(CDlgTrendChart::IDD,this);
	//m_dlgTrendChart.ShowWindow(SW_SHOW);
	//USES_CONVERSION;
	//m_statTab.MoveWindow(0,50,25,600);
	//WCHAR *pChar1 = new WCHAR[50];
	//memset(pChar1,0,50*2);
	//CString strTmp = CLanguageInterface::Instance()->GetStr(30002);
	//if (strTmp.IsEmpty())
	//{
	//	strTmp = _T("贵金属市场 ");
	//}
	//memcpy(pChar1,T2W(strTmp.GetBuffer(0))/*T2W(_T("贵金属市场 "))*/,(strTmp.GetLength()+1)*sizeof(TCHAR));
	//WCHAR *pChar2 = new WCHAR[50];
	//memset(pChar2,0,50*2);
	//strTmp = CLanguageInterface::Instance()->GetStr(30003);
	//if (strTmp.IsEmpty())
	//{
	//	strTmp = _T("连续现货市场");
	//}
	//memcpy(pChar2,T2W(strTmp.GetBuffer(0)),(strTmp.GetLength()+1)*sizeof(TCHAR));
	////WCHAR *p1122 = 
	//WCHAR *pChar3 = new WCHAR[50];
	//memset(pChar3,0,50*2);
	//strTmp = CLanguageInterface::Instance()->GetStr(30004);
	//if (strTmp.IsEmpty())
	//{
	//	strTmp = _T("  仓库  ");
	//}
	//memcpy(pChar3,T2W(strTmp.GetBuffer(0)),(strTmp.GetLength()+1)*sizeof(TCHAR));
	//WCHAR *pChar4 = new WCHAR[50];
	//memset(pChar4,0,50*2);
	//strTmp = CLanguageInterface::Instance()->GetStr(30005);
	//if (strTmp.IsEmpty())
	//{
	//	strTmp = _T("资金与公告");
	//}
	//memcpy(pChar4,T2W(strTmp.GetBuffer(0)),(strTmp.GetLength()+1)*sizeof(TCHAR));
	//BTNLIST BtnList[4] = {{IDB_TAB_ONE,0,(pChar1),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,1},
	//{IDB_TAB_TOW,0,(pChar2),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,1},
	//{IDB_TAB_THREE,0,(pChar3),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,1},
	//{IDB_TAB_FOUR,0,(pChar4),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,1}};

	//m_statTab.InitTab(BtnList,4);
	//m_statTab.SetArrayType(2);	
	//m_statTab.SetSelTab(IDB_TAB_ONE);
}

void CDlgMetalsMarket::InitTradeWnd()
{
	m_dlgTradeInfo.SetIndex(0);
	m_dlgTradeInfo.Create(CDlgTradeInfo::IDD,this);
	m_dlgTradeInfo.ShowWindow(SW_SHOW);
	//m_hTradeWnd = CPublic::Instance()->GetTradeDll()->m_pInitDlog(GetSafeHwnd());//对话框
	//if (IsWindow(m_hTradeWnd))
	//{
	//	CRect clientrect;
	//	GetClientRect(clientrect);
	//	SetTradeWndPos(clientrect);
	//}
}

void CDlgMetalsMarket::SetTradeWndPos(CRect &rcWnd)
{
	//CPublic::Instance()->GetTradeDll()->m_pMoveDlg(rcWnd);
	if (!IsWindow(m_dlgTradeInfo.GetSafeHwnd())) return;

	m_dlgTradeInfo.MoveWindow(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());
	//m_dlgTradeInfo.ShowWindow(SW_SHOW);
}

LRESULT CDlgMetalsMarket::OnWndSizeChange(WPARAM wParam,LPARAM lParm)
{
	int iRet = 0;
	CRect rcTrade,rcPrice,rcClt;
	GetClientRect(&rcClt);
	m_DlgPrice.GetClientRect(&rcPrice);
	/////
	//////
	rcTrade = rcClt;
	rcTrade.top = rcPrice.bottom;
	SetTradeWndPos(rcTrade);
	//m_fWndRatio = (rcPrice.Height()*1.0)/rcClt.Height();
	m_iPriceHight = rcPrice.Height();
	CPublic::Instance()->WritePriceHight(m_iPriceHight);
	m_iWndReArrayType = 0;
	return iRet;
}

BOOL CDlgMetalsMarket::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CDlgMetalsMarket::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);

	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
LRESULT CDlgMetalsMarket::OnWndReArray(WPARAM wPar,LPARAM lPar)
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
void CDlgMetalsMarket::ReArrayCtrl()
{
	CRect rcWnd;
	GetClientRect(&rcWnd);
	int iPriceHight = m_iPriceHight;//cy*m_fWndRatio;
	if (m_iWndReArrayType == 1)iPriceHight = rcWnd.Height();
	if (m_iWndReArrayType == 2)iPriceHight = 11;

	int iTabWith = 0/*22*/;
	int iTabMinHight = 500;
	//if (IsWindow(m_statTab.GetSafeHwnd()))
	//{
	//	int iTop = 0;
	//	m_statTab.MoveWindow(0,iTop,iTabWith,iTabMinHight);
	//	if (!m_statTab.IsWindowVisible())iTabWith = 0;
	//	//if (m_iClientType != 2)iTabWith = 0;
	//}

	CRect rcTrade(iTabWith,iPriceHight,rcWnd.Width()-iTabWith,rcWnd.Height());
	SetTradeWndPos(rcTrade);

	CRect rcPrice(iTabWith,0,rcWnd.Width()-iTabWith,iPriceHight);
	if (m_DlgPrice.GetSafeHwnd())
	{
		m_DlgPrice.SetWndPos(rcPrice);
		//m_dlgTrendChart.SetWndPos(rcPrice);
	}
}
LRESULT CDlgMetalsMarket::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	m_DlgPrice.PostMessage(WM_SKINCHANGE,0,0);
	return 1;
}
LRESULT CDlgMetalsMarket::OnReLayout(WPARAM wPar,LPARAM lPar)
{
	if (m_DlgPrice.GetSafeHwnd())
	{
		m_DlgPrice.PostMessage(WM_RELAYOUT);
	}
	CRect rcWnd;
	GetClientRect(&rcWnd);
	m_iPriceHight = rcWnd.Height()*0.5;
	m_iWndReArrayType = 0;
	ReArrayCtrl();
	return 1;
}
void CDlgMetalsMarket::SetDrawLineCallBcakFun()
{
	CDlgTrendChart* pWnd = (CDlgTrendChart*)(CPublic::Instance()->GetView(10));
	if (pWnd)pWnd->SetDrawLineCallBcakFun();
}

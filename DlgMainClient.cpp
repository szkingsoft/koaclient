// DlgMainClient.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgMainClient.h"
#include "afxdialogex.h"



#define IDB_TAB_ONE        (WM_USER + 211)
#define IDB_TAB_TOW        (WM_USER + 212)
#define IDB_TAB_THREE      (WM_USER + 213)
#define IDB_TAB_FOUR       (WM_USER + 214)
// CDlgMainClient dialog

IMPLEMENT_DYNAMIC(CDlgMainClient, CDialog)

CDlgMainClient::CDlgMainClient(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMainClient::IDD, pParent)
{
	m_iClientType = 0;
	m_iFundPos = 0;
}

CDlgMainClient::~CDlgMainClient()
{
}

void CDlgMainClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_TAB, m_statTab);
	DDX_Control(pDX, IDC_BTN_FUND, m_BtnFund);
}


BEGIN_MESSAGE_MAP(CDlgMainClient, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_DATEMANAGE,OnDateManage)
	ON_COMMAND(IDB_TAB_ONE,OnTabOne)
	ON_COMMAND(IDB_TAB_TOW,OnTabTow)
	ON_COMMAND(IDB_TAB_THREE,OnTabThree)
	ON_COMMAND(IDB_TAB_FOUR,OnTabFour)
	ON_WM_SIZE()
	ON_MESSAGE(WM_NOTIFY_LOGIN_SUC,OnLoginSuccess)
//	ON_WM_SETCURSOR()
    ON_MESSAGE(M_TRADENOTIFY,OnTradeNotify)
	ON_MESSAGE(WM_OPENDRAWTOOL,OnOpenDrawTool)
	ON_MESSAGE(WM_UPDATEDRAWTOOL,OnUpdateDrawTool)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
	ON_MESSAGE(WM_RELAYOUT,OnReLayout)
	ON_MESSAGE(WM_NOTIFY_QUOTE_SYS,OnNotifyQuoteSys)
	ON_MESSAGE(WM_NOTIFY_KOA_SYS,OnNotifyKOASys)
ON_WM_ERASEBKGND()
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMainClient message handlers

#include "KOAClient.h"
void CDlgMainClient::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox(_T("OnLButtonDown"));
	static int i=0;
	i++;
	//::SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_SHOWTOOLBAR,i%2,0);
	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CDlgMainClient::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CPublic::Instance()->GetTradeDll()->m_pRegDateManageWnd(this->GetSafeHwnd(),WM_DATEMANAGE);
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMainClient::InitCtrl()
{
	CRect rcClt;
	GetClientRect(&rcClt);
	CPublic::Instance()->RegistWnd(11,this,2);
	USES_CONVERSION;
	m_statTab.MoveWindow(0,50,25,600);
	WCHAR *pChar1 = new WCHAR[50];
	memset(pChar1,0,50*2);
	CString strTmp = CLanguageInterface::Instance()->GetStr(30002);
	if (strTmp.IsEmpty())
	{
		strTmp = _T("贵金属市场 ");
	}
	memcpy(pChar1,T2W(strTmp.GetBuffer(0))/*T2W(_T("贵金属市场 "))*/,(strTmp.GetLength()+1)*sizeof(TCHAR));
	WCHAR *pChar2 = new WCHAR[50];
	memset(pChar2,0,50*2);
	strTmp = CLanguageInterface::Instance()->GetStr(30003);
	if (strTmp.IsEmpty())
	{
		strTmp = _T("连续现货市场");
	}
	memcpy(pChar2,T2W(strTmp.GetBuffer(0)),(strTmp.GetLength()+1)*sizeof(TCHAR));
	//WCHAR *p1122 = 
	WCHAR *pChar3 = new WCHAR[50];
	memset(pChar3,0,50*2);
	strTmp = CLanguageInterface::Instance()->GetStr(30004);
	if (strTmp.IsEmpty())
	{
		strTmp = _T("  仓库  ");
	}
	memcpy(pChar3,T2W(strTmp.GetBuffer(0)),(strTmp.GetLength()+1)*sizeof(TCHAR));
	WCHAR *pChar4 = new WCHAR[50];
	memset(pChar4,0,50*2);
	strTmp = CLanguageInterface::Instance()->GetStr(30005);
	if (strTmp.IsEmpty())
	{
		strTmp = _T("资金与公告");
	}
	memcpy(pChar4,T2W(strTmp.GetBuffer(0)),(strTmp.GetLength()+1)*sizeof(TCHAR));
	BTNLIST BtnList[4] = {{IDB_TAB_ONE,0,(pChar1),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,0},
	{IDB_TAB_TOW,0,(pChar2),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,0},
	{IDB_TAB_THREE,0,(pChar3),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,0},
	{IDB_TAB_FOUR,0,(pChar4),{0,0,0,0},Color::MakeARGB(255,244,244,244),15,NULL,0,1,1}};

	m_statTab.InitTab(BtnList,4);
	m_statTab.SetArrayType(2);	

	m_BtnFund.MoveWindow(0,0,1080,50);
	m_BtnFund.ShowWindow(SW_SHOW);

	FUNDINFO stcFund;
	stcFund.Init();
	SetFundCtrlDate(stcFund);

	m_DlgTopToolbar.Create(CDlgTopToolbar::IDD,this);
	m_DlgTopToolbar.ShowWindow(SW_SHOW);

	m_dlgStatus.Create(CDlgStatus::IDD,this);
	m_dlgStatus.ShowWindow(SW_SHOW);

	m_DlgMetalsMarket.Create(CDlgMetalsMarket::IDD,this);
	m_DlgMetalsMarket.ShowWindow(SW_SHOW);

	m_dlgSpotMarket.Create(CDlgSpotMarket::IDD,this);
	m_dlgSpotMarket.ShowWindow(SW_HIDE);
	
	m_dlgBinManagePack.Create(CDlgBinManagePack::IDD,this);
	m_dlgBinManagePack.ShowWindow(SW_HIDE);

	m_dlgFundInfoPack.Create(CDlgBinManagePack::IDD,this);
	m_dlgFundInfoPack.ShowWindow(SW_HIDE);

	m_dlgDrawTool.Create(CDlgDrawTool::IDD,this);
	m_dlgDrawTool.ShowWindow(SW_HIDE);

	m_dlgWarn.Create(CDlgWarnInfoEx::IDD,this);
	m_dlgWarn.ShowWindow(SW_HIDE);

	AccessCtrl();
	//else
	//{
	//	m_statTab.SetSelTab(IDB_TAB_ONE);
	//	OnTabOne();
	//	m_statTab.ShowWindow(SW_HIDE);
	//}
	//m_statTab.ShowWindow(SW_HIDE);
	//m_BtnFund.ShowWindow(SW_HIDE);
	m_iFundPos = CPublic::Instance()->GetFundPos();
	ReArrayCtrl();
}
void CDlgMainClient::ResetCtrl()
{
	m_statTab.VisableItem(IDB_TAB_ONE,FALSE);
	m_statTab.VisableItem(IDB_TAB_TOW,FALSE);
	m_statTab.VisableItem(IDB_TAB_THREE,FALSE);
	//m_statTab.VisableItem(IDB_TAB_THREE,FALSE);
}
void CDlgMainClient::AccessCtrl()
{
	ResetCtrl();
	m_iClientType = CPublic::Instance()->GetClientType();
	m_statTab.SetSelTab(IDB_TAB_FOUR);
	if (m_iClientType & 0x0010)
	{
		m_statTab.VisableItem(IDB_TAB_THREE,TRUE);
		m_statTab.SetSelTab(IDB_TAB_THREE);
	}
	if (m_iClientType & 0x0100)
	{
		//m_statTab.SetSelTab(IDB_TAB_TOW);
		//OnTabTow();
		//m_statTab.ShowWindow(SW_HIDE);
		m_statTab.VisableItem(IDB_TAB_TOW,TRUE);
		m_statTab.SetSelTab(IDB_TAB_TOW);
	}
	if (m_iClientType & 0x1000)
	{
		//m_statTab.SetSelTab(IDB_TAB_ONE);
		//OnTabOne();
		//m_statTab.ShowWindow(SW_SHOW);
		m_statTab.VisableItem(IDB_TAB_ONE,TRUE);
		m_statTab.SetSelTab(IDB_TAB_ONE);
	}
	if (m_iClientType == 0x1000)
	{
		m_statTab.VisableItem(IDB_TAB_FOUR,FALSE);
		m_statTab.ShowWindow(SW_HIDE);
	}
	ShowTab();
}
LRESULT CDlgMainClient::OnDateManage(WPARAM wPar,LPARAM lPar)
{
	//AfxMessageBox(_T("wwwwww"));
	pFUNDINFO pFund = (pFUNDINFO)lPar;
	if (pFund)
	{
		FUNDINFO stcFund = *pFund;
		//delete pFund;
		SetFundCtrlDate(stcFund);
		if (stcFund.bRefreshSGRN)
		{
			CPublic::Instance()->SendUserSGRNChangeMsg(TRUE);
		}
	}
	
	return 1;
}

void CDlgMainClient::SetFundCtrlDate(FUNDINFO & stcFund)
{
	CSkinAttribute_Fund *pSkin = (CSkinAttribute_Fund *)CSkinTransform::Instance()->GetSkinAttribute(4);
	RECTANGLEINFO stcFundDate[9];
	int i=0;
	CString strTmp;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1221,1),stcFund.dTodayBalance); //本日结余 
	stcFundDate[i].strText = strTmp;
	//stcFundDate[i].clrText = clrText;

	i++;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1224,1)/*_T("平仓盈亏")*/,
		stcFund.dPingcangYK);
	if (stcFund.dPingcangYK > 0)
	{
		stcFundDate[i].clrText = pSkin->clrTextUp;//Color::MakeARGB(255,255,20,20);
	}
	else if (stcFund.dPingcangYK < 0)
	{
		stcFundDate[i].clrText = pSkin->clrTextDown;//Color::MakeARGB(255,20,255,20);
	}
	else
	{
		stcFundDate[i].clrText = pSkin->clrTextNormal;//Color::MakeARGB(255,255,255,255);
	}
	stcFundDate[i].fRatio = 0.0;
	stcFundDate[i].strText = strTmp;

	i++;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1223,1)/*_T("浮动盈亏")*/,
		stcFund.dFudongYK);
	if (stcFund.dFudongYK > 0)
	{
		stcFundDate[i].clrText = pSkin->clrTextUp;//Color::MakeARGB(255,255,20,20);
	}
	else if (stcFund.dFudongYK < 0)
	{
		stcFundDate[i].clrText = pSkin->clrTextDown;//Color::MakeARGB(255,0,255,0);
	}
	else
	{
		stcFundDate[i].clrText = pSkin->clrTextMark;//Color::MakeARGB(255,255,255,255);
	}
	stcFundDate[i].fRatio = 0.0;
	stcFundDate[i].strText = strTmp;
	i++;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1222,1)/*_T("净值")*/,
		stcFund.dJingzhi);
	stcFundDate[i].strText = strTmp;
	i++;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1048,1)/*_T("占用保证金")*/,
		stcFund.dZhanyongBZHJ);
	stcFundDate[i].strText = strTmp;
	i++;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1049,1)/*_T("冻结保证金")*/,
		stcFund.dDongjieBZHJ);
	stcFundDate[i].strText = strTmp;
	i++;
	strTmp.Format(_T("%s\r\n%.02f"),CLanguageInterface::Instance()->GetStr(1047,1),/*_T("可用保证金"),*/
		stcFund.dKeyongBZHJ);
	stcFundDate[i].strText = strTmp;

	if (stcFund.dFengxianL <= 0.000001)
		strTmp.Format(_T("%s\r\n-.-%%"),CLanguageInterface::Instance()->GetStr(1225,1)/*_T("风险率")*/);//风险率
	else
		strTmp.Format(_T("%s\r\n%.02f%%"),CLanguageInterface::Instance()->GetStr(1225,1)/*_T("风险率")*/,stcFund.dFengxianL*100);//风险率
	i++;
	stcFundDate[i].strText = strTmp;

	i++;
	strTmp.Format(_T("%s\r\n"),CLanguageInterface::Instance()->GetStr(1273,1)/*_T("风险级别")*/);
	if (stcFund.dFengxianJB == 2)	//"自动斩仓风险率" 
	{
		stcFundDate[i].clrText = pSkin->clrTextUp;//Color::MakeARGB(255,255,20,20);
		strTmp  += CLanguageInterface::Instance()->GetStr(1226,1)/*_T("接近强平")*/;	//风险级别：接近强平
	}
	else if (stcFund.dFengxianJB == 1) //"追加保证金风险率"
	{
		stcFundDate[i].clrText = pSkin->clrTextMark;//Color::MakeARGB(255,20,20,255);
		strTmp  += CLanguageInterface::Instance()->GetStr(1227,1)/*_T("追加")*/;//风险级别：追加
	}
	else
	{
		stcFundDate[i].clrText = pSkin->clrTextDown;//Color::MakeARGB(255,0,255,0);
		strTmp  += CLanguageInterface::Instance()->GetStr(1228,1)/*_T("正常")*/;//风险级别：正常
	}
	stcFundDate[i].fRatio = 0.0;
	stcFundDate[i].strText = strTmp;

	m_BtnFund.SetFundDate(stcFundDate,9);
}
int CDlgMainClient::GetCurMarket()
{
	int iSel = m_statTab.GetSelTab();
	if (iSel == IDB_TAB_ONE) return 1;
	if (iSel == IDB_TAB_TOW) return 2;

	return -1;
}
void CDlgMainClient::ShowTab()
{
	int iSel = m_statTab.GetSelTab();

	if (iSel==IDB_TAB_ONE) m_DlgMetalsMarket.SetDrawLineCallBcakFun();
	else if (iSel==IDB_TAB_TOW) m_dlgSpotMarket.SetDrawLineCallBcakFun();
	m_DlgMetalsMarket.ShowWindow(iSel==IDB_TAB_ONE?SW_SHOW:SW_HIDE);
	m_dlgSpotMarket.ShowWindow(iSel==IDB_TAB_TOW?SW_SHOW:SW_HIDE);
	m_dlgBinManagePack.ShowWindow(iSel==IDB_TAB_THREE?SW_SHOW:SW_HIDE);
	m_dlgFundInfoPack.ShowWindow(iSel==IDB_TAB_FOUR?SW_SHOW:SW_HIDE);
	if (m_dlgBinManagePack.IsWindowVisible())m_dlgBinManagePack.NotifyTradeBin(WM_NOTIFY_UPDATEBIN,2,0);
	if (m_dlgFundInfoPack.IsWindowVisible())CDateManage::Instance()->NotifyTrade(WM_NOTIFY_UPDATEFUN,0,0,3);
	m_DlgTopToolbar.SetShowMode();
	if (iSel==IDB_TAB_TOW)
	{
		m_DlgTopToolbar.SetShowMode(1);
	}
	CDateManage::Instance()->SetMarketType(iSel-IDB_TAB_ONE);
}

void CDlgMainClient::OnTabOne()
{
	ShowTab();
}

void CDlgMainClient::OnTabTow()
{
	ShowTab();
}
//#include "LoginDlg.h"
void CDlgMainClient::OnTabThree()
{
	ShowTab();
}

void CDlgMainClient::OnTabFour()
{
	ShowTab();
}

void CDlgMainClient::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	ReArrayCtrl();
}

void CDlgMainClient::ReArrayCtrl()
{
	int iFundHight = 50;
	int iTabWith = 22;
	int iFundMinWith = 1080;
	int iTopToolbarHight = 40;
	int iTopToolbarMinWith = 1000;
	int iTabMinHight = 500;
	int iStautusWndHight = 30;

	CRect rectClient;
	GetClientRect(&rectClient);

	if (rectClient.Width() > iFundMinWith)
	{
		iFundMinWith = rectClient.Width();
	}

	if (IsWindow(m_BtnFund.GetSafeHwnd()))
	{
		int iTop = iTopToolbarHight;
		if (m_iFundPos == 1)
		{
			iTop = rectClient.bottom-iStautusWndHight - iFundHight;
		}	
		m_BtnFund.MoveWindow(0,iTop,iFundMinWith,iFundHight);
		//if (!m_BtnFund.IsWindowVisible())iFundHight = 0;
	}
	
	if (1/*rectClient.Width() > iTopToolbarMinWith*/)
	{
		iTopToolbarMinWith = rectClient.Width();		
	}
	if (IsWindow(m_DlgTopToolbar.GetSafeHwnd()))
	{
		m_DlgTopToolbar.MoveWindow(0,0,iTopToolbarMinWith,iTopToolbarHight);
		//if (!m_DlgTopToolbar.IsWindowVisible())iTopToolbarHight = 0;
	}

	if (IsWindow(m_statTab.GetSafeHwnd()))
	{
		int iTop = iTopToolbarHight+iFundHight;
		if (m_iFundPos == 1)
		{
			iTop = iTopToolbarHight;
		}	
		m_statTab.MoveWindow(0,iTop,iTabWith,iTabMinHight);
		if (m_iClientType == 0x1000) iTabWith = 0;
		//if (!m_statTab.IsWindowVisible())iTabWith = 0;
		//if (m_iClientType != 2)iTabWith = 0;
	}

	if (IsWindow(m_dlgStatus.GetSafeHwnd()))
	{
		m_dlgStatus.MoveWindow(0,rectClient.bottom-iStautusWndHight,rectClient.Width(),iStautusWndHight);
	}
	if (m_iFundPos == 1)
	{
		rectClient.InflateRect(-iTabWith,-iTopToolbarHight,0,-iStautusWndHight-iFundHight);
	}
	else
	{
		rectClient.InflateRect(-iTabWith,-iFundHight-iTopToolbarHight,0,-iStautusWndHight);
	}
	if (IsWindow(m_DlgMetalsMarket.GetSafeHwnd()))
	{
		m_DlgMetalsMarket.SetWindowPos(NULL, rectClient.left, rectClient.top,
			rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if (IsWindow(m_dlgSpotMarket.GetSafeHwnd()))
	{
		m_dlgSpotMarket.SetWindowPos(NULL, rectClient.left, rectClient.top,
			rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}
	
	if (IsWindow(m_dlgBinManagePack.GetSafeHwnd()))
	{
		m_dlgBinManagePack.SetWindowPos(NULL, rectClient.left, rectClient.top,
			rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if (IsWindow(m_dlgFundInfoPack.GetSafeHwnd()))
	{
		m_dlgFundInfoPack.SetWindowPos(NULL, rectClient.left, rectClient.top,
			rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

BOOL CDlgMainClient::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	//if (pMsg->message == WM_LBUTTONDOWN)
	//{
	//	AfxMessageBox(_T("44444"));
	//}
	return CDialog::PreTranslateMessage(pMsg);
}


//BOOL CDlgMainClient::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	SetCursor(LoadCursor(NULL,IDC_SIZEWE)); 
//	return TRUE;;
//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
//}


BOOL CDlgMainClient::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	
	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
LRESULT CDlgMainClient::OnLoginSuccess(WPARAM wPar,LPARAM lPar)
{
	AccessCtrl();

	ReArrayCtrl();
	//Invalidate();
	m_statTab.UpdateWnd();
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_NOTIFY_LOGIN_SUC,0,0);
    if (CPublic::Instance()->GetInitiAtiveShowWarn()&&IsWindow(m_dlgWarn.GetSafeHwnd()))
	{
		CRect rcClt;
		GetClientRect(&rcClt);
		ClientToScreen(&rcClt);
		m_dlgWarn.MoveWindow(&rcClt);
		m_dlgWarn.ShowWindow(SW_SHOW);
	}
	return 1;
}
LRESULT CDlgMainClient::OnTradeNotify(WPARAM wPar,LPARAM lPar)
{
	TCHAR *pRet = (TCHAR*)lPar;
	CString strRet = pRet;
	//if (pRet)
	//{
	//	delete []pRet;
	//	pRet = NULL;
	//}

	if (!strRet.IsEmpty())
	{
		::MessageBox(this->GetSafeHwnd(),strRet,_T("系统提示"),MB_ICONINFORMATION|MB_OK);
		//m_dlgBidTrade.PostMessage(wParam);
	}
	return 1;
}

LRESULT CDlgMainClient::OnOpenDrawTool(WPARAM wPar,LPARAM lPar)
{
	if (CPublic::Instance()->IsTrendChart())
	{
		m_dlgDrawTool.ShowWindow(SW_SHOW);
	}	
	return 1;
}

LRESULT CDlgMainClient::OnUpdateDrawTool(WPARAM wPar,LPARAM lPar)
{
	if (CPublic::Instance()->IsTrendChart())
	{
		m_dlgDrawTool.ResetSel(0);
	}	
	return 1;
}
LRESULT CDlgMainClient::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	m_statTab.Invalidate();
	m_BtnFund.Invalidate();
	int iSel = m_statTab.GetSelTab();
	m_DlgMetalsMarket.PostMessage(WM_SKINCHANGE,wPar,lPar);
	m_dlgSpotMarket.PostMessage(WM_SKINCHANGE,wPar,lPar);
	m_dlgBinManagePack.PostMessage(WM_SKINCHANGE,wPar,lPar);
	m_dlgFundInfoPack.PostMessage(WM_SKINCHANGE,wPar,lPar);
	m_dlgStatus.PostMessage(WM_SKINCHANGE,wPar,lPar);
	m_DlgTopToolbar.PostMessage(WM_SKINCHANGE,wPar,lPar);
	return 1;
}
LRESULT CDlgMainClient::OnReLayout(WPARAM wPar,LPARAM lPar)
{
	if (IsWindow(m_DlgMetalsMarket.GetSafeHwnd()))
	{
		m_DlgMetalsMarket.PostMessage(WM_RELAYOUT);
	}
	return 1;
}
LRESULT CDlgMainClient::OnNotifyKOASys(WPARAM wPar,LPARAM lPar)
{
	if (IsWindow(m_dlgSpotMarket.GetSafeHwnd()))
	{
		::PostMessage(m_dlgSpotMarket.GetSafeHwnd(),WM_NOTIFY_KOA_SYS,wPar,lPar);
	}
	return 1;
}
LRESULT CDlgMainClient::OnNotifyQuoteSys(WPARAM wPar,LPARAM lPar)
{
	if ((int)wPar == 1)
	{
		
		if (IsWindow(m_dlgSpotMarket.GetSafeHwnd()))
		{
		::PostMessage(m_dlgSpotMarket.GetSafeHwnd(),WM_COMMAND,MAKEWPARAM(lPar, wPar),0);
		}
	}
	return 1;
}

void CDlgMainClient::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		ReArrayCtrl();
	}
}
void CDlgMainClient::NotifySpotMarket(UINT uiMsg,WPARAM wPar,LPARAM lPar)
{
	if (!IsWindow(m_dlgSpotMarket.GetSafeHwnd()))return;
	
	m_dlgSpotMarket.PostMessage(uiMsg,wPar,lPar);
}

void CDlgMainClient::UpdateToolBarStatus()
{
	m_DlgTopToolbar.UpdateButtonStatus();
}
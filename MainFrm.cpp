
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "KOAClient.h"
#include "WINDOWSX.H"
#include "inputkey.h"
#include "input.h"

#include "MainFrm.h"
#include "SysProtyDlg.h"
#include "StockView.h"
#include "ChangePswdForm.h"
#include "../trade/trademsgdef.h"


#include <Wininet.h>  
#include <Sensapi.h>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_SEPARATOR, 
	IDS_SPERATOR_QUOTELINKSTATUS,
	ID_SEPARATOR,
	IDS_SPERATOR_TRADELINKSTATUS,
	IDS_SPERATOR_USERID,
	IDS_SPERATOR_USERSTATUS,
	IDS_SPERATOR_USERNAME,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
//	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_SYS_BEGIN,ID_SYS_END,OnMenuInfo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SYS_BEGIN,ID_SYS_END,OnUpdateMenuInfo)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_SPERATOR_QUOTELINKSTATUS,IDS_SPERATOR_USERNAME,OnUpdateStatusBarPanes)
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_TRADELOGININFO,OnTradeMessage)
	ON_MESSAGE(WM_TASKMANAGE,OnTaskManage)
	ON_MESSAGE(WM_REQUESTDATA,OnKeyGhostMessage)
	ON_MESSAGE(WM_KEYGHOSTMSG,OnKeyGhostMessageZYQ)
	ON_MESSAGE(WM_SHOWTOOLBAR,OnShowToolBar)
	ON_COMMAND(ID_SC_CLOSE, OnFrameClose)
	ON_COMMAND(ID_SC_MAX, OnFrameMax)
	ON_COMMAND(ID_SC_MIN, OnFrameMin)
	ON_COMMAND(ID_SC_RESTORE, OnFrameRestore)
	ON_COMMAND_RANGE(ID_TITLEMENUMIN,ID_TITLEMENUMAX,OnTitleMenu)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_SETTINGCHANGE()
//	ON_WM_INITMENU()
//	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_LOGINCHANGEPWD,OnLoginChangePwd)
	ON_MESSAGE(WM_NOTIFY_LOGIN_SUC,OnLoginSuccess)
ON_WM_SIZE()
ON_WM_NCPAINT()
ON_WM_ERASEBKGND()
ON_WM_NCACTIVATE()
ON_WM_NCCALCSIZE()
ON_WM_NCHITTEST()
ON_WM_SYSCOMMAND()
ON_WM_ACTIVATE()
ON_WM_NCLBUTTONDBLCLK()
ON_WM_ACTIVATEAPP()
//ON_WM_PAINT()
END_MESSAGE_MAP()


// CMainFrame 构造/析构

CMainFrame::CMainFrame():m_rcBorder(0,0,0,0)
{
	m_nReconnectCount = 0;
	m_bFullScreenMode = FALSE;
	m_bShowDrawLineBar = FALSE;
	m_bShowState = TRUE;
	m_bReLogin = FALSE;
	m_bInternetStatus = FALSE;
	m_nTradeReconnectCount = 1;

	m_clrCaptionBk = Color::MakeARGB(255,72,0,1);
	m_pImglogo = NULL;
	m_pImgBk = NULL;
	m_pImgBorderL = NULL;
	m_pImgBorderR = NULL;
	m_pImgBorderB = NULL;
	m_iCaptionWith = 30;
	m_pImgCaption = NULL;

	m_bRefresh = TRUE;
	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
	ReleasImg();
	std::vector<WWndLessButton*>::iterator vIt;
	for (vIt = m_vecMenu.begin();vIt != m_vecMenu.end();vIt++)
	{
		delete (*vIt);
	}
	m_vecMenu.clear();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);
	SetMenu(NULL);
	//ModifyStyle(0, WS_CLIPSIBLINGS);
	//lpCreateStruct->style |= ~WS_VISIBLE;
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//MoveWindow(0,0,0,0);
	//ShowWindow(SW_HIDE);
	//this->ModifyStyle(0, WS_CLIPSIBLINGS);
	//去除标题栏
	ModifyStyle(WS_CAPTION,0);
	//ModifyStyle(WS_CAPTION|WS_SYSMENU,0,SWP_FRAMECHANGED);
	//ModifyStyle(WS_CAPTION|WS_SYSMENU|WS_BORDER,0);
	//ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	//ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);
	
	//OnApplicationLook(theApp.m_nAppLook);
	// 设置用于绘制所有用户界面元素的视觉管理器
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));//CMFCVisualManagerVS2008));CMFCVisualManagerWindows7
	// 启用 Visual Studio 2005 样式停靠窗口行为
	//CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	//EnableAutoHidePanes(CBRS_ALIGN_ANY);

	//// enable Visual Studio 2005 style docking window behavior
	//CDockingManager::SetDockingMode(DT_SMART);
	//// enable Visual Studio 2005 style docking window auto-hide behavior
	//EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MAINFRAME, theApp.m_bHiColorIcons ? IDR_MAINFRAME : 0);


	//EnableDocking(CBRS_ALIGN_ANY);	

//	m_menuMain.LoadMenu(IDR_MAINFRAME);
	//CLanguageInterface::Instance()->InitMainMenu(&m_menuMain);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;     
	}

// 	if (!m_wndMenuBar.Create(this))
// 	{
// 		TRACE0("未能创建菜单栏\n");
// 		return -1;     
// 	}

// 	if(0 != CPublic::Instance()->GetMainViewMode())
// 	{	
// 		if(!CPublic::Instance()->GetMoneyInOutEnable())
// 		{
// 			m_menuMain.DeleteMenu(3,MF_BYPOSITION);
// 		}
// 		m_menuMain.DeleteMenu(1,MF_BYPOSITION);
// 
// 	}
// 	else
// 	{
// 		CString strMenuAppend;
// 		strMenuAppend= CPublic::Instance()->GetAppendMenu();
// 		if(!strMenuAppend.IsEmpty())
// 		{
// 			m_menuMain.AppendMenu(MF_ENABLED|MF_STRING,ID_SYS_APPENDMENU,strMenuAppend);
// 		}
// 	}
// 	m_wndMenuBar.CreateFromMenu(m_menuMain.GetSafeHmenu(),TRUE,TRUE);
// 	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndMenuBar);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	//////////////////////////////////////////////////////////////////////////
	//创建工具栏
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_CUSTOMERASE/*TBSTYLE_FLAT*/, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | 
// 		CBRS_FLYBY | CBRS_SIZE_DYNAMIC,IDR_MAINFRAME) ||
// 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
// 	{
// 		TRACE0("未能创建工具栏\n");
// 		return -1;      // 未能创建
// 	}
    //CLanguageInterface::Instance()->InitToolBar(IDR_MAINFRAME,&m_wndToolBar);
// 	if(0 != CPublic::Instance()->GetMainViewMode())
// 	{	
// 
// 		for(int i =0;i<11;i++)
// 			m_wndToolBar.RemoveButton(8);
// 		if(!CPublic::Instance()->GetMoneyInOutEnable())
// 		{
// 			for(int i =0;i<2;i++)
// 				m_wndToolBar.RemoveButton(5);
// 		}
// 	}
// 	//m_wndToolBar.EnableTextLabels();
// 
// 	m_wndToolBar.SetWindowText(CLanguageInterface::Instance()->GetStr(10027));
//	if(CPublic::Instance()->GetMoneyInOutEnable())
//		m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE,  CLanguageInterface::Instance()->GetStr(10029));

	//m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	//DockPane(&m_wndToolBar);
	//////////////////////////////////////////////////////////////////////////

	if(0 == CPublic::Instance()->GetMainViewMode())
	{
		if (!m_wndDrawLineBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | 
			CBRS_FLYBY | CBRS_SIZE_DYNAMIC,IDT_DRAWLINE) ||
			!m_wndDrawLineBar.LoadToolBar( IDT_DRAWLINE))
		{
			TRACE ("创建画线工具栏失败");
			return -1;
		}
		CLanguageInterface::Instance()->InitToolBar(IDT_DRAWLINE,&m_wndDrawLineBar);
		//m_wndDrawLineBar.SetSizes(CSize(42,42), CSize(32,32));
		m_wndDrawLineBar.EnableTextLabels();
		m_wndDrawLineBar.SetWindowText(CLanguageInterface::Instance()->GetStr(10028));

		//mfcBt.m_bText = TRUE;
		//mfcBt.m_strText = _T("123456");
		//m_wndDrawLineBar.InsertButton(mfcBt,3);
		//m_wndDrawLineBar.EnableDocking(CBRS_ALIGN_TOP);
		//DockPane(&m_wndDrawLineBar);	
		//m_wndDrawLineBar.e
	}
	else
	{

	}

	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}
	
	EnablePaneMenu(TRUE, NULL,NULL,ID_VIEW_TOOLBAR);
//	EnablePaneMenu(TRUE,ID_VIEW_CUSTOMIZE, CLanguageInterface::Instance()->GetStr(10029), ID_VIEW_TOOLBAR);	

	// 启用快速(按住 Alt 拖动)工具栏自定义
	//yfj D
	//CMFCToolBar::EnableQuickCustomization();
	//m_wndMenuBar.SetShowAllCommands();
	////
//////////////////////////////////////////////////////////////////////////
	//delete by david gong at 20140210
// 	if(CPublic::Instance()->IsAutoLogin())
// 	{//自动登录，此处开始连接行情
// 	//	CPublic::Instance()->InitQuote(AfxGetMainWnd()->GetSafeHwnd());
// 		if(!CPublic::Instance()->InitBalanceServer())
// 		{
// 			MessageBox(CLanguageInterface::Instance()->GetStr(10008),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
// 			return FALSE;
// 		}
// 		else
// 		{
// 
// 		}
// 		CPublic::Instance()->InitTrade(GetSafeHwnd());
// 	}
	RegHotKey();
	SetWindowText(CPublic::Instance()->GetProductName());
	ResetStatusBar();
	CString strName;
	m_uiCheckConnectTimer = SetTimer(10,1000,NULL);
	m_bInternetStatus = TRUE;
	//////////////////////////////////////////////////////////////////////////
	CString strIconPath;
	strIconPath = CPublic::Instance()->GetSysPath()+_T("/interface/koa.ico");
	HICON   hIcon=(HICON)::LoadImage(AfxGetInstanceHandle(),strIconPath,IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	SetIcon(hIcon,FALSE);
	//SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);
	
	//m_DlgMainClient.Create(CDlgMainClient::IDD,this);
	//m_DlgMainClient.ShowWindow(SW_SHOW);

	//m_dlgMainWnd.Create(CDlgMainClient::IDD,this);
	//m_dlgMainWnd.ShowWindow(SW_SHOW);

	m_wndStatusBar.ShowWindow(SW_HIDE);
//    m_wndMenuBar.ShowWindow(SW_HIDE);
	m_wndTrade.ShowWindow(SW_HIDE);

//	m_wndToolBar.ShowWindow(SW_HIDE);
	
	if(0 == CPublic::Instance()->GetMainViewMode())
	{
		m_wndDrawLineBar.ShowWindow(SW_HIDE);
		m_wndPriceListView.ShowWindow(SW_HIDE);
		m_wndPriceInfoView.ShowWindow(SW_HIDE);
		RecalcLayout();
	}
	InitCtrl();
	
	return 0;
}

BOOL CMainFrame::ChangToolBar(UINT iMode)
{
	return TRUE;
}

 LRESULT CMainFrame::OnShowToolBar(WPARAM wParam,LPARAM lParam)
 {
	 //m_wndToolBar.ShowWindow((int)wParam);
	// m_ToolbarLeft.ShowWindow((int)wParam);
	 RecalcLayout();
	 return 1;
 }

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	//cs.style  &= ~FWS_ADDTOTITLE;  
    //m_strTitle = CPublic::Instance()->GetProductName();    //设置程序标题
	//cs.style &= ~(WS_VISIBLE);
	//ModifyStyle(WS_CAPTION|WS_SYSMENU|WS_BORDER,0);
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	//if (!m_TabDp.Create(_T("TabLeft"), this, CRect(0, 0,50,100), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	//{
	//	// WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI
	//	TRACE0("未能创建“TAB”窗口\n");
	//	return FALSE; // 未能创建
	//}
	//m_TabDp.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_TabDp);

	if (!m_wndTrade.Create(CLanguageInterface::Instance()->GetStr(10026), this, CRect(0, 0, 100, 50), TRUE, ID_VIEW_TRADE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		TRACE0("未能创建交易窗口\n");
		return FALSE; // 未能创建
	}
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_TRADE_WND_HC : IDI_TRADE_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTrade.SetIcon(hOutputBarIcon, FALSE);
	//m_wndTrade.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndTrade);

	if(0 == CPublic::Instance()->GetMainViewMode())
	{
		if (!m_wndPriceInfoView.Create(CLanguageInterface::Instance()->GetStr(10024), this, CRect(0, 0,200,400), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP|CBRS_BORDER_RIGHT | CBRS_FLOATING,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
		{                                                                                                                                 // WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI
			TRACE0("未能创建“类视图”窗口\n");
			return FALSE; // 未能创建
		}

		if (!m_wndPriceListView.Create(CLanguageInterface::Instance()->GetStr(10025), this, CRect(0, 0,200,400), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP|CBRS_BORDER_RIGHT| CBRS_FLOATING,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
		{
			TRACE0("未能创建“行情列表 视图”窗口\n");
			return FALSE; // 未能创建
		}
		// 允许用户定义的工具栏操作:

		//m_wndPriceListView.EnableDocking(CBRS_ALIGN_ANY);
		//m_wndPriceInfoView.EnableDocking(CBRS_ALIGN_ANY);
		//DockPane(&m_wndPriceListView);

		HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_PRICELIST_VIEW_HC : IDI_PRICELIST_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndPriceListView.SetIcon(hFileViewIcon, FALSE);

		HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_PRICEINFO_WND_HC : IDI_PRICEINFO_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndPriceInfoView.SetIcon(hClassViewIcon, FALSE);
	}

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{

}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	//CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	//pDlgCust->EnableUserDefinedToolbars();
	//pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	//if (lres == 0)
	//{
	//	return 0;
	//}

	//CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	//ASSERT_VALID(pUserToolbar);

	//BOOL bNameValid;
	//CString strCustomize;
	//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);

	//pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 为所有用户工具栏启用自定义按钮
	//CString strCustomize;
	//strCustomize = CLanguageInterface::Instance()->GetStr(10029);
	//for (int i = 0; i < iMaxUserToolbars; i ++)
	//{
	//	CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
	//	if (pUserToolbar != NULL)
	//	{	

	//		pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);	
	//	}
	//}
	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
}

LRESULT CMainFrame::OnDataMessage(WPARAM wParam,LPARAM lParam)
{	
	HWND hWnd = CPublic::Instance()->GetLoginWnd();
	if(NULL  == wParam)
	{
		switch(lParam)
		{
		case QUOTELINKDOWN:
			{
				m_nReconnectCount++;
				CPublic::Instance()->InitQuote(AfxGetMainWnd()->GetSafeHwnd());
				ResetStatusBarLinkStatus(1,FALSE);
			}
			break;
		case QUOTELINKFAIL:
			{
				if(0 == m_nReconnectCount)
				{
					if(IDYES == MessageBox(CLanguageInterface::Instance()->GetStr(10007),CPublic::Instance()->GetProductName(),MB_ICONWARNING|MB_YESNO))
					{
						ExitMainWnd(1);
					}
				}
				ResetStatusBarLinkStatus(1,FALSE);
			}
			break;
		case QUOTELINKSUCCESS:
			{
				CPublic::Instance()->ReqLoginReq();
				ResetStatusBarLinkStatus(1,TRUE);
			}
			break;
		default:
			break;
		}
		return 0;
	}
	if( 0 ==lParam)return -1;

	const SNormaHead  * pNoramHead = (SNormaHead*)wParam;

	CDateManage::Instance()->ManageMsg((const char*)pNoramHead,(int)lParam);
	ResetStatusBarLinkStatus(1,TRUE);

	switch(pNoramHead->m_usType)
	{
	case USERPWDVALID:
		{
			const SUserPwdDataRes* pRes = (SUserPwdDataRes*)(pNoramHead+1);
			const char* pcText = (char*)(pRes+1);					
			if('1' != pRes->m_cPassed)
			{
				if(hWnd)::SendMessage(hWnd,WM_SETTEXT,QUOTELOGINFALIED,0);
				MessageBox(CString(pcText),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
				CPublic::Instance()->GetDataSource()->UnLink();
				return FALSE;
			}
			if(hWnd)
			{
				if(!m_bReLogin)	
				{
					::SendMessage(CPublic::Instance()->GetLoginWnd(),WM_SETTEXT,QUOTELOGINSUCCESS,0);
					((CKOAClientApp*)AfxGetApp())->InitAfterLogin(1);

					CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));

					if (NULL != pView)
						pView->UpdateToolBarStatus();
				}
				else
				{
					::SendMessage(CPublic::Instance()->GetLoginWnd(),WM_SETTEXT,QUOTEOTHERSUCCESS,0);//
				}
			}
			int iAccountAc = CPublic::Instance()->GetClientType();
			if(!m_bReLogin)
			{
				if (iAccountAc&0x1000)
				{
					CPublic::Instance()->ReqInitReq();
				}
				if (iAccountAc&0x0100)
				{
					CDateManage::Instance()->ReqInitMarket_Sp();
				}
			}
			if (iAccountAc&0x1000)
			{
				CPublic::Instance()->ReqStockInfo();			
				CPublic::Instance()->ReqReportInfo();
			}
			
			if (iAccountAc&0x0100)
			{
				CDateManage::Instance()->ReqStockInfo_SP();
				CDateManage::Instance()->ReqReportData_Sp();
			}

			ResetStatusBarUserInfo();
		}
		break;
	case INITMARKET:
		{	
			CInputKey::Instance()->RemoveAll();
			CInputKey::Instance()->ReadCommand();
			const StuSymbolData* pCodeInfo =(StuSymbolData*)(pNoramHead+1);
			CInputKey::Instance()->AddStockCode(pCodeInfo,pNoramHead->m_usSize,1);

			if(hWnd)
			{
				::SendMessage(CPublic::Instance()->GetLoginWnd(),WM_SETTEXT,QUOTEOTHERSUCCESS,0);				
			}

			CPublic::Instance()->ReqReportInfo();
			if(CPublic::Instance()->GetMainViewMode() == 0)
				CPublic::Instance()->ReqStockInfo();
			if(!m_bReLogin)	
			{
				RecalcLayout();	
				m_wndTrade.LoadDllInfo();
				RedrawWindow();
			}
			else
			{

			}
			//if((CPublic::Instance()->GetTradeDll()->m_pIsNeedChangePwd())&&(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus()))
			//{//是否需要修改密码
			//	if(IDYES== MessageBox(CLanguageInterface::Instance()->GetStr(10012),CPublic::Instance()->GetProductName(),MB_ICONWARNING|MB_YESNO))
			//	{
			//		OnMenuInfo(ID_TRADE_MODIFYPWD);
			//	}
			//}
			m_bReLogin = TRUE;
		}
		break;	
	case INITMARKET_ZYQ:
		{
			if(hWnd)
			{
				::SendMessage(CPublic::Instance()->GetLoginWnd(),WM_SETTEXT,QUOTEOTHERSUCCESS,0);				
			}
			CDateManage::Instance()->ReqReportData_Sp();
		}
		break;
	default:	
		break;
	}
	CPublic::Instance()->GetDataSource()->FreeBuffer((char*)wParam);
	return 1;
}

LRESULT CMainFrame::OnLoginChangePwd(WPARAM wParam,LPARAM lParam)
{
	if((CPublic::Instance()->GetTradeDll()->m_pIsNeedChangePwd())&&(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus()))
	{
		if(IDYES== MessageBox(CLanguageInterface::Instance()->GetStr(10012),CPublic::Instance()->GetProductName(),MB_ICONWARNING|MB_YESNO))
		{
			OnMenuInfo(ID_TRADE_MODIFYPWD);
		}
	}
	return 1;
};
void CMainFrame::FullScreenMode(bool bShow)
{
	if( bShow )
	{
		GetWindowPlacement(&m_OldWndPlacement); 
		CRect WindowRect; 
		GetWindowRect(&WindowRect); 

		CRect ClientRect; 
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, 
			&ClientRect); 

		ClientToScreen(&ClientRect); 

		m_bFullScreenMode = TRUE;

		int nFullWidth  = GetSystemMetrics(SM_CXSCREEN); 
		int nFullHeight = GetSystemMetrics(SM_CYSCREEN); 

		m_FullScreenRect.left	=	WindowRect.left - ClientRect.left; 
		m_FullScreenRect.top	=	WindowRect.top - ClientRect.top; 
		m_FullScreenRect.right	=	WindowRect.right - ClientRect.right + nFullWidth; 
		m_FullScreenRect.bottom	=	WindowRect.bottom - ClientRect.bottom + nFullHeight-40;

		// 进入全屏显示状态 
		WINDOWPLACEMENT wndpl; 

		wndpl.length	= sizeof(WINDOWPLACEMENT); 
		wndpl.flags		= 0; 
		wndpl.showCmd	= SW_SHOWNORMAL; 

		wndpl.rcNormalPosition = m_FullScreenRect; 

		SetWindowPlacement(&wndpl); 

	}
	else
	{
		m_bFullScreenMode = FALSE;

		SetWindowPlacement(&m_OldWndPlacement);
		ShowWindow(SW_SHOWMAXIMIZED);	
	}			
}

void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();
}

void CMainFrame::OnClose()
{
	if(IDYES != MessageBox(CLanguageInterface::Instance()->GetStr(10014),CPublic::Instance()->GetProductName(), MB_YESNO|MB_ICONQUESTION))return;
	ExitMainWnd(1);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (1)
	{
		m_sysCloseBtn.RelayEvent(pMsg);
		m_sysMinBtn.RelayEvent(pMsg);
		m_sysMaxBtn.RelayEvent(pMsg);
		if (!CPublic::Instance()->IsLockMainClient())
		{
			std::vector<WWndLessButton*>::iterator vIt;
			for (vIt = m_vecMenu.begin();vIt != m_vecMenu.end();vIt++)
			{
				(*vIt)->RelayEvent(pMsg);
			}
		}
	}

	if (pMsg->message == WM_HOTKEY /*&& pMsg->wParam == 0xa001*/)
	{
		OnMenuInfo(pMsg->wParam);
	}

	return CFrameWndEx::PreTranslateMessage(pMsg);
}
#include "DlgTrendChart.h"
void CMainFrame::OnMenuInfo(UINT nID)
{
	CWnd *pWnd = this;
	switch(nID)
	{
	case IDM_RELAYOUT:
		{
			CDateManage::Instance()->NotifyWndReLayout();
		}
		break;
	case ID_DRAWTOOL:
		{
			CWnd* pView = (CWnd*)(CPublic::Instance()->GetView(11,2));
			if (pView&&IsWindow(pView->GetSafeHwnd()))
			{
				::PostMessage(pView->GetSafeHwnd(),WM_OPENDRAWTOOL,0,0);
			}
		}
		break;
	case ID_TOOL_FULLSCREEN:   
		{
			if(m_bFullScreenMode)
				FullScreenMode(FALSE);
			else
				FullScreenMode(TRUE);
		}
		break;
	case  ID_TOOL_CALC:
		{
			ShellExecute(NULL,_T("open"),"calc.exe",NULL,NULL,SW_NORMAL);
		}
		break;
	case ID_SYSTEM_CONNECTTOHOST: //连接主站
		{	
			((CKOAClientApp*)AfxGetApp())->AskLoginDlg();
			SetWindowText(CPublic::Instance()->GetProductName());
		}
		break;
	case ID_SYSTEM_DISCONNECTHOST:// 断开主站
		{
			CPublic::Instance()->UnLinkQuote();	
			CPublic::Instance()->UnLinkTrade();
			ResetStatusBarLinkStatus(1,FALSE);
			ResetStatusBarLinkStatus(3,FALSE);
			ClearFund();
		}
		break;
	case ID_SYSTEM_SAVEASPIC:
		{
			Save2Pic();
		}
		break;
	case ID_INTELLIG_FUNCTIONMANAGE:
		{
			CDlgMainClient* pWnd = (CDlgMainClient*)(CPublic::Instance()->GetView(11,2));
			if (pWnd)
			{
				SDefineReturn define;
				CFormulaFace::OnOpenDefineTechDlg(define,NULL,pWnd);
			}	
		}
		break;
	case ID_QUOTE_ANALYSISTPIC:
	case ID_QUOTE_REALRUNPIC:
		{
			CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));
			if(pView)
			{
				pView->ChangeMode(nID-ID_QUOTE_REALRUNPIC);
			}
		}
		break;
	case ID_TOOL_DRAWTOOL:
		{
			ShowDrawLineToolBar();
		}
		break;
	case ID_NORMALORDER:
		{
			CDlgMainClient* pWnd = (CDlgMainClient*)(CPublic::Instance()->GetView(11,2));
			if(pWnd)
			{
				pWnd->NotifySpotMarket(WM_COMMAND,MAKEWPARAM(ID_NORMALORDER, 0),0);
			}
		}
		break;
	case ID_SYSTEM_PRICEORDER:
	case ID_SYSTEM_NEWORDER:
		{
			CString strTemp;

			int iMarketType = CDateManage::Instance()->GetMarketType();
			if (iMarketType != 0)break;
			const SCodeInfo* pCode;
			CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));
			if(NULL == pView)
			{
				pCode= CPublic::Instance()->GetCurStock();
			}
			else
			{
				pCode= pView->GetCurStock();
			}	

			strTemp.Format(_T("下单商品:%s \r\n"),pCode->szCode);

			if(nID == ID_SYSTEM_NEWORDER)
				CPublic::Instance()->CallQuickTrade(pCode);
			else
				CPublic::Instance()->CallQuickTrade(pCode,0,1);
		}
		break;
	case ID_TOOL_BOSSKEY:
		{
			if(m_bShowState)
			{
				HideMainFrm();
			}
			else
			{
				ShowMainFrm();
			}
		}
		break;
	case ID_TOOL_UPDATE:
		{
			if (m_bRefresh)
			{
				CPublic::Instance()->GetTradeDll()->m_pRefresh();
				int iAccountAc = CPublic::Instance()->GetClientType();
				if (iAccountAc&0x1000)
				{
					CPublic::Instance()->ReqStockInfo();
					CPublic::Instance()->ReqReportInfo();
				}
				if (iAccountAc&0x0100)
				{
					CDateManage::Instance()->ReqStockInfo_SP();
					CDateManage::Instance()->ReqReportData_Sp();
				}
				m_bRefresh = FALSE;
				SetTimer(EVENT_REFRESH,EVENT_REFRESH_TM,NULL);
			}
		}
		break;
	case ID_TRADE_MODIFYPWD:
		{
			if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
			{
				CChangePswdForm changepwddlg;
				changepwddlg.DoModal();
			}
			else
			{
				//MessageBox(CLanguageInterface::Instance()->GetStr(10006),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
				MessageBox(CLanguageInterface::Instance()->GetStr(10008),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
			}

		}
		break;
	case ID_SYS_APPENDMENU:
		{
			ShellExecute(NULL,_T("open"),CPublic::Instance()->GetAppendMenuLink(),NULL,NULL,SW_NORMAL);
		}
		break;
	case ID_HELP_HOW2USE:
		{
			CString strWeb;
			strWeb = CPublic::Instance()->GetHow2UseWebPage();
			if(-1 == strWeb.Find("http://"))
			{
				strWeb = CPublic::Instance()->GetSysPath() +strWeb;
			}
			ShellExecute(NULL,_T("open"),strWeb,NULL,NULL,SW_NORMAL);
		}
	case ID_HELP_EDUCATION:
		{
			CString strWeb;
			strWeb = CPublic::Instance()->GetEducationStr();
			if(-1 == strWeb.Find("http://"))
			{
				strWeb = CPublic::Instance()->GetSysPath() +strWeb;
			}
			ShellExecute(NULL,_T("open"),strWeb,NULL,NULL,SW_NORMAL);
		}
		break;
	default:
		{
			if((nID >ID_ANALYSIST_BEGIN)&&(nID <ID_ANALYSIST_END))
			{
				CDlgTrendChart* pView = (CDlgTrendChart*)(CPublic::Instance()->GetView(/*VIEWINFO_STOCK*/10));
				if(pView)pView->OnUserCmd(nID);
			}
			else if((nID >ID_TRADE_BEGIN)&&(nID<ID_TRADE_END))
			{
				if(!CPublic::Instance()->GetTradeDll()->m_pMenuCmd(nID,CDateManage::Instance()->GetMarketType()))
				{
					MessageBox(CLanguageInterface::Instance()->GetStr(20067),CLanguageInterface::Instance()->GetStr(20068),MB_OK|MB_ICONWARNING);
				}
			}
		}
		break;
	}
}

void CMainFrame::ClearFund()
{
	
	CWnd* pWnd = (CWnd*)(CPublic::Instance()->GetView(11,2));
	if (IsWindow(pWnd->GetSafeHwnd()))
	{
		FUNDINFO stcFund;
		pWnd->SendMessage(WM_DATEMANAGE,0,(LPARAM)&stcFund);
	}
}
void CMainFrame::OnUpdateMenuInfo(CCmdUI* pCmdUI)
{
	switch( pCmdUI->m_nID) 
	{
	case ID_SYSTEM_DISCONNECTHOST:
		{
			pCmdUI->Enable(CPublic::Instance()->GetDataSource()->IsOnline());
		}
		break;   
	case ID_SYSTEM_CONNECTTOHOST:
		{
			pCmdUI->Enable(!CPublic::Instance()->GetDataSource()->IsOnline());
		}
		break;
	case ID_NORMALORDER:
		{
			int iMarketType = CDateManage::Instance()->GetMarketType();
			if (iMarketType == 0)pCmdUI->Enable(FALSE);
			else pCmdUI->Enable(TRUE);
		}
		break;
	case ID_SYSTEM_PRICEORDER:
	case ID_SYSTEM_NEWORDER:
		{
			int iMarketType = CDateManage::Instance()->GetMarketType();
			if (iMarketType == 0)
			{
				CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));
				if(NULL == pView)return;
				SCodeInfo* pCode= pView->GetCurStock();
				const StuSymbolData* pSymbol = CPublic::Instance()->GetStockInfo(pCode);
				if(pSymbol&&(pSymbol->cTrade == SYMBOLTYPE_TRADEPRODUCT)&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
				{
					pCmdUI->Enable(TRUE);
				}
				else
				{
					pCmdUI->Enable(FALSE);
				}
			}
			else
			    pCmdUI->Enable(FALSE);
		}
		break;
	case ID_TRADE_INMONEY:
	case ID_TRADE_OUTMONEY:
		{
			if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
			{
				pCmdUI->Enable(TRUE);
			}
			else
			{
				pCmdUI->Enable(FALSE);
			}
		}
		break;
	case ID_TRADE_SIGN:
	case ID_TRADE_TERMINATE:
		{
			if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
			{
				char cCCode[10] = {0};
				int iRet = CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus(cCCode);
				if(cCCode[0]=='G'){pCmdUI->Enable(FALSE);break;};
				if (cCCode[0]=='4' || cCCode[0]=='C'){pCmdUI->Enable(FALSE);break;};
				if(iRet)
				{
					if(pCmdUI->m_nID == ID_TRADE_TERMINATE)
					{
						pCmdUI->Enable(TRUE);
						break;
					}					
				}
				else
				{
					if(pCmdUI->m_nID == ID_TRADE_SIGN)
					{
						pCmdUI->Enable(TRUE);
						break;
					}
				}
				
			}
			pCmdUI->Enable(FALSE);
		}
		break;
	case ID_TRADE_HOLD:
	case ID_TRADE_PRODUCT:
		{
			int iMarketType = CDateManage::Instance()->GetMarketType();
			if (iMarketType == 0||iMarketType == 1)
			{
				pCmdUI->Enable(TRUE);
			}
			else
			{
				pCmdUI->Enable(FALSE);
			}
		}
		break;
	default:
		{		
			if((pCmdUI->m_nID >ID_ANALYSIST_BEGIN)&&(pCmdUI->m_nID <ID_ANALYSIST_END))
			{
				CDlgTrendChart* pView = (CDlgTrendChart*)(CPublic::Instance()->GetView(10));
				if(pView)pView->OnUpdateUserCmd(pCmdUI);
			}

			else
			{
				pCmdUI->Enable(TRUE);
			}			
		}
		break;
	}
}

void CMainFrame::ShowDrawLineToolBar()
{
	if(!m_bShowDrawLineBar)
	{
		m_bShowDrawLineBar = TRUE;
		m_wndDrawLineBar.ShowPane(TRUE,FALSE,TRUE);
	}
	else
	{
		m_bShowDrawLineBar = FALSE;
		m_wndDrawLineBar.ShowPane(FALSE,FALSE,FALSE);
	}
}

LRESULT CMainFrame::OnTradeMessage(WPARAM wParam,LPARAM lParam)
{
	HWND hWnd = CPublic::Instance()->GetLoginWnd();
	if(MAINFRAME_LINKMSG == wParam)
	{
		if(MAINFRAME_LINKSTATUS_LINKED == lParam)
		{
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELINKSUCCESS,0);
			ResetStatusBarLinkStatus(3,TRUE);

		}
		else if( MAINFRAME_LINKSTATUS_UNLINK == lParam)
		{
			if(m_nReconnectCount < m_nTradeReconnectCount)return 0;
			ResetStatusBarLinkStatus(3,FALSE);			
			CPublic::Instance()->GetTradeDll()->m_pUnLink();
			CPublic::Instance()->GetDataSource()->UnLink();
			if(!((CKOAClientApp*)AfxGetApp())->AskLoginDlg())
			{
				ExitMainWnd(1);
				return 0;
			}	
		}
		else if(MAINFRAME_LINKSTATUS_RELINK == lParam)
		{
			ResetStatusBarLinkStatus(3,FALSE);
		}
	}
	else if(MAINFRAME_PWDVALIDTEMSG == wParam)
	{
		if(MAINFRAME_PWDVALIDSTATUS_OK == lParam)
		{
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINSUCCESS,0);
			CPublic::Instance()->InitQuote(AfxGetMainWnd()->GetSafeHwnd());
		}
		else
		{
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINFAILED,lParam);
		}
	}
	else if(MAINFRAME_RELOGINMSG ==wParam)
	{
		OnMenuInfo(ID_SYSTEM_DISCONNECTHOST);
		OnMenuInfo(ID_SYSTEM_CONNECTTOHOST);
	}
	else if(MAINFRAME_EXIT == wParam)
	{
		ExitMainWnd(0);
	}
	return 0;
}
void CMainFrame::Save2Pic()
{
	CClientDC SHDC(this);
	CPaintDC   dc(this);
	CDC memDC;
	CRect rect;
	GetClientRect(rect);

	memDC.CreateCompatibleDC(&SHDC);
	CBitmap bm;
	int Width = rect.Width();//
	int Height = rect.Height();//
	bm.CreateCompatibleBitmap(&SHDC, Width, Height);
	CBitmap*  pOld = memDC.SelectObject(&bm);
	memDC.BitBlt(0, 0, Width, Height, &SHDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
	BITMAP  btm;
	bm.GetBitmap(&btm);
	DWORD  size = btm.bmWidthBytes * btm.bmHeight;
	LPSTR lpData =(LPSTR)GlobalAllocPtr(GPTR, size);
	BITMAPFILEHEADER   bfh;
	/////////////////////////////////////////////
	BITMAPINFOHEADER  bih;
	bih.biBitCount = btm.bmBitsPixel;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biHeight = btm.bmHeight;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = size;
	bih.biWidth = btm.bmWidth;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	GetDIBits(dc,bm,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType = ((WORD)('M'<< 8)|'B');
	bfh.bfSize = 54 + size;
	bfh.bfOffBits = 54;

	CFileDialog dlg(false,_T("BMP"),_T("*.bmp"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("*.bmp|*.bmp|*.*|*.*|"));
	if (dlg.DoModal()==IDOK)
	{
		CFile  bf;
		CString ss=dlg.GetPathName();
		if(bf.Open(ss, CFile::modeCreate | CFile::modeWrite))
		{
			bf.Write(&bfh, sizeof(BITMAPFILEHEADER));
			bf.Write(&bih, sizeof(BITMAPINFOHEADER));
			bf.Write(lpData, size);
			bf.Close();
		}
		GlobalFreePtr(lpData);
	}
}

void CMainFrame::RegHotKey()
{
	::RegisterHotKey(this->GetSafeHwnd() ,0x7705,MOD_ALT,UINT('K'));
}

void CMainFrame::UnRegHotKey()
{
	UnregisterHotKey(this->GetSafeHwnd(), 0x7705);
}

void CMainFrame::ShowMainFrm()
{
	ShowWindow(SW_SHOW);
	m_bShowState = TRUE;
	Shell_NotifyIcon(NIM_DELETE, &m_SkyNID);
}

void CMainFrame::HideMainFrm()
{
	ShowWindow(SW_HIDE);
	m_bShowState = FALSE;
	m_SkyNID.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_SkyNID.hWnd = this->m_hWnd;
	m_SkyNID.uID = IDR_MAINFRAME;
	m_SkyNID.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	m_SkyNID.dwState = NIS_SHAREDICON;
	m_SkyNID.uCallbackMessage = WM_TASKMANAGE;
	m_SkyNID.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy(m_SkyNID.szTip, (LPCTSTR)CPublic::Instance()->GetProductName());
	Shell_NotifyIcon(NIM_ADD, &m_SkyNID);
}


LRESULT CMainFrame::OnTaskManage(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case WM_RBUTTONUP:
		{
			CMenu * pTaskMenu, popMenu;
			CPoint pMousePosition;

			popMenu.LoadMenu(IDR_RIGHTMENU); 
			
			pTaskMenu = popMenu.GetSubMenu(4);
			CLanguageInterface::Instance()->InitRightMenu(pTaskMenu,4);
			GetCursorPos(&pMousePosition);
			SetForegroundWindow();
			pTaskMenu->TrackPopupMenu(TPM_RIGHTALIGN,pMousePosition.x,pMousePosition.y,this);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			ShowMainFrm();
		}
	  break;
	default:
		break;
	}
	return 0;
}
LRESULT CMainFrame::OnKeyGhostMessageZYQ(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case KEYGHOST_COMMAND:
		{
			OnMenuInfo((UINT)lParam);
		}
		break;
	case KEYGHOST_STOCK:
	case KEYGHOST_FORMULA:
		{
			CWnd *pWnd = CPublic::Instance()->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				pWnd->PostMessage(WM_KEYGHOSTMSG,wParam,lParam);
			}
		}
		break;
	default:
		break;
	}
	return 0;
}
LRESULT CMainFrame::OnKeyGhostMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case KEYGHOST_STOCK:
		{ 	
			SCodeInfo* pCodeInfo = (SCodeInfo*)lParam;	
			if(pCodeInfo == NULL)
				return 0;
			CPublic::Instance()->ChangeStockEvent(pCodeInfo,RealObj);

			delete pCodeInfo;
		}
		break;
	case KEYGHOST_COMMAND:
		{
			OnMenuInfo((UINT)lParam);
		}
		break;
	case KEYGHOST_FORMULA:
		{
			CPublic::Instance()->OnFormulaCommand((char*)lParam);
		}
		break;
	default:
		break;
	}
	return 0;
}


void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CFrameWndEx::OnChar(nChar, nRepCnt, nFlags);
	if(!CPublic::Instance()->NeedKeyGhost())return;

	if((nChar>='0')&&(nChar<='9') 
		|| (nChar>='A')&&(nChar<='Z')
		|| (nChar>='a')&&(nChar<='z'))
	{
		{
			CDlgMainClient * pWnd = (CDlgMainClient *)CPublic::Instance()->GetView(11,2);
			if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			{
				int iMarket = pWnd->GetCurMarket();
				if (iMarket >= 0)
				{
					CInput inputDlg(this);
					inputDlg.m_PosWnd = this;
					inputDlg.cKey = nChar;
					inputDlg.m_uiMarket = iMarket;
					inputDlg.DoModal();
				}
			}
		}
	}
}
void CMainFrame::ExitMainWnd(int iDs)
{
	UnRegHotKey();
	CPublic::Instance()->UnLinkTrade();
	CPublic::Instance()->GetDataSource()->UnLink();
	if (iDs == 0)
	{
		::MessageBox(this->GetSafeHwnd(),CLanguageInterface::Instance()->GetStr(1133,1),CLanguageInterface::Instance()->GetStr(2038,1),MB_OK|MB_ICONINFORMATION);//账户下线，请重新登录！
	}
	CInputKey::ExInstance();
	CInterfaceManager::ExitInstance();
	CPublic::Instance()->GetTradeDll()->m_pDestoryDlg();

	if(m_bFullScreenMode)
		FullScreenMode(FALSE);//

	CFrameWndEx::OnClose();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (1025 == nIDEvent)
	{
		CView *pView = GetActiveView();
		if (pView)
		{
			pView->PostMessage(WM_NOTIFY_LOCK,1,0);

			KillTimer(1025);
		}
	}
	if (EVENT_REFRESH == nIDEvent)
	{
		m_bRefresh = TRUE;
		KillTimer(EVENT_REFRESH);
	}
	if(m_uiCheckConnectTimer == nIDEvent)
	{
		DWORD   flags; 
		if(InternetGetConnectedState(&flags,0))
		{
			if(!m_bInternetStatus)
			{
				m_bInternetStatus = TRUE;
			}
		}
		else
		{
			if(m_bInternetStatus)
			{
				ResetStatusBarLinkStatus(1,FALSE);
				ResetStatusBarLinkStatus(3,FALSE);
				m_bInternetStatus = FALSE;
			}
		}
	}	
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::ResetStatusBarUserInfo()
{
	CString strUser,strPwd;
	BOOL bSave,bSaveAccount;
	BOOL bNeedCFCA = FALSE;
	BOOL bSupportCFCA = TRUE;
	CPublic::Instance()->GetLoginInfo(strUser,strPwd,bSave,bSaveAccount,bNeedCFCA,bSupportCFCA);
	CString strShowUser;
	strShowUser = CLanguageInterface::Instance()->GetStr(10032)+ strUser;
	m_wndStatusBar.SetPaneText(5,strShowUser);

	bool bUsetStatus;
	bUsetStatus = CPublic::Instance()->GetTradeDll()->m_pGetUserStatus();
	CString strUserStatus;
	//strUserStatus = bUsetStatus?CLanguageInterface::Instance()->GetStr(10033):CLanguageInterface::Instance()->GetStr(10034);
	strUserStatus = CLanguageInterface::Instance()->GetStr(10033);
	strUserStatus =CLanguageInterface::Instance()->GetStr(10035)+strUserStatus;
	m_wndStatusBar.SetPaneText(6,strUserStatus);

	char szUserName[100];
	memset(szUserName,0,sizeof(szUserName));
	CPublic::Instance()->GetTradeDll()->m_pGetUserInfo(szUserName,0);
	CString strUserName;
	strUserName = CLanguageInterface::Instance()->GetStr(10036);
	strUserName += szUserName;
	m_wndStatusBar.SetPaneText(7,strUserName);
	m_wndStatusBar.SetPaneTextColor(7,RGB(0,0,255));

	CPublic::Instance()->SendUserChangeMsg(TRUE);
}

void CMainFrame::ResetStatusBarLinkStatus(int nIndex,BOOL bLinked)
{
	m_wndStatusBar.SetPaneIcon(nIndex+1,bLinked?m_bitmapLinked:m_bitmapUnlink);
	CPublic::Instance()->SendLinkStatus(nIndex,bLinked);
}
void CMainFrame::OnUpdateStatusBarPanes(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CMainFrame::ResetStatusBar(void)
{
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL,120);	
	m_wndStatusBar.SetPaneInfo(2, IDS_SPERATOR_QUOTELINKSTATUS, SBPS_STRETCH, 0);	
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL, 120);
	m_wndStatusBar.SetPaneInfo(4, IDS_SPERATOR_TRADELINKSTATUS, SBPS_STRETCH, 0);	
	m_wndStatusBar.SetPaneInfo(5, IDS_SPERATOR_USERID, SBPS_NORMAL, 160);
	m_wndStatusBar.SetPaneInfo(6, IDS_SPERATOR_USERSTATUS, SBPS_NORMAL, 120);
	m_wndStatusBar.SetPaneInfo(7, IDS_SPERATOR_USERNAME, SBPS_NORMAL, 160);
	m_wndStatusBar.SetPaneText(1,CLanguageInterface::Instance()->GetStr(10022));	
	m_wndStatusBar.SetPaneText(3,CLanguageInterface::Instance()->GetStr(10023));	

	m_bitmapUnlink.LoadBitmap(IDB_BITMAP_UNLINK);
	m_bitmapLinked.LoadBitmap(IDB_BITMAP_LINKED);

	ResetStatusBarLinkStatus(1,TRUE);
	ResetStatusBarLinkStatus(3,TRUE);
}

#define CACU(a) ((a>0)?a:0)
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// Create a rgn without 3 pixels border
	CRect wrc;
	GetWindowRect(&wrc);
	// we got right wrc {top=58 bottom=453 left=44 right=530}
	wrc.OffsetRect(-wrc.left,-wrc.top); 

	int offx = GetSystemMetrics(SM_CXFRAME);
	int offY = GetSystemMetrics(SM_CYFRAME);

	wrc.DeflateRect(offx,offY,offx,offY);
	//
	CRgn rgn;
	BOOL bl=rgn.CreateRectRgnIndirect(&wrc);
	if(bl) SetWindowRgn(rgn,TRUE);
	rgn.Detach();

	UpdateSysBtnStat(nType);

	//CView*	pView = CPublic::Instance()->GetView(VIEWINFO_STOCK);
	//if (pView!= NULL)
	//{
	//	pView->Invalidate();
	//	pView->UpdateWindow();
	//}

	//CRect rectClient;
	//GetClientRect(&rectClient);
	//if (IsWindow(m_DlgMainClient.GetSafeHwnd()))
	//{
	//	m_DlgMainClient.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	//	m_DlgMainClient.SetFocus();
	//}	

	//CRect rectClient;
	//GetClientRect(&rectClient);
	//if (IsWindow(m_dlgMainWnd.GetSafeHwnd()))
	//{
	//	m_dlgMainWnd.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	//	m_dlgMainWnd.SetFocus();
	//}	
}


void CMainFrame::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFrameWndEx::OnNcPaint()
	CDC *pDc;
	pDc = GetWindowDC();
	if (pDc == NULL)	
		return;
	//////////
	DrawNcPanit(pDc);


	ReleaseDC(pDc);
}
void CMainFrame::DrawNcPanit(CDC *pDc)
{
	CRect rcWnd,rcClt,rcLeft;
	GetWindowRect(rcWnd);
	//GetClientRect(rcClt);
	int offx = 7;
	int OffT = 6;
	int OffR = 7;
	int OffB = 7;
	rcClt = rcWnd;
	rcClt.OffsetRect(-rcClt.left,-rcClt.top);
	rcClt.top += (m_iCaptionWith+2+OffT);
	rcClt.left += (1+offx);
	rcClt.right -= (1+OffR);
	rcClt.bottom -= (1+OffB);
	pDc->ExcludeClipRect(rcClt);

	CDC cdMem;
	cdMem.CreateCompatibleDC(pDc);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(pDc->m_hDC, rcWnd.Width(), rcWnd.Height());
	cdMem.SelectObject(hMemBitmap);

	DrawCaption(&cdMem);
	DrawSysButtons( &cdMem );
	DrawBorder(&cdMem);

	::BitBlt(pDc->m_hDC,0,0,rcWnd.Width(),rcWnd.Height(),
		cdMem,0,0,SRCCOPY);
	DeleteObject(hMemBitmap);
	cdMem.DeleteDC();
}

void CMainFrame::ReleasImg()
{
	if (m_pImgBk)
	{
		delete m_pImgBk;
		m_pImgBk = NULL;
	}
	if (m_pImglogo)
	{
		delete m_pImglogo;
		m_pImglogo = NULL;
	}
	if (m_pImgBorderL)
	{
		delete m_pImgBorderL;
		m_pImgBorderL = NULL;
	}
	if (m_pImgBorderR)
	{
		delete m_pImgBorderR;
		m_pImgBorderR = NULL;
	}
	if (m_pImgBorderB)
	{
		delete m_pImgBorderB;
		m_pImgBorderB = NULL;
	}
	if (m_pImgCaption)
	{
		delete m_pImgCaption;
		m_pImgCaption = NULL;
	}
}

void CMainFrame::InitCaption()
{
	TCHAR exeFullPath[MAX_PATH]; 
	CString strPath;
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strPath = exeFullPath;
	int pos = strPath.ReverseFind(('\\'));
	CString strImgDirc = strPath.Left(pos);
	CString strImgPath = strImgDirc +  _T("\\interface\\");
	USES_CONVERSION;
	ReleasImg();
	m_pImgBk = Image::FromFile(T2W(strImgPath + _T("TitleRight.png")));
	m_pImglogo = Image::FromFile(T2W(strImgPath + _T("Logo.png")));
	//if (m_pImgBorderL)1 = m_pImgBorderL->GetWidth();
	//if (m_pImgBorderR)1 = m_pImgBorderR->GetWidth();
	//if (m_pImgBorderB)1 = m_pImgBorderB->GetHeight();
}

void CMainFrame::DrawCaption(CDC *pDC)
{
	ASSERT(pDC != NULL);

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);

	int OffT = 6;
	int iCaptionHight = m_iCaptionWith;//+2 +OffT;

	CRect rcCaption;
	rcCaption.top = rcWindow.top+2 +OffT;
	rcCaption.left = rcWindow.left;
	rcCaption.right = rcWindow.right;
	rcCaption.bottom = rcCaption.top + iCaptionHight ;

	Gdiplus::Bitmap cachBmp(rcCaption.right-rcCaption.left,rcCaption.bottom-rcCaption.top);

	Graphics graphicsDraw(pDC->m_hDC);
	Graphics graphics(&cachBmp);
	CSkinAttribute_General *pSkin = (CSkinAttribute_General *)CDateManage::Instance()->GetSkinAttribute(6);
	SolidBrush sbrBk(pSkin->clrBkAll/*Color((ARGB)m_clrCaptionBk)*/);
	graphics.FillRectangle(&sbrBk,Rect(0,0,rcCaption.Width(),rcCaption.Height()));
	if (m_pImglogo)graphics.DrawImage(m_pImglogo,Rect(0,0,m_pImglogo->GetWidth(),iCaptionHight));
	if (m_pImgBk)graphics.DrawImage(m_pImgBk,Rect(rcCaption.right-m_pImgBk->GetWidth(),0,m_pImgBk->GetWidth(),iCaptionHight));

	if (m_pImgCaption)
	{
		delete m_pImgCaption;
		m_pImgCaption = NULL;
	}
	m_pImgCaption = cachBmp.Clone(0,0,cachBmp.GetWidth(),cachBmp.GetHeight(),cachBmp.GetPixelFormat());
	graphicsDraw.DrawImage(&cachBmp,rcCaption.left,rcCaption.top);
	graphicsDraw.ReleaseHDC(pDC->m_hDC);
}

void CMainFrame::DrawBorder(CDC *pDC)
{
	//return;
	CRect rcWindow,rcLeft,rcBottom,rcRight,rcTop;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	int offx = 7;
	int OffT = 6;
	int OffR = 7;
	int OffB = 7;

	int iCaptionHeight = m_iCaptionWith+2+OffT;

	rcLeft.top = rcWindow.top + iCaptionHeight;
	rcLeft.left = rcWindow.left;
	rcLeft.right = rcWindow.left + 1+offx;
	rcLeft.bottom = rcWindow.bottom;

	rcBottom.top = rcWindow.bottom - 1-OffB;
	rcBottom.left = rcWindow.left;
	rcBottom.right = rcBottom.left + rcWindow.Width();
	rcBottom.bottom = rcBottom.top + 1;

	rcRight.top = rcWindow.top + iCaptionHeight;
	rcRight.left = rcWindow.right - 1-OffR;
	rcRight.right = rcWindow.right;
	rcRight.bottom = rcWindow.bottom;

	if ( 2 > 0)
	{
		rcTop.top = rcWindow.top;
		rcTop.left = rcWindow.left;
		rcTop.right = rcWindow.right;
		rcTop.bottom = rcTop.top + 2 +OffT ;
	}


	Gdiplus::Bitmap cachBmpL(rcLeft.Width(),rcLeft.Height());
	Gdiplus::Bitmap cachBmpB(rcBottom.Width(),rcBottom.Height());
	Gdiplus::Bitmap cachBmpR(rcRight.Width(),rcRight.Height());
	Gdiplus::Bitmap cachBmpT(rcTop.Width(),rcTop.Height());

	Graphics graphicsDraw(pDC->m_hDC);
	Graphics graphicsL(&cachBmpL);
	Graphics graphicsB(&cachBmpB);
	Graphics graphicsR(&cachBmpR);
	Graphics graphicsT(&cachBmpT);

	SolidBrush sbrushBk(Color(255,31,34,45));

//	graphicsL.DrawImage(&imgLeft,Rect(0,0,rcLeft.Width(),rcLeft.Height()));
	if (m_pImgBorderL)
	{
		graphicsL.DrawImage(m_pImgBorderL,Rect(rcLeft.right-1,0,1,rcLeft.Height()+2));
	}
	else
	{
		graphicsL.FillRectangle(&sbrushBk,Rect(rcLeft.right-1,0,1,rcLeft.Height()));
	}
	if (m_pImgBorderR)
	{
		graphicsR.DrawImage(m_pImgBorderR,Rect(0,0,1,rcRight.Height()));
	}
	else
	{
		graphicsR.FillRectangle(&sbrushBk,Rect(0,0,1,rcRight.Height()));
	}
	if (m_pImgBorderB)
	{
		graphicsB.DrawImage(m_pImgBorderB,Rect(0,0,rcBottom.Width(),1));
	}
	else
	{
		graphicsB.FillRectangle(&sbrushBk,Rect(0,0,rcBottom.Width(),1));
	}
	graphicsT.FillRectangle(&sbrushBk,Rect(0,0,rcTop.Width(),rcTop.Height()));

	if (1 > 0)graphicsDraw.DrawImage(&cachBmpL,rcLeft.left,rcLeft.top);
	if (1 > 0)graphicsDraw.DrawImage(&cachBmpB,rcBottom.left,rcBottom.top);
	if (1 > 0)graphicsDraw.DrawImage(&cachBmpR,rcRight.left,rcRight.top);
	if (2 > 0)graphicsDraw.DrawImage(&cachBmpT,rcTop.left,rcTop.top);

	graphicsDraw.ReleaseHDC(pDC->m_hDC);

}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rcClt;
	//GetClientRect(&rcClt);
	////pDC->FillSolidRect(rcClt.left,rcClt.top,rcClt.Width(),rcClt.Height(),RGB(31,34,41));
	//CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CFrameWndEx::OnEraseBkgnd(pDC);
}

// void CMainFrame::ActivateFrame(int nCmdShow )
// {
// 	CRect wrc;
// 	GetWindowRect(&wrc);
// 	wrc.OffsetRect(-wrc.left,-wrc.top);
// 	// we got right wrc {top=58 bottom=453 left=44 right=530}
// 	///////
// 	int offx = 7;
// 	int OffT = 6;
// 	int OffR = 7;
// 	int OffB = 7;
// 	wrc.DeflateRect(offx,OffT,OffR,OffB);
// 	/////////////
// 	
// 	//wrc.DeflateRect(3,3);
// 	//
// 	CRgn rgn;
// 	BOOL bl=rgn.CreateRectRgnIndirect(&wrc);
// 	if(bl) SetWindowRgn(rgn,TRUE);
// 	rgn.Detach();
// }

BOOL CMainFrame::OnNcActivate(BOOL bActive)
{
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bRet = CFrameWndEx::OnNcActivate(bActive);
	if (bActive)
	{
		CDC *pDC;
		pDC = GetWindowDC();
		if (pDC == NULL)	//获取DC失败,则不画
			return bRet;//CFrameWndEx::OnNcActivate(bActive);
		//DrawCaption(pDC);
		//DrawSysButtons( pDC );
		//DrawBorder(pDC);
		DrawNcPanit(pDC);

		ReleaseDC(pDC);
	}
	return bRet;
}


void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CFrameWndEx::OnNcCalcSize(bCalcValidRects, lpncsp);
	if( GetStyle()&WS_CHILD ){
		CFrameWndEx::OnNcCalcSize( bCalcValidRects,lpncsp );
		return;
	}
	RECT rect = lpncsp->rgrc[0];

	DWORD dwStyle = GetWindowLong( m_hWnd,GWL_STYLE );
	if( dwStyle & WS_CAPTION ){
		lpncsp->rgrc[0].top += (m_iCaptionWith);
	}
// 	if( dwStyle & WS_BORDER ){
//  		lpncsp->rgrc[0].top += -6;	
//  		lpncsp->rgrc[0].left += -2;
//  		lpncsp->rgrc[0].right -= -7;
//  		lpncsp->rgrc[0].bottom -= -7;
// 	}

	CFrameWndEx::OnNcCalcSize(bCalcValidRects, lpncsp);

}


LRESULT CMainFrame::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcWnd;
	GetWindowRect(&rcWnd);
	rcWnd.bottom = rcWnd.top + 38;
	if (rcWnd.PtInRect(point))
	{
		return HTCAPTION;
	}

	return CFrameWndEx::OnNcHitTest(point);
}

void CMainFrame::InitCtrl()
{
	InitCaption();
	CreateSysButton();
}

void CMainFrame::CreateSysButton()
{
	if (IsWindow(m_sysToolTipCtrl.GetSafeHwnd()))
	{
		return;
	}

	m_sysToolTipCtrl.Create(this);
	m_sysToolTipCtrl.SetDelayTime(250);

	CString strImgPath = CDateManage::Instance()->GetImgPath();
	USES_CONVERSION;

	m_sysCloseBtn.Create(CRect(0, 0,50,22 ),
		this, ID_SC_CLOSE,strImgPath+_T("CLOSE.bmp")/*, m_strImgColse,m_pImgTitle*/ );

	m_sysToolTipCtrl.AddTool(this, _T("关闭"),
		&CRect(0, 0, 1, 1),
		ID_SC_CLOSE);

	m_sysMinBtn.Create(CRect(0, 0,30,22 ),
		this, ID_SC_MIN,strImgPath+_T("MIN.bmp")/*, m_strImgColse,m_pImgTitle*/ );

	m_sysToolTipCtrl.AddTool(this, _T("最小化"),
		&CRect(0, 0, 1, 1),
		ID_SC_MIN);
	
	m_sysMaxBtn.Create1(CRect(0, 0,30,22 ),
		this, ID_SC_MAX, ID_SC_RESTORE, 
		strImgPath+_T("MAX.bmp"),NULL,strImgPath+_T("Restore.bmp"));

	m_sysToolTipCtrl.AddTool(this, _T("最大化"),
		&CRect(0, 0, 1, 1),
		ID_SC_MAX);

	CMenu popMenu;
	CMenu* pMenu;
	popMenu.LoadMenu(IDR_MAINFRAME); 
	int iSubNum = popMenu.GetMenuItemCount();
	m_vecMenu.clear();
	int iOffID = 0;
	CString strText;
	int iSubItemNum = 0;
	CCustomUser *pCusUser = CPublic::Instance()->GetCustomUser();
	int i = 0;
	int nMenuId = 0;

	for (; i < iSubNum;i++)
	{
		WWndLessButton *pccTitleBtn = new WWndLessButton;
		pccTitleBtn->Create(CRect(0, 0,60,24 ),
			this, ID_TITLEMENUMIN+(iOffID++)/*, m_strImgColse,m_pImgTitle*/ );
		popMenu.GetMenuString(i, strText, MF_BYPOSITION);
		pccTitleBtn->SetDisplayText(strText);
		m_vecMenu.push_back(pccTitleBtn);
		pMenu = popMenu.GetSubMenu(i);
		iSubItemNum = pMenu->GetMenuItemCount();
		if (iSubItemNum <= 0) continue;
		pccTitleBtn->CreateMenuWnd(this,i,150,25,0);
		if (i == 1&&pCusUser)
		{
			if (pCusUser->m_iType == 1)
				pccTitleBtn->AddMenu(((CCustomUser_Type1*)pCusUser)->m_strCustomMenuText,ID_CUSTOMTYPE_1);
		}
		for (int x = 0; x < iSubItemNum; x++)
		{
			nMenuId = pMenu->GetMenuItemID(x);

			if (nMenuId == ID_HELP_EDUCATION && !CPublic::Instance()->IsShowEducationMenum())
				continue;

			if (nMenuId == ID_HELP_HOW2USE && !CPublic::Instance()->IsShowUseHelpMenum())
				continue;

			pMenu->GetMenuString(x, strText, MF_BYPOSITION);
			pccTitleBtn->AddMenu(strText, nMenuId);
		}
	}
	/////////
	///////////
	std::vector<SKININFO> vecSkin;
	std::vector<SKININFO>::iterator itVec;
	CDateManage::Instance()->GetSkinInfo(vecSkin);
	if (vecSkin.size() > 1)
	{
		WWndLessButton *pccTitleBtn = new WWndLessButton;
		pccTitleBtn->Create(CRect(0, 0,60,24 ),
			this, ID_TITLEMENUMIN+(iOffID++));
		pccTitleBtn->SetDisplayText(_T("换肤"));
		m_vecMenu.push_back(pccTitleBtn);
		pccTitleBtn->CreateMenuWnd(this,i,150,25,0);
		for (itVec = vecSkin.begin(); itVec != vecSkin.end(); itVec++)
		{
			pccTitleBtn->AddMenu(itVec->strSkinName,ID_SKIN_BEGIN+itVec->iId);
		}
	}
	////////////
	////////////
	OnSetSysButtonPos();
}

BOOL CMainFrame::OnSetSysButtonPos()
{
	if (!IsWindow(m_sysToolTipCtrl.GetSafeHwnd()))
	{
		return FALSE;
	}

	DWORD dwStyle = GetStyle();

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	int OffT = 6;
	int OffR = 7;
	int offx = 7;

	int OffSetW = -1;
	int OffSetH = -1;
	int OffSetSp = 0;

	OffSetW += OffR;
	OffSetH += OffT;
	CSize szBtnBmp = m_sysCloseBtn.GetImageSize();
	CPoint ptOrg(rcWindow.Width() - szBtnBmp.cx - OffSetW,
		2+OffSetH);

	ptOrg.Offset(rcWindow.TopLeft());
	ScreenToClient(&ptOrg);
	CRect rcSysCloseBtn( ptOrg.x,ptOrg.y,ptOrg.x+szBtnBmp.cx,ptOrg.y+szBtnBmp.cy );

	m_sysCloseBtn.Move( rcSysCloseBtn ); 
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_CLOSE, &rcSysCloseBtn);

	szBtnBmp = m_sysMaxBtn.GetImageSize();
	ptOrg.Offset(-szBtnBmp.cx-OffSetSp, 0);
	CRect rcSysMaxBtn( ptOrg.x,ptOrg.y,ptOrg.x+szBtnBmp.cx,ptOrg.y+szBtnBmp.cy );
	m_sysMaxBtn.Move(rcSysMaxBtn); 
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_MAX, &rcSysMaxBtn);

	szBtnBmp = m_sysMinBtn.GetImageSize();
	ptOrg.Offset(-szBtnBmp.cx-OffSetSp, 0);
	CRect rcSysMinBtn( ptOrg.x,ptOrg.y,ptOrg.x+szBtnBmp.cx,ptOrg.y+szBtnBmp.cy );
	m_sysMinBtn.Move(rcSysMinBtn); 
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_MIN, &rcSysMinBtn);

	int iOffMenuSpa = 5;
	int iOffL = 90;
	int iOffMenuH = 2;
	iOffL+= offx;
	iOffMenuH += OffT;
	CPoint ptMenu( iOffL,
		2+iOffMenuH);
	ptMenu.Offset(rcWindow.TopLeft());
	ScreenToClient(&ptMenu);

	std::vector<WWndLessButton*>::iterator vIt;
	for (vIt = m_vecMenu.begin();vIt != m_vecMenu.end();vIt++)
	{
		WWndLessButton *pccTitleBtn = *vIt;
		szBtnBmp = pccTitleBtn->GetImageSize();
		CRect rcBtn( ptMenu.x,ptMenu.y,ptMenu.x+szBtnBmp.cx,ptMenu.y+szBtnBmp.cy );
		pccTitleBtn->Move(rcBtn);
		ptMenu.Offset(szBtnBmp.cx+iOffMenuSpa, 0);
	}

	return TRUE;
}

void CMainFrame::DrawSysButtons(CDC *pDC)
{

	if (!OnSetSysButtonPos())return;

	DWORD dwStyle = GetStyle();
	if (0)
		return;

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	CRect rcsysCloseBtn = m_sysCloseBtn.GetRect();
	m_sysCloseBtn.DrawButton(CPoint(rcsysCloseBtn.left,rcsysCloseBtn.top), pDC); 

	CRect rcsysMaxBtn = m_sysMaxBtn.GetRect();
	m_sysMaxBtn.DrawButton(CPoint(rcsysMaxBtn.left,rcsysMaxBtn.top), pDC); 

	CRect rcsysMinBtn = m_sysMinBtn.GetRect();
	m_sysMinBtn.DrawButton(CPoint(rcsysMinBtn.left,rcsysMinBtn.top), pDC); 

	CRect rcMemu;
	std::vector<WWndLessButton*>::iterator vIt;
	for (vIt = m_vecMenu.begin();vIt != m_vecMenu.end();vIt++)
	{
		WWndLessButton *pccTitleBtn = *vIt;
		rcMemu = pccTitleBtn->GetRect();
		pccTitleBtn->SetBkImg(m_pImgCaption);
		pccTitleBtn->DrawButton(CPoint(rcMemu.left,rcMemu.top), pDC); 
	}
}

void CMainFrame::UpdateSysBtnStat(UINT nType)
{
	if (!IsWindow(m_sysToolTipCtrl.GetSafeHwnd()))return;

	if (nType == SIZE_MAXIMIZED && !m_sysMaxBtn.m_bMaximized)
	{

		if (!m_sysMaxBtn.m_bMinimized)
			m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = TRUE;
		m_sysMaxBtn.m_bMinimized = FALSE;
		m_sysToolTipCtrl.UpdateTipText(_T("还原"),this,ID_SC_MAX);
		SendMessage(WM_NCPAINT, 0, 0);
		CRect rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		rt.left-=8;
		rt.top-=8;
		rt.right+=8;
		rt.bottom+=8;
		MoveWindow(rt);
	}

	if (nType == SIZE_MINIMIZED
		&& !m_sysMaxBtn.m_bMinimized)
	{
		if (!m_sysMaxBtn.m_bMaximized)
			m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = FALSE;
		m_sysMaxBtn.m_bMinimized = TRUE;
		m_sysToolTipCtrl.UpdateTipText(_T("还原"),this,ID_SC_MAX);
		//更新按钮
		SendMessage(WM_NCPAINT, 0, 0);
	}

	if (nType == SIZE_RESTORED
		&& (m_sysMaxBtn.m_bMaximized || m_sysMaxBtn.m_bMinimized))
	{
		m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = FALSE;
		m_sysMaxBtn.m_bMinimized = FALSE;	
		m_sysToolTipCtrl.UpdateTipText(_T("最小化"),this,ID_SC_MAX);
		///////////////
		//CRect rcWindow;
		//GetWindowRect(&rcWindow);
		static BOOL bFlag = FALSE;
		
		if (!bFlag)
		{
			bFlag = TRUE;
			CRect rt;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

			int isysFramex = 60;
			rt.left+=isysFramex;
			rt.top+=isysFramex;
			rt.right-=isysFramex;
			rt.bottom-=isysFramex;
			MoveWindow(rt);
		}
		/////////////////////
		SendMessage(WM_NCPAINT, 0, 0);
	}
}

void CMainFrame::OnFrameClose()
{
	ReleaseCapture();
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFrameMin()
{
	CRect rcWnd,rcEnd;
	GetWindowRect(&rcWnd);
	rcEnd = rcWnd;
	rcEnd.left = rcWnd.right - 20;
	rcEnd.top = rcWnd.bottom - 20;
	DrawAnimatedRects(IDANI_CAPTION,&rcWnd,&rcEnd);
	ShowWindow(SW_MINIMIZE);
}

void CMainFrame::OnFrameMax()
{
	ShowWindow(SW_SHOWMAXIMIZED);
	//CRect rt;
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	//MoveWindow(rt);

	//if (!m_sysMaxBtn.m_bMinimized)
	//	m_sysMaxBtn.Exchange();
	//m_sysMaxBtn.m_bMaximized = TRUE;
	//m_sysMaxBtn.m_bMinimized = FALSE;
	//m_sysToolTipCtrl.UpdateTipText(_T("还原"),this,ID_SC_MAX);
	//SendMessage(WM_NCPAINT, 0, 0);

	//UpdateSysBtnStat(SIZE_MAXIMIZED);
}

void CMainFrame::OnFrameRestore()
{
	ShowWindow(SW_RESTORE);
	//UpdateSysBtnStat(SIZE_RESTORED);
}

void CMainFrame::OnTitleMenu(UINT nID)
{
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFrameWndEx::OnSysCommand(nID, lParam);
}


//void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CFrameWndEx::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: 在此处添加消息处理程序代码
//}


void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFrameWndEx::OnNcLButtonDblClk(nHitTest, point);
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWndEx::OnActivate(nState, pWndOther, bMinimized);
}


void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	//CFrameWndEx::OnActivateApp(bActive, dwThreadID);

	//// TODO: 在此处添加消息处理程序代码
	if (bActive)
	{
		if (CPublic::Instance()->IsNeedLock())
		{
			KillTimer(1025);
		}
	}
	else
	{
		if (CPublic::Instance()->IsNeedLock())
		{
			SetTimer(1025,(1000*60*30),NULL);
		}	
	}
}
LRESULT CMainFrame::OnLoginSuccess(WPARAM wPar,LPARAM lPar)
{

	return 1;
}
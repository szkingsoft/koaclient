
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
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)//响应数据业务接口消息
	ON_MESSAGE(WM_TRADELOGININFO,OnTradeMessage)//响应交易端信息
	ON_MESSAGE(WM_TASKMANAGE,OnTaskManage)
	ON_MESSAGE(WM_REQUESTDATA,OnKeyGhostMessage)//键盘精灵消息
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_SETTINGCHANGE()
//	ON_WM_INITMENU()
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	m_nReconnectCount = 0;
	m_bFullScreenMode = FALSE;
	m_bShowDrawLineBar = FALSE;
	m_bShowState = TRUE;
	m_bReLogin = FALSE;
	m_bInternetStatus = FALSE;
	m_nTradeReconnectCount = 1;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));//CMFCVisualManagerVS2008));CMFCVisualManagerWindows7
	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	CMFCToolBar::AddToolBarForImageCollection(IDR_MAINFRAME, theApp.m_bHiColorIcons ? IDR_MAINFRAME : 0);


	EnableDocking(CBRS_ALIGN_ANY);	

	m_menuMain.LoadMenu(IDR_MAINFRAME);
	CLanguageInterface::Instance()->InitMainMenu(&m_menuMain);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	if(0 != CPublic::Instance()->GetMainViewMode())
	{	
		if(!CPublic::Instance()->GetMoneyInOutEnable())
		{
			m_menuMain.DeleteMenu(3,MF_BYPOSITION);
		}
		m_menuMain.DeleteMenu(1,MF_BYPOSITION);

	}
	else
	{
		CString strMenuAppend;
		strMenuAppend= CPublic::Instance()->GetAppendMenu();
		if(!strMenuAppend.IsEmpty())
		{
			m_menuMain.AppendMenu(MF_ENABLED|MF_STRING,ID_SYS_APPENDMENU,strMenuAppend);
		}
	}
	m_wndMenuBar.CreateFromMenu(m_menuMain.GetSafeHmenu(),TRUE,TRUE);
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	//////////////////////////////////////////////////////////////////////////
	//创建工具栏
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	CLanguageInterface::Instance()->InitToolBar(IDR_MAINFRAME,&m_wndToolBar);
	if(0 != CPublic::Instance()->GetMainViewMode())
	{	

		for(int i =0;i<11;i++)
			m_wndToolBar.RemoveButton(8);
		if(!CPublic::Instance()->GetMoneyInOutEnable())
		{
			for(int i =0;i<2;i++)
				m_wndToolBar.RemoveButton(5);
		}
	}
	m_wndToolBar.EnableTextLabels();

	m_wndToolBar.SetWindowText(CLanguageInterface::Instance()->GetStr(10027));
//	if(CPublic::Instance()->GetMoneyInOutEnable())
//		m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE,  CLanguageInterface::Instance()->GetStr(10029));

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
	//////////////////////////////////////////////////////////////////////////

	if(0 == CPublic::Instance()->GetMainViewMode())
	{//如果是其他方式不显示停靠窗口
		//画线工具TBSTYLE_FLAT|TBSTYLE_LIST,,IDT_DRAWLINE
		if (/*!m_wndDrawLineBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | 
			CBRS_FLYBY | CBRS_SIZE_DYNAMIC,IDT_DRAWLINE) ||*/
			!m_wndDrawLineBar.CreateEx(this, TBSTYLE_FLAT/* | TBSTYLE_LIST*/, WS_CHILD | WS_VISIBLE | 
			CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)||
			!m_wndDrawLineBar.LoadToolBar( IDT_DRAWLINE))
		{
			TRACE ("创建画线工具栏失败");
			return -1;
		}
		CLanguageInterface::Instance()->InitToolBar(IDT_DRAWLINE,&m_wndDrawLineBar);
		//m_wndDrawLineBar.SetSizes(CSize(42,42), CSize(32,32));
		m_wndDrawLineBar.EnableTextLabels();
		m_wndDrawLineBar.SetWindowText(CLanguageInterface::Instance()->GetStr(10028));

		m_wndDrawLineBar.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndDrawLineBar);	
	}
	else
	{

	}
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndTrade.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTrade);

	EnablePaneMenu(TRUE, NULL,NULL,ID_VIEW_TOOLBAR);
//	EnablePaneMenu(TRUE,ID_VIEW_CUSTOMIZE, CLanguageInterface::Instance()->GetStr(10029), ID_VIEW_TOOLBAR);	

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	m_wndMenuBar.SetShowAllCommands();
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
	return 0;;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	cs.style  &= ~FWS_ADDTOTITLE;  
    m_strTitle = CPublic::Instance()->GetProductName();    //设置程序标题
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	if(0 == CPublic::Instance()->GetMainViewMode())
	{
		if (!m_wndPriceInfoView.Create(CLanguageInterface::Instance()->GetStr(10024), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“类视图”窗口\n");
			return FALSE; // 未能创建
		}

		if (!m_wndPriceListView.Create(CLanguageInterface::Instance()->GetStr(10025), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“行情列表 视图”窗口\n");
			return FALSE; // 未能创建
		}
		// 允许用户定义的工具栏操作:

		m_wndPriceListView.EnableDocking(CBRS_ALIGN_ANY);
		m_wndPriceInfoView.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndPriceListView);
		CDockablePane* pTabbedBar = NULL;
		m_wndPriceInfoView.AttachToTabWnd(&m_wndPriceListView, DM_SHOW, TRUE, &pTabbedBar);

		HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_PRICELIST_VIEW_HC : IDI_PRICELIST_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndPriceListView.SetIcon(hFileViewIcon, FALSE);

		HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_PRICEINFO_WND_HC : IDI_PRICEINFO_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndPriceInfoView.SetIcon(hClassViewIcon, FALSE);
	}
	if (!m_wndTrade.Create(CLanguageInterface::Instance()->GetStr(10026), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_TRADE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI,AFX_CBRS_REGULAR_TABS,AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE))
	{
		TRACE0("未能创建交易窗口\n");
		return FALSE; // 未能创建
	}
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_TRADE_WND_HC : IDI_TRADE_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTrade.SetIcon(hOutputBarIcon, FALSE);
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
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
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
	CString strCustomize;
	strCustomize = CLanguageInterface::Instance()->GetStr(10029);
	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{	

			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);	
		}
	}
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
	{//响应类消息
		switch(lParam)
		{
		case QUOTELINKDOWN://断线
			{
				m_nReconnectCount++;
				CPublic::Instance()->InitQuote(AfxGetMainWnd()->GetSafeHwnd());
				ResetStatusBarLinkStatus(1,FALSE);
			}
			break;
		case QUOTELINKFAIL://连接失败
			{
				if(0 == m_nReconnectCount)
				{
					if(IDYES == MessageBox(CLanguageInterface::Instance()->GetStr(10007),CPublic::Instance()->GetProductName(),MB_ICONWARNING|MB_YESNO))
					{
						ExitMainWnd();
					}
				}
				ResetStatusBarLinkStatus(1,FALSE);
			}
			break;
		case QUOTELINKSUCCESS: //连接成功
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
	//下面是数据类消息
	if( 0 ==lParam)return -1;
	const SNormaHead  * pNoramHead = (SNormaHead*)wParam;
	//此处筛选行情

	CPublic::Instance()->UpdateData((const char*)pNoramHead,(int)lParam);
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
				}
				else
				{
					::SendMessage(CPublic::Instance()->GetLoginWnd(),WM_SETTEXT,QUOTEOTHERSUCCESS,0);//
				}
			}
			if(!m_bReLogin)
			{
				CPublic::Instance()->ReqInitReq();//请求初始化信息
			}
			//刷新行情数据//重新登录的时候刷新界面数据
			CPublic::Instance()->ReqStockInfo();			
			CPublic::Instance()->ReqReportInfo();

			ResetStatusBarUserInfo();
		}
		break;
	case INITMARKET:
		{	
			CInputKey::Instance()->RemoveAll();
			CInputKey::Instance()->ReadCommand();
			const StuSymbolData* pCodeInfo =(StuSymbolData*)(pNoramHead+1);
			CInputKey::Instance()->AddStockCode(pCodeInfo,pNoramHead->m_usSize);

			if(hWnd)
			{
				::SendMessage(CPublic::Instance()->GetLoginWnd(),WM_SETTEXT,QUOTEOTHERSUCCESS,0);				
			}
			//请求界面数据
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
			if((CPublic::Instance()->GetTradeDll()->m_pIsNeedChangePwd())&&(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus()))
			{//是否需要修改密码
				if(IDYES== MessageBox(CLanguageInterface::Instance()->GetStr(10012),CPublic::Instance()->GetProductName(),MB_ICONWARNING|MB_YESNO))
				{
					OnMenuInfo(ID_TRADE_MODIFYPWD);
				}
			}
			m_bReLogin = TRUE;
		}
		break;	
	default:
		break;
	}
	CPublic::Instance()->GetDataSource()->FreeBuffer((char*)wParam);
	return 1;
}

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

		// 获取屏幕的分辨率 
		int nFullWidth  = GetSystemMetrics(SM_CXSCREEN); 
		int nFullHeight = GetSystemMetrics(SM_CYSCREEN); 

		// 将除控制条外的客户区全屏显示到从(0,0)到(nFullWidth, nFullHeight)区域,
		//将(0,0)和(nFullWidth, nFullHeight)两个点外扩充原窗口和除控制条之外的
		//客户区位置间的差值, 就得到全屏显示的窗口位置 

		m_FullScreenRect.left	=	WindowRect.left - ClientRect.left; 
		m_FullScreenRect.top	=	WindowRect.top - ClientRect.top; 
		m_FullScreenRect.right	=	WindowRect.right - ClientRect.right + nFullWidth; 
		m_FullScreenRect.bottom	=	WindowRect.bottom - ClientRect.bottom + nFullHeight;

		// 进入全屏显示状态 
		WINDOWPLACEMENT wndpl; 

		wndpl.length	= sizeof(WINDOWPLACEMENT); 
		wndpl.flags		= 0; 
		wndpl.showCmd	= SW_SHOWNORMAL; 

		wndpl.rcNormalPosition = m_FullScreenRect; 

		SetWindowPlacement(&wndpl); 
	}
	// 退出全屏显示, 恢复原窗口显示 
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
	ExitMainWnd();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_HOTKEY && pMsg->wParam == 0xa001)
	{
		if(m_bShowState)
			HideMainFrm();//隐藏窗口  
		else
			ShowMainFrm();//显示窗口
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnMenuInfo(UINT nID)
{
	switch(nID)
	{
	case ID_TOOL_FULLSCREEN:   //全屏显示
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
	case ID_TOOL_DEFAULT:
		{
			CString strRegFile;
			strRegFile = CPublic::Instance()->GetSysPath();
			strRegFile+="default.reg";
			CString strParameters;
			strParameters = "/s \"" + strRegFile + "\" /adminoption 1";			
			if(IDYES == AfxMessageBox(CLanguageInterface::Instance()->GetStr(10040),MB_YESNO|MB_ICONQUESTION))
			{
				ExitMainWnd();
				ShellExecute(NULL,"runas","regedit.exe",strParameters,NULL,SW_HIDE);
			}
		}
		break;
	case ID_SYSTEM_CONNECTTOHOST: //连接主站
		{	
			((CKOAClientApp*)AfxGetApp())->AskLoginDlg();
//  		CLoginDlg dlg(this);
// 			dlg.DoModal();
			SetWindowText(CPublic::Instance()->GetProductName());
		}
		break;
	case ID_SYSTEM_DISCONNECTHOST:// 断开主站
		{
			CPublic::Instance()->UnLinkQuote();	
			CPublic::Instance()->UnLinkTrade();
			ResetStatusBarLinkStatus(1,FALSE);
			ResetStatusBarLinkStatus(3,FALSE);
		}
		break;
	case ID_TOOL_SYSTEMSETTING:
		{
			CSysProtyDlg setdlg;
			setdlg.DoModal();
		}
		break;
	case ID_SYSTEM_SAVEASPIC:
		{
			Save2Pic();
		}
		break;
	case ID_INTELLIG_FUNCTIONMANAGE:
		{
			SDefineReturn define;
			CFormulaFace::OnOpenDefineTechDlg(define);
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
	case ID_SYSTEM_PRICEORDER:
	case ID_SYSTEM_NEWORDER:
		{
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
			CPublic::Instance()->GetTradeDll()->m_pRefresh();//刷新交易数据
			//刷新行情数据
			CPublic::Instance()->ReqStockInfo();			
			CPublic::Instance()->ReqReportInfo();
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
				MessageBox(CLanguageInterface::Instance()->GetStr(10006),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
			}

		}
		break;
	case ID_SYS_APPENDMENU://银企转账
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
				CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));
				if(pView)pView->OnUserCmd(nID);
			}
			else if((nID >ID_TRADE_BEGIN)&&(nID<ID_TRADE_END))
			{
				CTradeWnd* pTradeWnd = (CTradeWnd*)(CPublic::Instance()->GetView(VIEWINFO_TRADE));
				pTradeWnd->OnUserCmd(nID);
			}
		}
		break;
	}
}
void CMainFrame::OnUpdateMenuInfo(CCmdUI* pCmdUI)
{
	switch( pCmdUI->m_nID) 
	{
	case ID_SYSTEM_DISCONNECTHOST: //断开联结
		{
			pCmdUI->Enable(CPublic::Instance()->GetDataSource()->IsOnline());
		}
		break;   
	case ID_SYSTEM_CONNECTTOHOST:
		{
			pCmdUI->Enable(!CPublic::Instance()->GetDataSource()->IsOnline());
		}
		break;
	case ID_SYSTEM_PRICEORDER:
	case ID_SYSTEM_NEWORDER:
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
				if(CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus())
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
			int nCurSel;
			nCurSel =CPublic::Instance()->GetTradeDll()->m_pGetCurSelTab();
			if(nCurSel == 0&& pCmdUI->m_nID ==ID_TRADE_HOLD)
			{
				pCmdUI->Enable(FALSE);
				break;
			}
			if(nCurSel == 4&& pCmdUI->m_nID ==ID_TRADE_PRODUCT)
			{
				pCmdUI->Enable(FALSE);
				break;
			}
			pCmdUI->Enable(TRUE);
		}
		break;
	default:
		{		
			if((pCmdUI->m_nID >=ID_DRAWLINE_TAG)&&(pCmdUI->m_nID <=ID_DRAWLINE_DELETE))
			{
				CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));
				if(NULL == pView)return;
				int nMode = pView->GetAnalisysMode();
				if(nMode == TechObj)
				{
					pCmdUI->Enable(TRUE);
				}
				else
				{
					pCmdUI->Enable(FALSE);
				}
			}
			else if((pCmdUI->m_nID >ID_ANALYSIST_BEGIN)&&(pCmdUI->m_nID <ID_ANALYSIST_END))
			{
				CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));
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
	{//交易端连接信息
		if(MAINFRAME_LINKSTATUS_LINKED == lParam)
		{//连接成功
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELINKSUCCESS,0);
			ResetStatusBarLinkStatus(3,TRUE);

		}
		else if( MAINFRAME_LINKSTATUS_UNLINK == lParam)
		{//连接失败
			//if(m_nReconnectCount !=0)return 0;
			if(m_nReconnectCount < m_nTradeReconnectCount)return 0;
			//if(hWnd)
			//{
			//	::SendMessage(hWnd,WM_SETTEXT,TRADELINKFAILED,(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20041));
			//}
			//else
			//{
			//}
			ResetStatusBarLinkStatus(3,FALSE);			
			CPublic::Instance()->GetTradeDll()->m_pUnLink();
			CPublic::Instance()->GetDataSource()->UnLink();
			if(!((CKOAClientApp*)AfxGetApp())->AskLoginDlg())
			{
				ExitMainWnd();
				return 0;
			}
			ResetStatusBarLinkStatus(3,TRUE);		
		}
		else if(MAINFRAME_LINKSTATUS_RELINK == lParam)
		{
			ResetStatusBarLinkStatus(3,FALSE);
		}
	}
	else if(MAINFRAME_PWDVALIDTEMSG == wParam)
	{
		if(MAINFRAME_PWDVALIDSTATUS_OK == lParam)
		{//密码验证成功
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINSUCCESS,0);
			CPublic::Instance()->InitQuote(AfxGetMainWnd()->GetSafeHwnd());//GetSafeHwnd());//继续查找行情
		}
		else
		{//密码验证失败
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINFAILED,lParam);//(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20042));
		}
	}
	else if(MAINFRAME_RELOGINMSG ==wParam)
	{//被踢出
		OnMenuInfo(ID_SYSTEM_DISCONNECTHOST);
		OnMenuInfo(ID_SYSTEM_CONNECTTOHOST);
	}
	else if(MAINFRAME_EXIT == wParam)
	{
		ExitMainWnd();
	}
	return 0;
}
void CMainFrame::Save2Pic()
{
	CClientDC SHDC(this);//取得客户区内存DC
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
//注册热键
void CMainFrame::RegHotKey()
{
	::RegisterHotKey(this->GetSafeHwnd() ,0xa001,MOD_ALT,UINT('K'));
}
//注销热键
void CMainFrame::UnRegHotKey()
{
	UnregisterHotKey(this->GetSafeHwnd(), 0xa001);
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

			popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
			
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

LRESULT CMainFrame::OnKeyGhostMessage(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case KEYGHOST_STOCK:
		{ 
			//传递当前代码
			SCodeInfo* pCodeInfo = (SCodeInfo*)lParam;	
			if(pCodeInfo == NULL)
				return 0;
			CPublic::Instance()->ChangeStockEvent(pCodeInfo,RealObj);
		}
		break;
	case KEYGHOST_COMMAND:
		{//键盘精灵命令
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
		//响应键盘精灵
		CInput inputDlg(this);
		inputDlg.m_PosWnd = this;
		inputDlg.cKey = nChar;
		inputDlg.DoModal();
	}
}
void CMainFrame::ExitMainWnd()
{
	CInputKey::ExInstance();
	CInterfaceManager::ExitInstance();

	CPublic::Instance()->UnLinkTrade();
	CPublic::Instance()->GetTradeDll()->m_pDestoryDlg();
	CPublic::Instance()->GetDataSource()->UnLink();

	if(m_bFullScreenMode)
		FullScreenMode(FALSE);//

	UnRegHotKey();

	CFrameWndEx::OnClose();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(m_uiCheckConnectTimer == nIDEvent)
	{
		DWORD   flags;//上网方式   
		if(InternetGetConnectedState(&flags,0))
		{
			if(!m_bInternetStatus)
			{
				//CPublic::Instance()->InitTrade(m_hWnd);
//				//CPublic::Instance()->InitQuote(m_hWnd);
				m_bInternetStatus = TRUE;
			}
		}
		else
		{
			if(m_bInternetStatus)
			{
				ResetStatusBarLinkStatus(1,FALSE);
				ResetStatusBarLinkStatus(3,FALSE);
				//CPublic::Instance()->GetDataSource()->UnLink();
				//CPublic::Instance()->GetTradeDll()->m_pUnLink();
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
	strUserStatus = bUsetStatus?CLanguageInterface::Instance()->GetStr(10033):CLanguageInterface::Instance()->GetStr(10034);
	strUserStatus =CLanguageInterface::Instance()->GetStr(10035)+strUserStatus;
	m_wndStatusBar.SetPaneText(6,strUserStatus);


	char szUserName[100];
	memset(szUserName,0,sizeof(szUserName));
	CPublic::Instance()->GetTradeDll()->m_pGetUserName(szUserName);
	CString strUserName;
	strUserName = CLanguageInterface::Instance()->GetStr(10036);
	strUserName += szUserName;
	m_wndStatusBar.SetPaneText(7,strUserName);
	m_wndStatusBar.SetPaneTextColor(7,RGB(0,0,255));
}

void CMainFrame::ResetStatusBarLinkStatus(int nIndex,BOOL bLinked)
{
	m_wndStatusBar.SetPaneIcon(nIndex+1,bLinked?m_bitmapLinked:m_bitmapUnlink);
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
	//	strServer = "行情服务器："+CPublic::Instance()->GetQuoteServer();
	m_wndStatusBar.SetPaneText(1,CLanguageInterface::Instance()->GetStr(10022));	
	//	strServer = "交易服务器："+CPublic::Instance()->GetTradeServer();
	m_wndStatusBar.SetPaneText(3,CLanguageInterface::Instance()->GetStr(10023));	

	m_bitmapUnlink.LoadBitmap(IDB_BITMAP_UNLINK);
	m_bitmapLinked.LoadBitmap(IDB_BITMAP_LINKED);

	ResetStatusBarLinkStatus(1,TRUE);
	ResetStatusBarLinkStatus(3,TRUE);
}

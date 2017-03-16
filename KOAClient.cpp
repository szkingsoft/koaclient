
// KOAClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "KOAClient.h"
#include "MainFrm.h"

#include "KOAClientDoc.h"
#include "StockView.h"
#include "DefineTradView.h"
#include "LoginDlg.h"
#include "DlgLogin1.h"
#include "markup.h"
#include "DlgUpdateSelect.h"
#include "../trade/trademsgdef.h"
#include <afxwin.h>
#include <afxinet.h>

#include <list>
#include <algorithm>
#include <functional>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTradeMessage(WPARAM wParam,LPARAM lParam);
	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_stcIcon;
};


CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDR_MAINFRAME, m_stcIcon);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_TRADELOGININFO,OnTradeMessage)//响应交易端信息
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_ABOUT_PRODUCTNAME)->SetWindowText(CPublic::Instance()->GetProductName());
	CString strUser,strPwd;
	BOOL bSave,bSaveAccount;
	BOOL bNeedCFCA = FALSE;
	BOOL bSupportCFCA=FALSE;
	CPublic::Instance()->GetLoginInfo(strUser,strPwd,bSave,bSaveAccount,bNeedCFCA,bSupportCFCA);

	GetDlgItem(IDC_ABOUT_REGISTER)->SetWindowText(strUser);
	
	CString strIconPath;
	strIconPath = CPublic::Instance()->GetSysPath()+_T("/interface/koa.ico");
	HICON   hIcon=(HICON)::LoadImage(AfxGetInstanceHandle(),strIconPath,IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_stcIcon.SetIcon(hIcon);
	///////////////
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon
	//////////////////
	CLanguageInterface::Instance()->InitDialog(IDD,this);//初始化对话框字体
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CAboutDlg::OnDataMessage(WPARAM wParam,LPARAM lParam)
{
	HWND hWnd = CPublic::Instance()->GetLoginWnd();
	if(NULL  == wParam)
	{//响应类消息
		switch(lParam)
		{
		case QUOTELINKSUCCESS://连接成功
			{
				CPublic::Instance()->GetDataSource()->ChangeReceieveWnd((unsigned int)theApp.m_pMainWnd->GetSafeHwnd(),WM_DATAARRIVE);
				CPublic::Instance()->GetTradeDll()->m_pChangeRecvWnd(theApp.m_pMainWnd->GetSafeHwnd(),WM_TRADELOGININFO);
				CPublic::Instance()->ReqLoginReq();
				if(hWnd)::SendMessage(hWnd,WM_SETTEXT,QUOTELINKSUCCESS,0);				
				//if(theApp.m_pMainWnd) 
				//	theApp.m_pMainWnd->SendMessage(WM_SETTEXT,QUOTELINKSUCCESS,0);	
			}
			break;
		case QUOTELINKFAIL://连接失败
			{
				if(hWnd)::SendMessage(hWnd,WM_SETTEXT,QUOTELINKFAIL,0);
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

LRESULT CAboutDlg::OnTradeMessage(WPARAM wParam,LPARAM lParam)
{
	HWND hWnd = CPublic::Instance()->GetLoginWnd();
	if(MAINFRAME_LINKMSG == wParam)
	{//交易端连接信息
		if(MAINFRAME_LINKSTATUS_LINKED == lParam)
		{//连接成功
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELINKSUCCESS,0);
		}
		else if(MAINFRAME_LINKSTATUS_UNLINK == lParam)
		{//连接失败
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELINKFAILED,(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20041));
		}
	}
	else if(MAINFRAME_PWDVALIDTEMSG == wParam)
	{
		if(MAINFRAME_PWDVALIDSTATUS_OK == lParam)
		{//密码验证成功
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINSUCCESS,0);
			CPublic::Instance()->InitQuote(GetSafeHwnd());//继续查找行情

			CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));

			if (NULL != pView)
				pView->UpdateToolBarStatus();
		}
		else
		{//密码验证失败
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINFAILED,lParam);//(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20042));
		}
	}
	return 0;
}
// CKOAClientApp

BEGIN_MESSAGE_MAP(CKOAClientApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CKOAClientApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CKOAClientApp 构造

CKOAClientApp::CKOAClientApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	SetAppID(_T("QTSystem.Client.1.0"));

}

// 唯一的一个 CKOAClientApp 对象

CKOAClientApp theApp;


// CKOAClientApp 初始化
BOOL CKOAClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_pGdiToken,&gdiplusStartupInput,NULL);

	CWinAppEx::InitInstance();
//	HRESULT hr = S_OK;
//	hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
//	if (hr != S_OK)
//	{
//		AfxMessageBox("初始化组件服务失败");
//		return FALSE;
//	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// afxAmbientActCtx = FALSE;
	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
    AfxInitRichEdit2();

	SetRegistryKey(_T("FS10334j16"));
	InitContextMenuManager();

	////////////////////
	::CoInitialize(NULL); 

	//CDateManage::Instance()->GetFontFamily();
	CDateManage::Instance()->InitSkinAttribute();

	CInterfaceManager::Instance()->Init();
	CFormulaFace::InitDefineTech();
	CLanguageInterface::Instance()->LoadLanauage();//载入系统语言
	InitKeyboardManager();

	//////////////////////////////////////////////////////////////////////////
	// add by david gong at 20140105
	if(CheckUpdate())return FALSE;
	//else
	//{
	//	//AfxMessageBox(_T("检测更新失败!"));
	//}
	
	//////////////////////////////////////////////////////////////////////////
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CRuntimeClass * pRunTime;

	switch(CPublic::Instance()->GetMainViewMode())
	{
	case 0:
		{
			pRunTime = RUNTIME_CLASS(CStockView);
		}
		break;
	default:
		{
			pRunTime = RUNTIME_CLASS(CDefineTradView);
		}
		break;
	}

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKOAClientDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		pRunTime);
	if (!pDocTemplate)
	{
		AfxMessageBox(_T("注册应用程序的文档模板失败!"));
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);
	
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生	

// 	if(!CPublic::Instance()->IsAutoLogin())
// 	{		
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);		

	//// Dispatch commands specified on the command line
	//ProcessShellCommand(cmdInfo);
	//m_pMainWnd->ShowWindow(SW_HIDE);
	theApp.InitAfterLinked();//连接成功	
		if(!AskLoginDlg())
		{
			return FALSE;
		}
// 	}
// 	else
// 	{	
// 		// 分析标准 shell 命令、DDE、打开文件操作的命令行
// 		CCommandLineInfo cmdInfo;
// 		ParseCommandLine(cmdInfo);
// 		// 调度在命令行中指定的命令。如果
// 		// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
// 		if (!ProcessShellCommand(cmdInfo))
// 			return FALSE;
// 		// 主窗口已初始化，因此显示它并对其进行更新
// 		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
// 		m_pMainWnd->UpdateWindow();
// 	}

	m_pMainWnd->SetWindowText(CPublic::Instance()->GetProductName());

	PostMessage(m_pMainWnd->m_hWnd,WM_LOGINCHANGEPWD,0,0);

	return TRUE;
}

int CKOAClientApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(m_pGdiToken);
	::CoUninitialize(); 

	CInterfaceManager::ExitInstance();
	CFormulaFace::ExitDefineTech();
	CLanguageInterface::ExitInstance();
	CPublic::ExitInstance();
	CDateManage::ExitInstance();

	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// CKOAClientApp 消息处理程序



// 用于运行对话框的应用程序命令
void CKOAClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CKOAClientApp 自定义加载/保存方法

void CKOAClientApp::PreLoadState()
{
}

void CKOAClientApp::LoadCustomState()
{
}

void CKOAClientApp::SaveCustomState()
{
}

// CKOAClientApp 消息处理程序

//void CKOAClientApp::OnFileNew()
//{
//	((theApp.m_pDocManager)->GetFirstDocTemplatePosition());
//	CDocTemplate* pTemplate = theApp.m_pDocManager->GetBestTemplate(_T("CMainFrame"));//theApp.m_pDocManager->GetNextDocTemplate(((theApp.m_pDocManager)->GetFirstDocTemplatePosition()));
//	ASSERT(pTemplate != NULL);
//	ASSERT_KINDOF(CDocTemplate, pTemplate);
//
//	pTemplate->OpenDocumentFile(NULL,FALSE);
//}
BOOL CKOAClientApp::InitAfterLinked()
{
	if(!m_pMainWnd||!((CMainFrame*)m_pMainWnd)->m_bReLogin)
	{
	// 	CMainFrame* pMainFrame = new CMainFrame;
	// 	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	// 	{
	// 		delete pMainFrame;
	// 		return FALSE;
	// 	}
	// 	m_pMainWnd = pMainFrame;
		// Parse command line for standard shell commands, DDE, file open
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);		
		//cmdInfo.m_bShowSplash = 0;
		// Dispatch commands specified on the command line
		ProcessShellCommand(cmdInfo);

		/////////////
		//CRect rt;
		//SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		//int isysFramex = 60;
		//rt.left +=(isysFramex);
		//rt.top +=(isysFramex);
		//rt.right -=(isysFramex);
		//rt.bottom -=(isysFramex);
		//m_pMainWnd->MoveWindow(rt);
		///////////////////////
		m_pMainWnd->ShowWindow(SW_HIDE);
	}
	//CPublic::Instance()->GetDataSource()->ChangeReceieveWnd((unsigned int)m_pMainWnd->GetSafeHwnd(),WM_DATAARRIVE);
	//CPublic::Instance()->GetTradeDll()->m_pChangeRecvWnd(m_pMainWnd->GetSafeHwnd(),WM_TRADELOGININFO);
	return TRUE;
}

void CKOAClientApp::InitAfterLogin(int nLoginResult)
{
	if ( nLoginResult == 1) 
	{
		//m_pMainWnd->ShowWindow(SW_RESTORE);
		//m_pMainWnd->UpdateWindow();
		CRect rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		rt.left-=8;
		rt.top-=8;
		rt.right+=8;
		rt.bottom+=8;
		m_pMainWnd->MoveWindow(rt);
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);

		//SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		//rt.left-=isysFramex;
		//rt.top-=isysFramex;
		//rt.right+=isysFramex;
		//rt.bottom+=isysFramex;
		//m_pMainWnd->MoveWindow(rt);
		//m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
		//m_pdlgMainWnd = new CDlgMainWnd;
		//m_pdlgMainWnd->Create(CDlgMainWnd::IDD);
		//m_pdlgMainWnd->MoveWindow(rt);
		//m_pdlgMainWnd->ShowWindow(SW_SHOW);
	}
	if(nLoginResult == 2)
	{
		if(m_pMainWnd)m_pMainWnd->PostMessage(WM_CLOSE);
	}
	
}
BOOL CKOAClientApp::AskLoginDlg()
{
	CAboutDlg InitAboutDlg;
	InitAboutDlg.Create(CAboutDlg::IDD);
	InitAboutDlg.ShowWindow(SW_HIDE);

	if(CPublic::Instance()->GetLoginMode() == 0)
	{
		CLoginDlg logindlg(&InitAboutDlg);

		if(IDOK != logindlg.DoModal())
		{
			return FALSE;
		}
	}
	else
	{
		CDlgLogin1 logindlg(&InitAboutDlg);
		if(IDOK != logindlg.DoModal())
		{
			return FALSE;
		}
	}

	return TRUE;
}

// CString CKOAClientApp::GetFileVersion(CString strFileName)
// {   
// 	int   iVerInfoSize;
// 	char   *pBuf;
// 	CString   asVer="";
// 	VS_FIXEDFILEINFO   *pVsInfo;
// 	unsigned   int   iFileInfoSize   =   sizeof(   VS_FIXEDFILEINFO   );
// 
// 
// 	iVerInfoSize   =   GetFileVersionInfoSize(strFileName,NULL); 
// 
// 	if(iVerInfoSize!= 0)
// 	{   
// 		pBuf   =   new   char[iVerInfoSize];
// 		if(GetFileVersionInfo(strFileName,0,   iVerInfoSize,   pBuf   )   )   
// 		{   
// 			if(VerQueryValue(pBuf,   "//",(void   **)&pVsInfo,&iFileInfoSize))   
// 			{   
// 				asVer.Format("%d.%d.%d.%d",HIWORD(pVsInfo->dwFileVersionMS),LOWORD(pVsInfo->dwFileVersionMS),HIWORD(pVsInfo->dwFileVersionLS),LOWORD(pVsInfo->dwFileVersionLS));
// 			}   
// 		}   
// 		delete   pBuf;   
// 	}   
// 	return   asVer;   
// } 
// 
// CString CKOAClientApp::GetModifyTime(CString appname)
// {
// 	WIN32_FIND_DATA ffd ;
// 	HANDLE hFind = FindFirstFile(appname,&ffd);
// 	SYSTEMTIME stUTC, stLocal;
// 	FileTimeToSystemTime(&(ffd.ftLastWriteTime), &stUTC);
// 	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
// 	CString strTmp;
// 	strTmp.Format("%04d%02d%02d", stLocal.wYear,stLocal.wMonth,stLocal.wDay);
// 	return strTmp;
// }

BOOL	CKOAClientApp::CheckUpdate()
{	
	
	//////////////////////////////////////////////////////////////////////////
	map <CString,CString> mapLocal;
	//读取本地升级文件
	CString strLocalUpdate;
	strLocalUpdate = CPublic::Instance()->GetSysPath()+"lastupdate.dat";
	CFile localupdateFile;
	CString strName,strUpdateTime;

	
	if(localupdateFile.Open(strLocalUpdate,CFile::modeRead|CFile::typeBinary))//载入本地升级文件失败
	{
		
		int nDataLen = localupdateFile.GetLength();
#pragma pack(1)
		struct stuUpdate
		{
			char szName[50];
			char szUpdateTime[9];
		};
#pragma pack()
		int nStructNum = nDataLen/sizeof(stuUpdate);
		stuUpdate * pUpdate = new stuUpdate[nStructNum];
		localupdateFile.Read(pUpdate,nStructNum*sizeof(stuUpdate));
		localupdateFile.Close();
		for(int i = 0;i<nStructNum;i++)
		{
			strName = pUpdate[i].szName;
			strName.MakeLower();
			strUpdateTime = pUpdate[i].szUpdateTime;
			mapLocal[strName] =strUpdateTime;//添加到本地列表
		}
		delete []pUpdate;
		
	}
	//////////////////////////////////////////////////////////////////////////
	//升级自动升级程序
	CString strLiveUpdateTmpExe = CPublic::Instance()->GetSysPath() + "update/liveupdate.exe";
	CString strLiveUpdateExe = CPublic::Instance()->GetSysPath() + "liveupdate.exe";

	
	if(-1 != _access(strLiveUpdateTmpExe,0))
	{
		
		BOOL bMove;
		bMove = CopyFile(strLiveUpdateTmpExe,strLiveUpdateExe,FALSE);
		if(bMove)
		{
			DeleteFile(strLiveUpdateTmpExe);
		}
		
	}
	//////////////////////////////////////////////////////////////////////////
	//此处下载xml文件解析
	// add by david gong at 20140105
	CString strServer,strTag;
	WORD	wServerPort;
	DWORD dwOutTm = 8000;
	CPublic::Instance()->GetUpdateServer(strServer,wServerPort,strTag,dwOutTm);
	DWORD dwError;
	CString strResponse;

	
	if(!Get(strResponse,strServer,wServerPort,strTag,dwError,dwOutTm))
	{	
		return FALSE;//连接网络失败
	}
	
	//解析xml和本地文件进行比较
	CMarkup markup;
	markup.SetDoc(strResponse);	

	if(!markup.FindElem("Product"))return FALSE;//读取标志失败
	markup.IntoElem();
	map<CString,CString> mapServerList;
	if(markup.FindElem("ServerList"))
	{//非必需项
		markup.IntoElem();
		CString strTag;
		CString strName,strUrl;
		for (int i =1;i<11;i++)
		{
			strTag.Format("Server%d",i);
			if(!markup.FindElem(strTag))break;//找不到，直接退出
			strName = markup.GetAttrib("Name");
			strUrl = markup.GetAttrib("URL");
			mapServerList[strName]= strUrl;
		}
		markup.OutOfElem();
	}
	
	if(mapServerList.size()<1)return FALSE;//没有找到升级地址

	int nNodeCount =0;
	map<CString,CString>::iterator it;

	while(markup.FindElem("Component"))
	{
		markup.IntoElem();
		if(markup.FindElem("Name"))
		{
			strName = markup.GetData();
			if(markup.FindElem("UpdateTime"))
			{
				strUpdateTime = markup.GetData();
			}
		}
		markup.OutOfElem();	
		strName.MakeLower();
		it = mapLocal.find(strName);
		if((it != mapLocal.end())&&(it->second >= strUpdateTime))
		{//不符合升级标准的，删除信息
			markup.RemoveElem();
			continue;
		}
		nNodeCount++;
	}
	
	if(nNodeCount== 0)return FALSE;
	//////////////////////////////////////////////////////////////////////////
	//此处识别电信还是联通
	CString strDownloadURL;
	if( 1 < mapServerList.size())
	{
		CDlgUpdateSelect dlg;
		dlg.m_pMapInfo = &mapServerList;
		if(IDOK == dlg.DoModal())
		{
			it  = mapServerList.find(dlg.m_strSelectServer);
			if(it != mapServerList.end())
				strDownloadURL = it->second;
		}

	}
	else 
	{
		strDownloadURL = mapServerList.begin()->second;//服务器地址
	}

	
	// 重新整理xml
	CString strDownloadAddr;
	markup.ResetMainPos();
	while(markup.FindElem("Component"))
	{
		markup.IntoElem();
		if(markup.FindElem("DownAddress"))
		{
			strDownloadAddr = markup.GetData();
			strDownloadAddr = strDownloadURL + strDownloadAddr;
			markup.SetData(strDownloadAddr);
		}
		markup.OutOfElem();	
		nNodeCount++;
	}

	
	//////////////////////////////////////////////////////////////////////////
	//生成需要升级的文件
	markup.OutOfElem();
	CString strUpdateNewFile;
	strUpdateNewFile = CPublic::Instance()->GetSysPath()+"update.xml";
	markup.Save(strUpdateNewFile);
	//////////////////////////////////////////////////////////////////////////

	
	TCHAR szMainExeName[MAX_PATH];
	GetModuleFileName(AfxGetApp()->m_hInstance,szMainExeName,sizeof(szMainExeName));
	strLiveUpdateExe += " ";
	strLiveUpdateExe += '"';
	strLiveUpdateExe += szMainExeName;
	strLiveUpdateExe += '"';

	CString strTemp;
	strTemp.Format(_T(" %d"),::GetCurrentProcessId());
	strLiveUpdateExe += strTemp;
	strLiveUpdateExe += _T(" davidsha 1");
	STARTUPINFO	 si;
	PROCESS_INFORMATION	pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	
	if(!CreateProcess(NULL, (char*)(LPCTSTR)strLiveUpdateExe, NULL, NULL, FALSE, 
		CREATE_NEW_PROCESS_GROUP|NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10013),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);		
		return FALSE;
	}	

	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return TRUE;
}
//BOOL	CKOAClientApp::CheckUpdate()
//{	
//	//////////////////////////////////////////////////////////////////////////
//	map <CString,CString> mapLocal;
//	//读取本地升级文件
//	CString strLocalUpdate;
//	strLocalUpdate = CPublic::Instance()->GetSysPath()+"lastupdate.dat";
//	CFile localupdateFile;
//	CString strName,strUpdateTime;
//	if(localupdateFile.Open(strLocalUpdate,CFile::modeRead|CFile::typeBinary))//载入本地升级文件失败
//	{
//		int nDataLen = localupdateFile.GetLength();
//#pragma pack(1)
//		struct stuUpdate
//		{
//			char szName[50];
//			char szUpdateTime[9];
//		};
//#pragma pack()
//		int nStructNum = nDataLen/sizeof(stuUpdate);
//		stuUpdate * pUpdate = new stuUpdate[nStructNum];
//		localupdateFile.Read(pUpdate,nStructNum*sizeof(stuUpdate));
//		localupdateFile.Close();
//		for(int i = 0;i<nStructNum;i++)
//		{
//			strName = pUpdate[i].szName;
//			strName.MakeLower();
//			strUpdateTime = pUpdate[i].szUpdateTime;
//			mapLocal[strName] =strUpdateTime;//添加到本地列表
//		}
//		delete []pUpdate;
//	}
//	//////////////////////////////////////////////////////////////////////////
//	//升级自动升级程序
//	CString strLiveUpdateTmpExe = CPublic::Instance()->GetSysPath() + "update/liveupdate.exe";
//	CString strLiveUpdateExe = CPublic::Instance()->GetSysPath() + "liveupdate.exe";
//	if(-1 != _access(strLiveUpdateTmpExe,0))
//	{
//		BOOL bMove;
//		bMove = CopyFile(strLiveUpdateTmpExe,strLiveUpdateExe,FALSE);
//		if(bMove)
//		{
//			DeleteFile(strLiveUpdateTmpExe);
//		}
//	}
//	//////////////////////////////////////////////////////////////////////////
//	//此处下载xml文件解析
//	// add by david gong at 20140105
//	CString strServer,strTag;
//	WORD	wServerPort;
//	CPublic::Instance()->GetUpdateServer(strServer,wServerPort,strTag);
//	DWORD dwError;
//	CString strResponse;
//	if(!Get(strResponse,strServer,wServerPort,strTag,dwError))
//	{	
//		return FALSE;//连接网络失败
//	}
//	//解析xml和本地文件进行比较
//	CMarkup markup;
//	markup.SetDoc(strResponse);	
//
//	if(!markup.FindElem("Product"))return FALSE;//读取标志失败
//	markup.IntoElem();
//	map<CString,CString> mapServerList;
//	if(markup.FindElem("ServerList"))
//	{//非必需项
//		markup.IntoElem();
//		CString strTag;
//		CString strName,strUrl;
//		for (int i =1;i<11;i++)
//		{
//			strTag.Format("Server%d",i);
//			if(!markup.FindElem(strTag))break;//找不到，直接退出
//			strName = markup.GetAttrib("Name");
//			strUrl = markup.GetAttrib("URL");
//			mapServerList[strName]= strUrl;
//		}
//		markup.OutOfElem();
//	}
//	if(mapServerList.size()<1)return FALSE;//没有找到升级地址
//
//	int nNodeCount =0;
//	map<CString,CString>::iterator it;
//
//	while(markup.FindElem("Component"))
//	{
//		markup.IntoElem();
//		if(markup.FindElem("Name"))
//		{
//			strName = markup.GetData();
//			if(markup.FindElem("UpdateTime"))
//			{
//				strUpdateTime = markup.GetData();
//			}
//		}
//		markup.OutOfElem();	
//		strName.MakeLower();
//		it = mapLocal.find(strName);
//		if((it != mapLocal.end())&&(it->second >= strUpdateTime))
//		{//不符合升级标准的，删除信息
//			markup.RemoveElem();
//			continue;
//		}
//		nNodeCount++;
//	}
//
//	if(nNodeCount== 0)return FALSE;
//	//////////////////////////////////////////////////////////////////////////
//	//此处识别电信还是联通
//	CString strDownloadURL;
//	if( 1 < mapServerList.size())
//	{
//		CDlgUpdateSelect dlg;
//		dlg.m_pMapInfo = &mapServerList;
//		if(IDOK == dlg.DoModal())
//		{
//			it  = mapServerList.find(dlg.m_strSelectServer);
//			if(it != mapServerList.end())
//				strDownloadURL = it->second;
//		}
//		
//	}
//	else 
//	{
//		strDownloadURL = mapServerList.begin()->second;//服务器地址
//	}
//	// 重新整理xml
//	CString strDownloadAddr;
//	markup.ResetMainPos();
//	while(markup.FindElem("Component"))
//	{
//		markup.IntoElem();
//		if(markup.FindElem("DownAddress"))
//		{
//			strDownloadAddr = markup.GetData();
//			strDownloadAddr = strDownloadURL + strDownloadAddr;
//			markup.SetData(strDownloadAddr);
//		}
//		markup.OutOfElem();	
//		nNodeCount++;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	//生成需要升级的文件
//	markup.OutOfElem();
//	CString strUpdateNewFile;
//	strUpdateNewFile = CPublic::Instance()->GetSysPath()+"update.xml";
//	markup.Save(strUpdateNewFile);
//	//////////////////////////////////////////////////////////////////////////
//
//	TCHAR szMainExeName[MAX_PATH];
//	GetModuleFileName(AfxGetApp()->m_hInstance,szMainExeName,sizeof(szMainExeName));
//	strLiveUpdateExe += " ";
//	strLiveUpdateExe += '"';
//	strLiveUpdateExe += szMainExeName;
//	strLiveUpdateExe += '"';
//
//	CString strTemp;
//	strTemp.Format(_T(" %d"),::GetCurrentProcessId());
//	strLiveUpdateExe += strTemp;
//	strLiveUpdateExe += _T(" davidsha 1");
//	STARTUPINFO	 si;
//	PROCESS_INFORMATION	pi;
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(si);
//	ZeroMemory(&pi, sizeof(pi));
//
//	if(!CreateProcess(NULL, (char*)(LPCTSTR)strLiveUpdateExe, NULL, NULL, FALSE, 
//		CREATE_NEW_PROCESS_GROUP|NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
//	{
//		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10013),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);		
//		return FALSE;
//	}	
//
//	CloseHandle(pi.hThread);
//	CloseHandle(pi.hProcess);
//	AfxMessageBox(_T("程序更新成功,请重新运行程序!"));
//	return TRUE;
//}

BOOL CKOAClientApp::Get(CString &csResponse,//返回的内容
	const char *szServer,//服务器
	INTERNET_PORT& nPort,//端口
	const char* szObject,//URI
	DWORD& dwHttpStatus,
	DWORD dwOutTm)//结果
{
	if((NULL == szServer)||(NULL == szObject))return  FALSE;
	dwHttpStatus = 0;

	char filebuf[1024]; // Buffer to store incoming file bytes
	CInternetSession netSession; // Our internetsession object
	CHttpConnection *pHttp = NULL;
	try
	{
		netSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,dwOutTm);
		netSession.SetOption (INTERNET_OPTION_CONNECT_RETRIES, 1) ;
		pHttp=netSession.GetHttpConnection(szServer,0,nPort,NULL,NULL);
		if(pHttp == NULL)return FALSE;

		CHttpFile *pFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_GET,szObject); 
		int nError=GetLastError();
		if(!pFile->SendRequest())
		{
			dwHttpStatus = 504;
			pFile->Close();
			return FALSE;
		}

		DWORD fileLength = pFile->SeekToEnd();
		char *szBuffer = new char[fileLength+1];
		memset(szBuffer,0,fileLength+1);
		pFile->SeekToBegin();
		memset(filebuf,0,sizeof(filebuf));
		int nCount = 0;
		int nGetLen;
		while ((nGetLen = pFile->Read(filebuf, 1024)) >0) // Read from file
		{
			memcpy(szBuffer+nCount*1024,filebuf,nGetLen);
			memset(filebuf,0,sizeof(filebuf));
			nCount++;
		}
		pFile->Close();
		csResponse = szBuffer;
		delete []szBuffer;
		return TRUE;
	}
	catch(CInternetException *IE)
	{
		IE->Delete(); // Delete exception object to prevent leaks
		dwHttpStatus = 504;
	}
	catch(CFileException *e)
	{
		e->Delete();
		dwHttpStatus = 404;
	}
	return FALSE;
}
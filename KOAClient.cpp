
// KOAClient.cpp : ����Ӧ�ó��������Ϊ��
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

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTradeMessage(WPARAM wParam,LPARAM lParam);
	// ʵ��
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
	ON_MESSAGE(WM_TRADELOGININFO,OnTradeMessage)//��Ӧ���׶���Ϣ
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
	CLanguageInterface::Instance()->InitDialog(IDD,this);//��ʼ���Ի�������
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

LRESULT CAboutDlg::OnDataMessage(WPARAM wParam,LPARAM lParam)
{
	HWND hWnd = CPublic::Instance()->GetLoginWnd();
	if(NULL  == wParam)
	{//��Ӧ����Ϣ
		switch(lParam)
		{
		case QUOTELINKSUCCESS://���ӳɹ�
			{
				CPublic::Instance()->GetDataSource()->ChangeReceieveWnd((unsigned int)theApp.m_pMainWnd->GetSafeHwnd(),WM_DATAARRIVE);
				CPublic::Instance()->GetTradeDll()->m_pChangeRecvWnd(theApp.m_pMainWnd->GetSafeHwnd(),WM_TRADELOGININFO);
				CPublic::Instance()->ReqLoginReq();
				if(hWnd)::SendMessage(hWnd,WM_SETTEXT,QUOTELINKSUCCESS,0);				
				//if(theApp.m_pMainWnd) 
				//	theApp.m_pMainWnd->SendMessage(WM_SETTEXT,QUOTELINKSUCCESS,0);	
			}
			break;
		case QUOTELINKFAIL://����ʧ��
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
	{//���׶�������Ϣ
		if(MAINFRAME_LINKSTATUS_LINKED == lParam)
		{//���ӳɹ�
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELINKSUCCESS,0);
		}
		else if(MAINFRAME_LINKSTATUS_UNLINK == lParam)
		{//����ʧ��
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELINKFAILED,(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20041));
		}
	}
	else if(MAINFRAME_PWDVALIDTEMSG == wParam)
	{
		if(MAINFRAME_PWDVALIDSTATUS_OK == lParam)
		{//������֤�ɹ�
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINSUCCESS,0);
			CPublic::Instance()->InitQuote(GetSafeHwnd());//������������

			CStockView* pView = (CStockView*)(CPublic::Instance()->GetView(VIEWINFO_STOCK));

			if (NULL != pView)
				pView->UpdateToolBarStatus();
		}
		else
		{//������֤ʧ��
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINFAILED,lParam);//(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20042));
		}
	}
	return 0;
}
// CKOAClientApp

BEGIN_MESSAGE_MAP(CKOAClientApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CKOAClientApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CKOAClientApp ����

CKOAClientApp::CKOAClientApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	SetAppID(_T("QTSystem.Client.1.0"));

}

// Ψһ��һ�� CKOAClientApp ����

CKOAClientApp theApp;


// CKOAClientApp ��ʼ��
BOOL CKOAClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_pGdiToken,&gdiplusStartupInput,NULL);

	CWinAppEx::InitInstance();
//	HRESULT hr = S_OK;
//	hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
//	if (hr != S_OK)
//	{
//		AfxMessageBox("��ʼ���������ʧ��");
//		return FALSE;
//	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// afxAmbientActCtx = FALSE;
	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
    AfxInitRichEdit2();

	SetRegistryKey(_T("FS10334j16"));
	InitContextMenuManager();

	////////////////////
	::CoInitialize(NULL); 

	//CDateManage::Instance()->GetFontFamily();
	CDateManage::Instance()->InitSkinAttribute();

	CInterfaceManager::Instance()->Init();
	CFormulaFace::InitDefineTech();
	CLanguageInterface::Instance()->LoadLanauage();//����ϵͳ����
	InitKeyboardManager();

	//////////////////////////////////////////////////////////////////////////
	// add by david gong at 20140105
	if(CheckUpdate())return FALSE;
	//else
	//{
	//	//AfxMessageBox(_T("������ʧ��!"));
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

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKOAClientDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		pRunTime);
	if (!pDocTemplate)
	{
		AfxMessageBox(_T("ע��Ӧ�ó�����ĵ�ģ��ʧ��!"));
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);
	
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������	

// 	if(!CPublic::Instance()->IsAutoLogin())
// 	{		
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);		

	//// Dispatch commands specified on the command line
	//ProcessShellCommand(cmdInfo);
	//m_pMainWnd->ShowWindow(SW_HIDE);
	theApp.InitAfterLinked();//���ӳɹ�	
		if(!AskLoginDlg())
		{
			return FALSE;
		}
// 	}
// 	else
// 	{	
// 		// ������׼ shell ���DDE�����ļ�������������
// 		CCommandLineInfo cmdInfo;
// 		ParseCommandLine(cmdInfo);
// 		// ��������������ָ����������
// 		// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
// 		if (!ProcessShellCommand(cmdInfo))
// 			return FALSE;
// 		// �������ѳ�ʼ���������ʾ����������и���
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

// CKOAClientApp ��Ϣ�������



// �������жԻ����Ӧ�ó�������
void CKOAClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CKOAClientApp �Զ������/���淽��

void CKOAClientApp::PreLoadState()
{
}

void CKOAClientApp::LoadCustomState()
{
}

void CKOAClientApp::SaveCustomState()
{
}

// CKOAClientApp ��Ϣ�������

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
	//��ȡ���������ļ�
	CString strLocalUpdate;
	strLocalUpdate = CPublic::Instance()->GetSysPath()+"lastupdate.dat";
	CFile localupdateFile;
	CString strName,strUpdateTime;

	
	if(localupdateFile.Open(strLocalUpdate,CFile::modeRead|CFile::typeBinary))//���뱾�������ļ�ʧ��
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
			mapLocal[strName] =strUpdateTime;//��ӵ������б�
		}
		delete []pUpdate;
		
	}
	//////////////////////////////////////////////////////////////////////////
	//�����Զ���������
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
	//�˴�����xml�ļ�����
	// add by david gong at 20140105
	CString strServer,strTag;
	WORD	wServerPort;
	DWORD dwOutTm = 8000;
	CPublic::Instance()->GetUpdateServer(strServer,wServerPort,strTag,dwOutTm);
	DWORD dwError;
	CString strResponse;

	
	if(!Get(strResponse,strServer,wServerPort,strTag,dwError,dwOutTm))
	{	
		return FALSE;//��������ʧ��
	}
	
	//����xml�ͱ����ļ����бȽ�
	CMarkup markup;
	markup.SetDoc(strResponse);	

	if(!markup.FindElem("Product"))return FALSE;//��ȡ��־ʧ��
	markup.IntoElem();
	map<CString,CString> mapServerList;
	if(markup.FindElem("ServerList"))
	{//�Ǳ�����
		markup.IntoElem();
		CString strTag;
		CString strName,strUrl;
		for (int i =1;i<11;i++)
		{
			strTag.Format("Server%d",i);
			if(!markup.FindElem(strTag))break;//�Ҳ�����ֱ���˳�
			strName = markup.GetAttrib("Name");
			strUrl = markup.GetAttrib("URL");
			mapServerList[strName]= strUrl;
		}
		markup.OutOfElem();
	}
	
	if(mapServerList.size()<1)return FALSE;//û���ҵ�������ַ

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
		{//������������׼�ģ�ɾ����Ϣ
			markup.RemoveElem();
			continue;
		}
		nNodeCount++;
	}
	
	if(nNodeCount== 0)return FALSE;
	//////////////////////////////////////////////////////////////////////////
	//�˴�ʶ����Ż�����ͨ
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
		strDownloadURL = mapServerList.begin()->second;//��������ַ
	}

	
	// ��������xml
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
	//������Ҫ�������ļ�
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
//	//��ȡ���������ļ�
//	CString strLocalUpdate;
//	strLocalUpdate = CPublic::Instance()->GetSysPath()+"lastupdate.dat";
//	CFile localupdateFile;
//	CString strName,strUpdateTime;
//	if(localupdateFile.Open(strLocalUpdate,CFile::modeRead|CFile::typeBinary))//���뱾�������ļ�ʧ��
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
//			mapLocal[strName] =strUpdateTime;//��ӵ������б�
//		}
//		delete []pUpdate;
//	}
//	//////////////////////////////////////////////////////////////////////////
//	//�����Զ���������
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
//	//�˴�����xml�ļ�����
//	// add by david gong at 20140105
//	CString strServer,strTag;
//	WORD	wServerPort;
//	CPublic::Instance()->GetUpdateServer(strServer,wServerPort,strTag);
//	DWORD dwError;
//	CString strResponse;
//	if(!Get(strResponse,strServer,wServerPort,strTag,dwError))
//	{	
//		return FALSE;//��������ʧ��
//	}
//	//����xml�ͱ����ļ����бȽ�
//	CMarkup markup;
//	markup.SetDoc(strResponse);	
//
//	if(!markup.FindElem("Product"))return FALSE;//��ȡ��־ʧ��
//	markup.IntoElem();
//	map<CString,CString> mapServerList;
//	if(markup.FindElem("ServerList"))
//	{//�Ǳ�����
//		markup.IntoElem();
//		CString strTag;
//		CString strName,strUrl;
//		for (int i =1;i<11;i++)
//		{
//			strTag.Format("Server%d",i);
//			if(!markup.FindElem(strTag))break;//�Ҳ�����ֱ���˳�
//			strName = markup.GetAttrib("Name");
//			strUrl = markup.GetAttrib("URL");
//			mapServerList[strName]= strUrl;
//		}
//		markup.OutOfElem();
//	}
//	if(mapServerList.size()<1)return FALSE;//û���ҵ�������ַ
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
//		{//������������׼�ģ�ɾ����Ϣ
//			markup.RemoveElem();
//			continue;
//		}
//		nNodeCount++;
//	}
//
//	if(nNodeCount== 0)return FALSE;
//	//////////////////////////////////////////////////////////////////////////
//	//�˴�ʶ����Ż�����ͨ
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
//		strDownloadURL = mapServerList.begin()->second;//��������ַ
//	}
//	// ��������xml
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
//	//������Ҫ�������ļ�
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
//	AfxMessageBox(_T("������³ɹ�,���������г���!"));
//	return TRUE;
//}

BOOL CKOAClientApp::Get(CString &csResponse,//���ص�����
	const char *szServer,//������
	INTERNET_PORT& nPort,//�˿�
	const char* szObject,//URI
	DWORD& dwHttpStatus,
	DWORD dwOutTm)//���
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
// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "ftsha1api.h"
#include "DlgWarnInfo.h"
#include "../trade/trademsgdef.h"

#include "mytoolkit.h"   //add by witming


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define CONNECTOUTTIME 60*1000
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: WDialog(CLoginDlg::IDD,_T("CLoginDlg"), pParent)
	, m_strValidateCode(_T(""))
	, m_nServerSel(-1)
	, m_bCheckWarnInfo(TRUE)
	, m_bSaveAccount(FALSE)
	, m_nNeedCFCA(0)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_strUserName = _T("");
	m_strPassword = _T("");
	m_bSavePassword = FALSE;
	m_bPwdChanged = FALSE;
	m_bLoginStatus = FALSE;
	m_nCurPos = 0;
	//}}AFX_DATA_INIT	
}
CLoginDlg::~CLoginDlg()
{

}

//##ModelId=40A890130196
void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	WDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	//	DDX_Control(pDX, IDC_STATIC_GIFTITLE, m_ctrlGifTitle);
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_PASSWORD_LOGIN, m_strPassword);
	DDX_Control(pDX, IDC_PROGRESS_STEP_SPR, m_progressStep);
	DDX_Control(pDX, IDC_STC_PROMPT_SPR, m_editPrompt);
	DDX_Check(pDX, IDC_CHECK_SAVEPASSWORD, m_bSavePassword);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STC_CHECKCODE, m_stcCheckCode);
	DDX_Text(pDX, IDC_EDIT_CHECKCODE, m_strValidateCode);
	DDV_MaxChars(pDX, m_strValidateCode, 4);

	DDX_CBIndex(pDX, IDC_COMBO_SERVERLIST, m_nServerSel);
	DDX_Control(pDX, IDC_COMBO_SERVERLIST, m_commServerList);
	DDX_Control(pDX, IDC_STATIC_REGIST, m_stcDemoLink);
	DDX_Check(pDX, IDC_CHECK_READWARNINFO, m_bCheckWarnInfo);
	DDX_Control(pDX,IDOK,m_LoginBtn);
	DDX_Control(pDX,IDCANCEL,m_ExitBtn);
	DDX_Check(pDX, IDC_CHECK_SAVEACCOUNT, m_bSaveAccount);
	DDX_Radio(pDX, IDC_RADIO1, m_nNeedCFCA);
	DDX_Control(pDX, IDC_STATIC_SERVER, m_statServer);
	DDX_Control(pDX, IDC_STATIC_ACCOUNT, m_statAccount);
	DDX_Control(pDX, IDC_STATIC_PWD, m_statPwd);
	DDX_Control(pDX, IDC_STATIC_CHECKCODE, m_statCheckCode);
	DDX_Control(pDX, IDC_CHECK_READWARNINFO, m_btnReadWarnInfo);
	DDX_Control(pDX, IDC_RADIO1, m_btnRadio1);
	DDX_Control(pDX, IDC_RADIO2, m_btnRadio2);
	DDX_Control(pDX, IDC_USERNAME, m_edtUserName);
	DDX_Control(pDX, IDC_PASSWORD_LOGIN, m_edtPassWord);
	DDX_Control(pDX, IDC_EDIT_CHECKCODE, m_edtCheckCode);
	DDX_Control(pDX, IDC_CHECK_SAVEACCOUNT, m_btnSaveAccount);
	DDX_Control(pDX, IDC_CHECK_SAVEPASSWORD, m_btnSavePassWord);
	DDX_Control(pDX, IDC_BUTTON_READWARN, m_btnReadWarn);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_statVersion);
	DDX_Control(pDX, IDC_STATIC_VER, m_statVersionName);
}


BEGIN_MESSAGE_MAP(CLoginDlg, WDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_MESSAGE( WM_SETTEXT, OnSetTextMsg )
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnOK)
	ON_EN_CHANGE(IDC_PASSWORD_LOGIN, &CLoginDlg::OnEnChangePasswordLogin)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_READWARN, &CLoginDlg::OnBnClickedButtonReadwarn)
	ON_BN_CLICKED(IDC_CHECK_READWARNINFO, &CLoginDlg::OnBnClickedCheckReadwarninfo)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STC_CHECKCODE, &CLoginDlg::OnStnClickedStcCheckcode)
	ON_STN_DBLCLK(IDC_STC_CHECKCODE, &CLoginDlg::OnStnDblclickStcCheckcode)
	ON_MESSAGE(WM_TRADELOGININFO,OnTradeMessage)//响应交易端信息
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	SetBaseDlgMode(300,0x0000);
	//SetBkImg(_T(""),_T("D:\\client20140526\\tradeclient\\prog\\interface\\loginbmp2.png"),_T(""));
	
	WDialog::OnInitDialog();

	int iHCaption = (CPublic::Instance()->GetUseLgOld())?100:50;

	CString strParamPath = CPublic::Instance()->GetSysPath();
	strParamPath += _T("interface\\Login\\");
	SetBkImg(strParamPath + _T("ImgBk.png"),iHCaption,0);
	SetTitleLogo(strParamPath + _T("Imglogo.png"),55,55);
	//SetWndPos();
	//MoveWindow(0,0,676,397/*600,450*/);
	CenterWindow();

	ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW);
	ModifyStyle(0,WS_SYSMENU|WS_MINIMIZEBOX,0);

	InitCtrl();
	//SetTitleText(_T("123456"),Color::MakeARGB(255,255,0,0),14,1);
	//CString strCancelBtnPath,strLoginBtnPath,strExitBtnPath;
	//strCancelBtnPath = CPublic::Instance()->GetSysPath()+"cancelbtn.bmp";
	//strExitBtnPath = CPublic::Instance()->GetSysPath()+"exitbtn.bmp";
	//strLoginBtnPath = CPublic::Instance()->GetSysPath()+"loginbtn.bmp";
	
	//m_hLoginbitmap = (HBITMAP) ::LoadImage(NULL,strLoginBtnPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
	//m_hCancelbitmap = (HBITMAP) ::LoadImage(NULL,strCancelBtnPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
	//m_hExitbitmap = (HBITMAP)  ::LoadImage(NULL,strExitBtnPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  

	//读取配置文件信息
	m_nNeedCFCA = 0;
	BOOL bSupportCFCA = TRUE;
	CPublic::Instance()->GetLoginInfo(m_strUserName,m_strMemPwd,m_bSavePassword,m_bSaveAccount,m_nNeedCFCA,bSupportCFCA);

	if (!bSupportCFCA)
	{
		GetDlgItem(IDC_RADIO1)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
	}

	if(!m_strMemPwd.IsEmpty())
	{
		m_strPassword ="software";
	}	
//	SetWindowText(CPublic::Instance()->GetProductName());
	m_bPwdChanged = FALSE;
	
	// add end
	m_progressStep.ShowWindow(FALSE);

	//m_LoginBtn.SetBitmap(m_hLoginbitmap);
	//m_ExitBtn.SetBitmap(m_hExitbitmap);

	vector<CString> vecServerList;
	CPublic::Instance()->GetServerNameList(vecServerList);
	vector<CString>::iterator it;
	for(it= vecServerList.begin();it != vecServerList.end();it++)
	{
		m_commServerList.AddString(*it);
	}
	int nCurSel;
	nCurSel = CPublic::Instance()->GetCurSelServer();
	if(vecServerList.size()>0&& nCurSel <vecServerList.size())m_nServerSel = nCurSel;	
	
	
	if(!CPublic::Instance()->GetWarnShowStatus())
	{
		GetDlgItem(IDC_BUTTON_READWARN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_READWARNINFO)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_bCheckWarnInfo= FALSE;
		//GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(CPublic::Instance()->GetSoftwareVersion());

	UpdateData(false);
//	::SetWindowPos(m_hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	CString strLink;
	strLink = CPublic::Instance()->GetDemoLink();
	if(!strLink.IsEmpty())
	{
		m_stcDemoLink.SetHyperlink(strLink);
		m_stcDemoLink.SetCaption(CLanguageInterface::Instance()->GetStr(IDC_STATIC_REGIST));
	}
	else
	{
		m_stcDemoLink.ShowWindow(SW_HIDE);
	}

	CLanguageInterface::Instance()->InitDialog(IDD,this);
#ifdef _DEBUG
	TEXTMETRIC lpText;
	GetTextMetrics(GetWindowDC()->m_hDC,&lpText);
	m_strValidateCode = m_stcCheckCode.m_pRandStr;
	m_strPassword = _T("123456");
	m_bPwdChanged = 1;
	UpdateData(FALSE);
#endif
	//OnOK();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnOK() 
{
	UpdateData();

	if(!m_bCheckWarnInfo)
	{
		CString strTmp;
		strTmp = CLanguageInterface::Instance()->GetStr(10041);
		if(strTmp == "")
			strTmp = "请阅读风险提示后再执行登录操作！";
		MessageBox(strTmp,CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}//	if( m_bLoginStatus)
//	{
//		ReturnStart();
//		SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20066));
//	}
//	else
	{	
		m_strUserName.Trim();
		if(m_strUserName.IsEmpty())
		{
			MessageBox(CLanguageInterface::Instance()->GetStr(10009),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(m_strPassword.IsEmpty())
		{
			MessageBox(CLanguageInterface::Instance()->GetStr(10010),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
			return;
		}

		m_bLoginStatus = TRUE;

#ifndef _DEBUG	
		if(m_strValidateCode != m_stcCheckCode.m_pRandStr)
		{
			MessageBox(CLanguageInterface::Instance()->GetStr(10011),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONERROR);
			m_stcCheckCode.CreateIdentifyCode();
			m_stcCheckCode.RedrawWindow();
			GetDlgItem(IDC_EDIT_CHECKCODE)->SetFocus();
			return;
		}
#endif
		m_ExitBtn.ShowWindow(SW_HIDE);
		m_LoginBtn.ShowWindow(SW_HIDE);
		if(m_bPwdChanged)
		{	//密码更改过，需重新加密
			char szTmpPwd[100];
			int nDestLen;
			memset(szTmpPwd,0,sizeof(szTmpPwd));
			CPublic::Instance()->GetTradeDll()->m_pSHA1Encrypt((char*)(LPCTSTR)m_strPassword,m_strPassword.GetLength(),szTmpPwd,nDestLen);
			m_strMemPwd = szTmpPwd;
		}
		CPublic::Instance()->SetCurSelServer(m_nServerSel);
		CPublic::Instance()->WriteIniInfo(m_strUserName,m_strMemPwd,false,m_bSaveAccount,m_nNeedCFCA);//写入配置信息

		SetButtonStat(0,NULL);
		Open(0,100);
		//m_bLoginStatus = TRUE;
//		m_LoginBtn.SetBitmap(m_hCancelbitmap);
//		m_LoginBtn.RedrawWindow();	

		bool nConnected;
		nConnected = false;
		for(int i = 0;i < 3; i++)
		{
			if(CPublic::Instance()->InitBalanceServer())
			{
				nConnected = true;
				break;
			}
		}

		if(!nConnected)
		{
			SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(10008));
			ReturnStart();
			return;
		}

		HWND hWnd; 
		hWnd = GetSafeHwnd();
		CPublic::Instance()->ResetLoginWnd(hWnd);
//		CPublic::Instance()->InitQuote(hWnd);
		hWnd = m_pParentWnd->GetSafeHwnd();
		if(0>=CPublic::Instance()->InitTrade(hWnd))
		{
			SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(10038));
			ReturnStart();
			return;
		}
		SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20065));
		m_uiConnectTimer = SetTimer(EVENT_CONNECT_OUT,EVENT_CONNECT_OUT_TM,NULL);//等待一分钟
	} 
	//CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Implementation //////////////////////////////////////////////////

////**** public
void CLoginDlg::Open(int nMinPos, int nMaxPos)
{
    if ( !GetSafeHwnd())
	{
		Create(IDD,NULL);	
	}
	//CenterWindow();	
	
	m_progressStep.ShowWindow(TRUE);
	
	if( SetRange(nMinPos,nMaxPos) )	
	{
		ShowWindow(SW_SHOW);
	}	
}

void CLoginDlg::Close()
{
	if( NULL == GetSafeHwnd())
		return;

	CPublic::Instance()->ResetLoginWnd();
	WDialog::OnOK();
}

void CLoginDlg::ReturnStart()
{
	try{
		CPublic::Instance()->UnLinkTrade();
		CPublic::Instance()->UnLinkQuote();
		m_stcCheckCode.CreateIdentifyCode();
	}catch(...){}

	m_stcCheckCode.RedrawWindow();
	m_progressStep.SetPos(0);
	m_bLoginStatus = FALSE;
	m_LoginBtn.ShowWindow(SW_SHOW);
//	m_LoginBtn.SetBitmap(m_hLoginbitmap);
//	m_LoginBtn.RedrawWindow();
	::ShowWindow(this->m_hWnd,TRUE);//	GetDlgItem(IDOK)->SetWindowText("登陆");
	m_ExitBtn.ShowWindow(SW_SHOW);
	KillTimer(m_uiConnectTimer);
}

BOOL CLoginDlg::SetRange(int nMinPos, int nMaxPos)
{
	if( nMinPos >= nMaxPos)
	{
		Close();
		return FALSE;
	}
	m_nMinPos = nMinPos; 
	m_nMaxPos = nMaxPos;
	m_nCurPos = nMinPos+1;
	m_nMovePos = m_nCurPos;

	if( NULL ==  m_progressStep.GetSafeHwnd())
		return FALSE;

	m_progressStep.SetRange(m_nMinPos, m_nMaxPos);
	m_progressStep.SetStep(1);

	return TRUE;
}

void CLoginDlg::SetPos(int nCurPos,const char *szPrompt)
{
	if( NULL == GetSafeHwnd())
		return;
	
	m_nCurPos = nCurPos;
	if(nCurPos >= m_nMaxPos)
	{
		m_nCurPos = m_nMaxPos;
	}

    int nPercentage = ((m_nCurPos-m_nMinPos)*100)/(m_nMaxPos - m_nMinPos);

	CString strTitle;
    strTitle.Format(_T("[%d%%]"),nPercentage);
	if ( NULL != szPrompt )
	{
		strTitle += szPrompt;
	}
	m_editPrompt.ShowWindow(SW_HIDE);	
	m_editPrompt.SetWindowText(strTitle);
	m_editPrompt.ShowWindow(SW_SHOW);	
	//m_editPrompt.RedrawWindow();
	m_progressStep.SetPos(m_nCurPos);	
	if(m_nCurPos >= m_nMaxPos)
	{
		Close();
		return;
	}

	//m_progressStep.RedrawWindow();
}

void CLoginDlg::SetButtonStat(int nCount,int *pEnableBtnIDList)
{
	if( NULL == GetSafeHwnd())
		return;
	
	for( int i=0; i<nCount; i++)
	{
		CWnd *pBtn = GetDlgItem(pEnableBtnIDList[i]);
		if( pBtn && pBtn->GetSafeHwnd())
		{
			pBtn->EnableWindow(TRUE);
		}
	}
}


void CLoginDlg::OnClose() 
{
	KillTimer(m_uiConnectTimer);
	WDialog::OnClose();
}

LRESULT CLoginDlg::OnTradeMessage(WPARAM wParam,LPARAM lParam)
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
		}
		else
		{//密码验证失败
			if(hWnd)::SendMessage(hWnd,WM_SETTEXT,TRADELOGINFAILED,lParam);//(LPARAM)(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20042));
		}
	}
	return 0;
}

LRESULT CLoginDlg::OnSetTextMsg( WPARAM wParam, LPARAM lParam )
{
	CString ssShowInfo = CLanguageInterface::Instance()->GetStr(20050+wParam);
	switch( wParam) 
	{
	case QUOTELINKFAIL: // 连接失败
	case QUOTELOGINFALIED://登陆失败
	case TRADELINKFAILED:

	case QUOTEOTHERERROR:
		{
			ReturnStart();	
			SetPos(0,(char*)(LPCTSTR)ssShowInfo);
		}
		break;
	case TRADELOGINFAILED:
		{
			char* pInfo = (char*)lParam;
			ReturnStart();	
			SetPos(0,pInfo);
		}
		break;
	case QUOTELINKSUCCESS: //连接成功
		{
			SetPos(60,(char*)(LPCTSTR)ssShowInfo);
			ShowWindow(SW_SHOW);
		}
		break;
	case QUOTELOGINSUCCESS: //登陆成功
		{
			SetPos(80,(char*)(LPCTSTR)ssShowInfo);
		}
		break;
	case TRADELINKSUCCESS: //交易连接成功
		{
			SetPos(20,(char*)(LPCTSTR)ssShowInfo);
		}
		break;
	case  TRADELOGINSUCCESS: //交易登陆成功
		{
			SetPos(40,(char*)(LPCTSTR)ssShowInfo);
			ShowWindow(SW_SHOW);
			CPublic::Instance()->SendLinkStatus(3,TRUE);
		}
		break;
	case QUOTEOTHERSUCCESS://行情其他信息接收完毕
		{
			//if (IsWindow(AfxGetMainWnd()->GetSafeHwnd()))
			//{
			//	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_TRADELOGININFO,MAINFRAME_LINKMSG,MAINFRAME_LINKSTATUS_LINKED);
			//}
			SetPos(100,(char*)(LPCTSTR)ssShowInfo);
			CWnd *pWnd = CPublic::Instance()->GetView(11,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				pWnd->PostMessage(WM_NOTIFY_LOGIN_SUC);
			}
			Close();
		}
		break;
	default:
		ReturnStart();
		break;
	}
	if(lParam)
	{
		//CString strer = CString((char*)lParam);
		//MessageBox(strer,_T("11111"));
		//MessageBox(CString((char*)lParam),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
	}
	return 1;
}

void CLoginDlg::OnEnChangePasswordLogin()
{
	m_bPwdChanged = TRUE;
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return WDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    HBRUSH hbr = WDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG
	//	|| nCtlColor == CTLCOLOR_BTN 
		|| nCtlColor ==  CTLCOLOR_STATIC
		)//|| nCtlColor == CTLCOLOR_EDIT)
	{   
		pDC->SetBkMode(TRANSPARENT);  
		//pDC->SetTextColor(RGB(255,255,255));
		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	} 
	else if(pWnd->GetDlgCtrlID() == IDC_COMBO_SERVERLIST)
	{ 
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(0,0,0));
		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	return hbr;
}


BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
	return WDialog::OnEraseBkgnd(pDC);
}


void CLoginDlg::OnBnClickedCancel()
{
	WDialog::OnCancel();
}


void CLoginDlg::OnBnClickedButtonReadwarn()
{
	CDlgWarnInfo info;
	info.DoModal();
}

void CLoginDlg::OnBnClickedCheckReadwarninfo()
{
	UpdateData();
	if(m_bCheckWarnInfo)
	{
		//GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
	{
		//GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}


void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	WDialog::OnTimer(nIDEvent);
	if(m_uiConnectTimer == nIDEvent)
	{
		SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20069));
		ReturnStart();
	}
}

void CLoginDlg::OnStnClickedStcCheckcode()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_stcCheckCode.OnStnClicked();
}

void CLoginDlg::OnStnDblclickStcCheckcode()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

//void CLoginDlg::InitCtrl()
//{
//	CRect rcClt;
//	GetClientRect(&rcClt);
//
//	int istatHight = 20;
//	int iEdtHight = 20;
//	int istatWith = 80;
//	int iEdtWith = 200;
//	int iGap = 10;
//
//	int iOffL = 120;
//	int iOffT = 5;
//	CPoint pt(rcClt.left + iOffL,rcClt.top+iOffT);
//	CPoint ptBase(pt);
//	pt.Offset(istatWith+iGap,0);
//	int iRadioWith = 100;
//	m_btnRadio1.MoveWindow(pt.x,pt.y,iRadioWith,istatHight);
//	pt.Offset(iRadioWith,0);
//	m_btnRadio2.MoveWindow(pt.x,pt.y,iRadioWith,istatHight);
//
//	ptBase.Offset(0,istatHight+iGap);
//	pt = ptBase;
//	m_statServer.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
//	pt.Offset(istatWith+iGap,0);
//	m_commServerList.MoveWindow(pt.x,pt.y,iEdtWith,iEdtHight);
//	pt.Offset(iEdtWith+iGap,0);
//	m_stcDemoLink.MoveWindow(pt.x,pt.y,istatWith,istatHight);
//
//	ptBase.Offset(0,istatHight+iGap);
//	pt = ptBase;
//	m_statAccount.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
//	pt.Offset(istatWith+iGap,0);
//	m_edtUserName.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
//	pt.Offset(iEdtWith+iGap,0);
//	m_btnSaveAccount.MoveWindow(pt.x,pt.y,150,istatHight);
//
//	ptBase.Offset(0,istatHight+iGap);
//	pt = ptBase;
//	m_statPwd.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
//	pt.Offset(istatWith+iGap,0);
//	m_edtPassWord.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
//	pt.Offset(iEdtWith+iGap,0);
//	m_btnSavePassWord.MoveWindow(pt.x,pt.y,150,istatHight);
//
//	ptBase.Offset(0,istatHight+iGap);
//	pt = ptBase;
//	m_statCheckCode.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
//	pt.Offset(istatWith+iGap,0);
//	m_edtCheckCode.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
//	pt.Offset(iEdtWith+iGap,0);
//	m_stcCheckCode.MoveWindow(pt.x,pt.y-3,90,25);
//
//	ptBase.Offset(0,istatHight+iGap);
//	pt = ptBase;
//	m_btnReadWarnInfo.MoveWindow(pt.x+55,pt.y,220,istatHight);
//	pt.Offset(190+iGap,0);
//	m_btnReadWarn.MoveWindow(pt.x+120,pt.y,132,23);
//
//	ptBase.Offset(90,istatHight+iGap*2);
//	pt = ptBase;
//	int ibtnW = 91;
//	int ibtnH = 37;
//	m_LoginBtn.MoveWindow(pt.x,pt.y,ibtnW,ibtnH);
//	pt.Offset(iGap*15,0);
//	m_ExitBtn.MoveWindow(pt.x,pt.y,ibtnW,ibtnH);
//
//	CString strParamPath = CPublic::Instance()->GetSysPath();
//	strParamPath += _T("interface\\Login\\");
//	m_LoginBtn.SetMode(0,strParamPath+_T("ImgOkBtn .png"),4,2);
//	m_LoginBtn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),24,10);
//	m_ExitBtn.SetMode(0,strParamPath+_T("ImgCancel.png"),4,2);
//	m_ExitBtn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),24,10);
//
//	m_btnReadWarn.SetMode(0,strParamPath+_T("ImgReadWarn.png"),4,2);
//	m_btnReadWarn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),2,2);
//
//	m_editPrompt.MoveWindow(rcClt.left + 30,rcClt.bottom-85,rcClt.Width()-60,20);
//	m_progressStep.MoveWindow(rcClt.left + 60,rcClt.bottom-60,rcClt.Width()-120,15);
//	
//	m_statVersionName.MoveWindow(rcClt.right - 125,rcClt.bottom-40,25,20);
//	m_statVersion.MoveWindow(rcClt.right - 100,rcClt.bottom-40,100,20);
//}
void CLoginDlg::InitCtrl()
{
	CRect rcClt;
	GetClientRect(&rcClt);

	if (CPublic::Instance()->GetUseLgOld() == 0)
	{
		int istatHight = 20;
		int iEdtHight = 20;
		int istatWith = 100;
		int iEdtWith = 200;
		int iGap = 10;
		int iOffWS = 20;

		int iOffL = 100;
		int iOffT = 100;
		CPoint pt(rcClt.left + iOffL,rcClt.top+iOffT);
		CPoint ptBase(pt);
		pt.Offset(istatWith+iGap,0);
		int iRadioWith = 100;
		m_btnRadio1.MoveWindow(pt.x,pt.y,iRadioWith,istatHight);
		pt.Offset(iRadioWith,0);
		m_btnRadio2.MoveWindow(pt.x,pt.y,iRadioWith,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statServer.MoveWindow(pt.x+iOffWS,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_commServerList.MoveWindow(pt.x,pt.y,iEdtWith,iEdtHight);
		pt.Offset(iEdtWith+iGap,0);
		m_stcDemoLink.MoveWindow(pt.x,pt.y,istatWith,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statAccount.MoveWindow(pt.x+iOffWS,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_edtUserName.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
		pt.Offset(iEdtWith+iGap,0);
		m_btnSaveAccount.MoveWindow(pt.x,pt.y,150,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statPwd.MoveWindow(pt.x+iOffWS,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_edtPassWord.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
		pt.Offset(iEdtWith+iGap,0);
		m_btnSavePassWord.MoveWindow(pt.x,pt.y,150,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statCheckCode.MoveWindow(pt.x+iOffWS,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_edtCheckCode.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
		pt.Offset(iEdtWith+iGap,0);
		m_stcCheckCode.MoveWindow(pt.x,pt.y-5,100,30);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_btnReadWarnInfo.MoveWindow(pt.x+55,pt.y,220,istatHight);
		pt.Offset(190+iGap,0);
		//m_btnReadWarn.MoveWindow(pt.x+120,pt.y,80,istatHight);
		m_btnReadWarn.MoveWindow(pt.x+120,pt.y,132,23);

		ptBase.Offset(50,istatHight+iGap*2);
		pt = ptBase;
		int ibtnW = 91;
		int ibtnH = 37;
		m_LoginBtn.MoveWindow(pt.x,pt.y,ibtnW,ibtnH);
		pt.Offset(iGap*20,0);
		m_ExitBtn.MoveWindow(pt.x,pt.y,ibtnW,ibtnH);

		CString strParamPath = CPublic::Instance()->GetSysPath();
		strParamPath += _T("interface\\Login\\");
		m_LoginBtn.SetMode(0,strParamPath+_T("ImgOkBtn .png"),4,2);
		m_LoginBtn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),24,10);
		m_ExitBtn.SetMode(0,strParamPath+_T("ImgCancel.png"),4,2);
		m_ExitBtn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),24,10);

		m_btnReadWarn.SetMode(0,strParamPath+_T("ImgReadWarn.png"),4,2);
		m_btnReadWarn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),2,2);

		m_editPrompt.MoveWindow(rcClt.left + 30,rcClt.bottom-85,rcClt.Width()-60,20);
		m_progressStep.MoveWindow(rcClt.left + 30,rcClt.bottom-60,rcClt.Width()-60,15);

		m_statVersionName.MoveWindow(rcClt.right - 125,rcClt.bottom-40,25,20);
		m_statVersion.MoveWindow(rcClt.right - 100,rcClt.bottom-40,100,20);
	}
	else
	{
		int istatHight = 20;
		int iEdtHight = 20;
		int istatWith = 80;
		int iEdtWith = 200;
		int iGap = 10;

		int iOffL = 120;
		int iOffT = 5;
		CPoint pt(rcClt.left + iOffL,rcClt.top+iOffT);
		CPoint ptBase(pt);
		pt.Offset(istatWith+iGap,0);
		int iRadioWith = 100;
		m_btnRadio1.MoveWindow(pt.x,pt.y,iRadioWith,istatHight);
		pt.Offset(iRadioWith,0);
		m_btnRadio2.MoveWindow(pt.x,pt.y,iRadioWith,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statServer.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_commServerList.MoveWindow(pt.x,pt.y,iEdtWith,iEdtHight);
		pt.Offset(iEdtWith+iGap,0);
		m_stcDemoLink.MoveWindow(pt.x,pt.y,istatWith,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statAccount.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_edtUserName.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
		pt.Offset(iEdtWith+iGap,0);
		m_btnSaveAccount.MoveWindow(pt.x,pt.y,150,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statPwd.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_edtPassWord.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
		pt.Offset(iEdtWith+iGap,0);
		m_btnSavePassWord.MoveWindow(pt.x,pt.y,150,istatHight);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_statCheckCode.MoveWindow(pt.x,pt.y+2,istatWith,istatHight);
		pt.Offset(istatWith+iGap,0);
		m_edtCheckCode.MoveWindow(pt.x,pt.y,iEdtWith,istatHight);
		pt.Offset(iEdtWith+iGap,0);
		m_stcCheckCode.MoveWindow(pt.x,pt.y-3,90,25);

		ptBase.Offset(0,istatHight+iGap);
		pt = ptBase;
		m_btnReadWarnInfo.MoveWindow(pt.x+55,pt.y,220,istatHight);
		pt.Offset(190+iGap,0);
		m_btnReadWarn.MoveWindow(pt.x+120,pt.y,132,23);

		ptBase.Offset(90,istatHight+iGap*2);
		pt = ptBase;
		int ibtnW = 91;
		int ibtnH = 37;
		m_LoginBtn.MoveWindow(pt.x,pt.y,ibtnW,ibtnH);
		pt.Offset(iGap*15,0);
		m_ExitBtn.MoveWindow(pt.x,pt.y,ibtnW,ibtnH);

		CString strParamPath = CPublic::Instance()->GetSysPath();
		strParamPath += _T("interface\\Login\\");
		m_LoginBtn.SetMode(0,strParamPath+_T("ImgOkBtn .png"),4,2);
		m_LoginBtn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),24,10);
		m_ExitBtn.SetMode(0,strParamPath+_T("ImgCancel.png"),4,2);
		m_ExitBtn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),24,10);

		m_btnReadWarn.SetMode(0,strParamPath+_T("ImgReadWarn.png"),4,2);
		m_btnReadWarn.SetFontStyle(18,1,Color::MakeARGB(255,211,211,211),2,2);

		m_editPrompt.MoveWindow(rcClt.left + 30,rcClt.bottom-85,rcClt.Width()-60,20);
		m_progressStep.MoveWindow(rcClt.left + 60,rcClt.bottom-60,rcClt.Width()-120,15);

		m_statVersionName.MoveWindow(rcClt.right - 125,rcClt.bottom-40,25,20);
		m_statVersion.MoveWindow(rcClt.right - 100,rcClt.bottom-40,100,20);
	}
}

//void OperationHollowOutCharacters()  
//{  
//	//  
//	CRect   rectCharacters(0, 0, Dialog_Width, HollowOutCharachers_Height);  
//
//	//  
//	if (!m_hDCMem && !m_hBitmap && !m_hOldBitmap)  
//	{  
//		//创建相关资源  
//		HDC     hDC = ::GetDC(NULL);  
//		m_hDCMem = CreateCompatibleDC(hDC);  
//		m_hBitmap = CreateCompatibleBitmap(hDC, rectCharacters.Width(), rectCharacters.Height());  
//		m_hOldBitmap = (HBITMAP)::SelectObject(m_hDCMem, m_hBitmap);  
//		::SetStretchBltMode(m_hDCMem, COLORONCOLOR);  
//
//		//hBmp资源由img释放  
//		{  
//			DWORD   dwID = m_bUnknownSourceChecked ? IDB_PNG_InstallMarket_UnknownChecked : IDB_PNG_InstallMarket_UnknownUnchecked;  
//			HBITMAP hBmp = AtlLoadGdiplusImage((_U_STRINGorID)dwID, _T("png")); //该处可由CImage.LoadFromResource实现  
//			CImage  img;  
//			img.Attach(hBmp);  
//			img.StretchBlt(m_hDCMem, rectCharacters, CRect(0, 0, img.GetWidth(), img.GetHeight()), SRCCOPY);  
//		}  
//
//		//整个客户区域显示，下面将不显示的进行RGN_XOR  
//		if (!m_hRgnWnd)  
//			m_hRgnWnd = ::CreateRectRgn(0, 0, Dialog_Width, Dialog_Height);  
//
//		//双层循环检查图像颜色，白色的过滤或只有黑色的才能显示  
//		COLORREF    color;  
//		COLORREF    colorCmp = RGB(255, 255, 255);  //RGB(0, 0, 0)  
//		HRGN        hRgnTemp;  
//		for (int i = 0; i < rectCharacters.Width(); ++i)  
//		{  
//			for (int j = 0; j < rectCharacters.Height(); ++j)  
//			{  
//				color = ::GetPixel(m_hDCMem, i, j);  
//				//条件二选一：颜色不是黑色或颜色是白色  
//				if (color == colorCmp)  
//				{  
//					hRgnTemp = ::CreateRectRgn(i, j, i + 1, j + 1);  
//					::CombineRgn(m_hRgnWnd, m_hRgnWnd, hRgnTemp, RGN_XOR);  
//					::DeleteObject(hRgnTemp);  
//				}  
//			}  
//		}  
//
//		//释放gdi资源  
//		::ReleaseDC(NULL, hDC);  
//	}  
//} 
//为使得窗口拥有透明属性，需要窗口拥有WS_EX_LAYERED属性  
//long    lStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);  
//::SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);  
//::SetLayeredWindowAttributes(m_hWnd, 0, 220, LWA_ALPHA);  
//
////设置镂空布局  
//if (m_hRgnWnd)  
//	::SetWindowRgn(m_hWnd, m_hRgnWnd, TRUE);  
//
//注意窗口关闭时的资源释放
//	[cpp] view plaincopy
//	void ReleaseResource()  
//{  
//	if (m_hDCMem && m_hBitmap && m_hOldBitmap)  
//	{  
//		::SelectObject(m_hDCMem, m_hOldBitmap);  
//		::DeleteObject(m_hBitmap);  
//		::DeleteDC(m_hDCMem);  
//	}  
//	m_hDCMem = NULL;  
//	m_hBitmap = NULL;  
//	m_hOldBitmap = NULL;  
//
//	if (m_hRgnWnd)  
//	{  
//		::DeleteObject(m_hRgnWnd);  
//		m_hRgnWnd = NULL;  
//	}  
//}  
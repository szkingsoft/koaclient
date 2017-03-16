// DlgLogin1.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgLogin1.h"
#include "afxdialogex.h"
#include "PictureEx.h"
#include "DlgAgentSetting.h"

// CDlgLogin1 对话框

IMPLEMENT_DYNAMIC(CDlgLogin1, CDialog)

CDlgLogin1::CDlgLogin1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin1::IDD, pParent)
	, m_strCheckCode(_T(""))
	, m_nServerSel(0)
	, m_nSelLanguage(0)
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
	, m_bSaveAccount(FALSE)
	, m_nNeedCFCA(0)
{
	m_bLoginStatus = FALSE;
}

CDlgLogin1::~CDlgLogin1()
{
}

void CDlgLogin1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CHECKCODE, m_stcCheckCode);
	DDX_Text(pDX, IDC_EDIT_CHECKCODE, m_strCheckCode);
	DDX_CBIndex(pDX, IDC_COMBO_SERVERTYPE, m_nServerSel);
	DDX_CBIndex(pDX, IDC_COMBO_LANGUAGE, m_nSelLanguage);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHECK_SAVEACCOUNT, m_bSaveAccount);
	DDX_Control(pDX, IDC_COMBO_SERVERTYPE, m_commServerList);
	DDX_Control(pDX, IDC_STATIC_PIC, m_ctrlGifTitle);
	DDX_Control(pDX, IDC_STATIC_PROMIT, m_editPrompt);
	DDX_Radio(pDX, IDC_RADIO1, m_nNeedCFCA);
}


BEGIN_MESSAGE_MAP(CDlgLogin1, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLogin1::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN__LINKTEST, &CDlgLogin1::OnBnClickedBtn)
	ON_BN_CLICKED(IDC_BTN_AGENTSETTING, &CDlgLogin1::OnBnClickedBtnAgentsetting)
	ON_BN_CLICKED(IDCANCEL, &CDlgLogin1::OnBnClickedCancel)
	ON_MESSAGE( WM_SETTEXT, OnSetTextMsg )
	ON_STN_CLICKED(IDC_STATIC_CHECKCODE, &CDlgLogin1::OnStnClickedStaticCheckcode)
END_MESSAGE_MAP()


// CDlgLogin1 消息处理程序
BOOL CDlgLogin1::OnInitDialog()
{	
	CDialog::OnInitDialog();
	BOOL bSavePassword;

	m_nNeedCFCA = 0;
	BOOL bSupportCFCA = TRUE;
	CPublic::Instance()->GetLoginInfo(m_strUserName,m_strMemPwd,bSavePassword,m_bSaveAccount,m_nNeedCFCA,bSupportCFCA);

	if (!bSupportCFCA)
	{
		GetDlgItem(IDC_RADIO1)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
	}

	if(!m_strMemPwd.IsEmpty())
	{
		m_strPassword ="software";
	}
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

	CString szPath;
	szPath = CPublic::Instance()->GetSysPath();
	szPath +="title.gif";	
	if(m_ctrlGifTitle.Load(szPath))m_ctrlGifTitle.Draw();
	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(CPublic::Instance()->GetSoftwareVersion());
	UpdateData(FALSE);
	return TRUE;
}


void CDlgLogin1::OnBnClickedOk()
{
	UpdateData();

	if( m_bLoginStatus)
	{
		ReturnStart();
		SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20066));
	}
	else
	{	
		if(m_strUserName.IsEmpty())
		{
			AfxMessageBox(CLanguageInterface::Instance()->GetStr(10009));
			return;
		}
		if(m_strPassword.IsEmpty())
		{
			AfxMessageBox(CLanguageInterface::Instance()->GetStr(10010));
			return;
		}

		m_bLoginStatus = TRUE;

#ifndef _DEBUG	
		if(m_strCheckCode != m_stcCheckCode.m_pRandStr)
		{
			AfxMessageBox(CLanguageInterface::Instance()->GetStr(10011),MB_OK|MB_ICONERROR);
			m_stcCheckCode.CreateIdentifyCode();
			m_stcCheckCode.RedrawWindow();
			GetDlgItem(IDC_EDIT_CHECKCODE)->SetFocus();
			return;
		}
#endif
		GetDlgItem(IDOK)->SetWindowText(CLanguageInterface::Instance()->GetStr(102));
		GetDlgItem(IDOK)->RedrawWindow();

		{	//密码更改过，需重新加密
			char szTmpPwd[100];
			int nDestLen;
			memset(szTmpPwd,0,sizeof(szTmpPwd));
			CPublic::Instance()->GetTradeDll()->m_pSHA1Encrypt((char*)(LPCTSTR)m_strPassword,m_strPassword.GetLength(),szTmpPwd,nDestLen);
			m_strMemPwd = szTmpPwd;
		}
		CPublic::Instance()->SetCurSelServer(m_nServerSel);
		CPublic::Instance()->WriteIniInfo(m_strUserName,m_strMemPwd,FALSE,m_bSaveAccount,m_nNeedCFCA);//写入配置信息

		SetButtonStat(0,NULL);
		Open(0,100);

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
		if(0>= CPublic::Instance()->InitTrade(hWnd))
		{
			SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(10038));
			ReturnStart();
			return;
		}
		SetPos(0,(char*)(LPCTSTR)CLanguageInterface::Instance()->GetStr(20065));
	}
}


void CDlgLogin1::OnBnClickedBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgLogin1::OnBnClickedBtnAgentsetting()
{
	CDlgAgentSetting setting;
	setting.DoModal();
}


void CDlgLogin1::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


////**** public
void CDlgLogin1::Open(int nMinPos, int nMaxPos)
{
    if ( !GetSafeHwnd())
	{
		Create(IDD,NULL);	
	}
	CenterWindow();	
	
	
	if( SetRange(nMinPos,nMaxPos) )	
	{
		ShowWindow(SW_SHOW);
	}	
}

void CDlgLogin1::Close()
{
	if( NULL == GetSafeHwnd())
		return;
	CPublic::Instance()->ResetLoginWnd();
	CDialog::OnOK();
}

void CDlgLogin1::ReturnStart()
{
	try{
		CPublic::Instance()->UnLinkTrade();
		CPublic::Instance()->UnLinkQuote();
		m_stcCheckCode.CreateIdentifyCode();
	}catch(...){}

	m_stcCheckCode.RedrawWindow();
	m_bLoginStatus = FALSE;
	GetDlgItem(IDOK)->SetWindowText(CLanguageInterface::Instance()->GetStr(101));
	::ShowWindow(this->m_hWnd,TRUE);//	GetDlgItem(IDOK)->SetWindowText("登陆");
}

BOOL CDlgLogin1::SetRange(int nMinPos, int nMaxPos)
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

	return TRUE;
}

void CDlgLogin1::SetPos(int nCurPos,const char *szPrompt)
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

	if(m_nCurPos >= m_nMaxPos)
	{
		Close();
		return;
	}
	//m_progressStep.RedrawWindow();
}

void CDlgLogin1::SetButtonStat(int nCount,int *pEnableBtnIDList)
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


LRESULT CDlgLogin1::OnSetTextMsg( WPARAM wParam, LPARAM lParam )
{
	CString ssShowInfo = CLanguageInterface::Instance()->GetStr(20050+wParam);
	switch( wParam) 
	{
	case QUOTELINKFAIL: // 连接失败
	case QUOTELOGINFALIED://登陆失败
	case TRADELINKFAILED:
	case QUOTEOTHERERROR:
		{
			SetPos(0,(char*)(LPCTSTR)ssShowInfo);
			ReturnStart();
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
		}
		break;
	case QUOTEOTHERSUCCESS://行情其他信息接收完毕
		{
			//SetPos(100,ssShowInfo.c_str());
			Close();
		}
		break;
	default:
		ReturnStart();
		break;
	}
	if(lParam)
	{
		AfxMessageBox(CString((char*)lParam));
	}
	return 1;
}


void CDlgLogin1::OnStnClickedStaticCheckcode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_stcCheckCode.OnStnClicked();
}

// WelcomeInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "koaclient.h"
#include "WelcomeInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWelcomeInfoDlg dialog


CWelcomeInfoDlg::CWelcomeInfoDlg(int nSystemUpdateType,CString strSystemMessage, CWnd* pParent /*=NULL*/)
	: CDialog(CWelcomeInfoDlg::IDD, pParent)
{
	m_nSystemUpdateType = nSystemUpdateType;
	m_szNoticeInfo = strSystemMessage;
	//{{AFX_DATA_INIT(CWelcomeInfoDlg)
	//}}AFX_DATA_INIT
}


void CWelcomeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomeInfoDlg)
	DDX_Control(pDX, IDC_NOTICECHK, m_chkNotice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcomeInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CWelcomeInfoDlg)
	ON_BN_CLICKED(IDC_NOTICECHK, OnNoticechk)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CWelcomeInfoDlg::OnCancel)
	ON_BN_CLICKED(ID_BTN_SYSUPDATE, &CWelcomeInfoDlg::OnBnClickedBtnSysupdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomeInfoDlg message handlers

void CWelcomeInfoDlg::OnCancel()
{
	if( m_nSystemUpdateType == AUTOUPDATE_MUST)
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLOSE,0,0);
		return;
	}

	CDialog::OnCancel();
}

void CWelcomeInfoDlg::OnNoticechk() 
{
	int nNoticed,nNoticeDate;
	if(m_chkNotice.GetCheck())
		nNoticed = 1;
	else
		nNoticed = 0;

	CTime tNow = CTime::GetCurrentTime();
	nNoticeDate = tNow.GetYear()*10000+tNow.GetMonth()*100+ tNow.GetDay();
	CPublic::Instance()->WriteNoticeInfo(nNoticed,nNoticeDate);
}

BOOL CWelcomeInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	
	m_ctrlNoticeInfo.CreateFromStatic(IDC_STATIC_NOTICEINFO,this);

	m_ctrlNoticeInfo.SetNewHTMLContent(m_szNoticeInfo,TRUE);
	//////////////////////////////////////////////////////////////////////////

	switch( m_nSystemUpdateType) 
	{
	case AUTOUPDATE_NORMAL:
		{
			SetWindowText(CLanguageInterface::Instance()->GetStr(10020));
			GetDlgItem(ID_BTN_SYSUPDATE)->ShowWindow(TRUE);
		}
		break;
	case AUTOUPDATE_MUST:
		{
			SetWindowText(CLanguageInterface::Instance()->GetStr(10021));
			GetDlgItem(IDC_NOTICECHK)->ShowWindow(FALSE);
		}
		break;
	default:
		{
//			GetDlgItem(IDOK)->ShowWindow(FALSE);
		}
	    break;
	}
	CLanguageInterface::Instance()->InitDialog(IDD,this);
	return TRUE;
}

void CWelcomeInfoDlg::OnBnClickedBtnSysupdate()
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

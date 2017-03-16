// SysProtyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "koaclient.h"
#include "SysProtyDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysProtyDlg dialog
CSysProtyDlg::CSysProtyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysProtyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysProtyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPageType = 0;
}

void CSysProtyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysProtyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysProtyDlg, CDialog)
	//{{AFX_MSG_MAP(CSysProtyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysProtyDlg message handlers

BOOL CSysProtyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//获取应用程序路径
	m_szAppPath = CPublic::Instance()->GetSysPath();

	//添加属性页
	m_sheet.AddPage(&sysPage);
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);
	m_sheet.SetActivePage(m_nPageType);

	RECT rect;
	m_sheet.GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	m_sheet.SetTitle(CLanguageInterface::Instance()->GetStr(170));
	
	m_sheet.SetWindowPos(NULL, 0, 0, width, height, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	CLanguageInterface::Instance()->InitDialog(IDD,this);
	return TRUE;
}

void CSysProtyDlg::OnOK()
{
	if(sysPage.IsSetActived())
	{
	  	sysPage.SaveInterfaceParam();
	}

	CDialog::OnOK();
}

void CSysProtyDlg::SetActvPage(int nPage)
{
	m_nPageType = nPage;
}
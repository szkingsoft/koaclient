// DlgMainWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgMainWnd.h"


// CDlgMainWnd 对话框

IMPLEMENT_DYNAMIC(CDlgMainWnd, CDialog)

CDlgMainWnd::CDlgMainWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMainWnd::IDD/*,_T("CDlgMainWnd")*/, pParent)
{

}

CDlgMainWnd::~CDlgMainWnd()
{
}

void CDlgMainWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMainWnd, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgMainWnd 消息处理程序


void CDlgMainWnd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if(IDYES != MessageBox(CLanguageInterface::Instance()->GetStr(100140),CPublic::Instance()->GetProductName(), MB_YESNO|MB_ICONQUESTION))return;
	//::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLOSE,0,0);
	CDialog::OnClose();
}


BOOL CDlgMainWnd::OnInitDialog()
{
	//SetBaseDlgMode(300);
	CDialog::OnInitDialog();
	//SetTitleLogo(_T("D:\\client20140526\\tradeclient\\prog\\interface\\Logo.png"),0,0);

	//SetWndPos();
	//MoveWindow(0,0,800,600);
	//CenterWindow();
	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (!IsWindow(m_DlgMainClient.GetSafeHwnd())) return;

	m_DlgMainClient.MoveWindow(0,0,cx,cy);
}

void CDlgMainWnd::InitCtrl()
{
	m_DlgMainClient.Create(CDlgMainClient::IDD,this);
	m_DlgMainClient.ShowWindow(SW_SHOW);
}
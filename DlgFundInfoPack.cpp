// DlgFundInfoPack.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgFundInfoPack.h"



// CDlgFundInfoPack 对话框

IMPLEMENT_DYNAMIC(CDlgFundInfoPack, CDialog)

CDlgFundInfoPack::CDlgFundInfoPack(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFundInfoPack::IDD, pParent)
{

}

CDlgFundInfoPack::~CDlgFundInfoPack()
{
}

void CDlgFundInfoPack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFundInfoPack, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgFundInfoPack 消息处理程序


BOOL CDlgFundInfoPack::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgFundInfoPack::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcTrade(0,0,cx,cy);
	SetTradeWndPos(rcTrade);
}

void CDlgFundInfoPack::InitCtrl()
{
	InitTradeWnd();
}

void CDlgFundInfoPack::InitTradeWnd()
{
	m_dlgTradeInfo.SetIndex(3);
	m_dlgTradeInfo.Create(CDlgTradeInfo::IDD,this);
	m_dlgTradeInfo.ShowWindow(SW_SHOW);
}

void CDlgFundInfoPack::SetTradeWndPos(CRect &rcWnd)
{
	//CPublic::Instance()->GetTradeDll()->m_pMoveDlg(rcWnd);
	if (!IsWindow(m_dlgTradeInfo.GetSafeHwnd())) return;

	m_dlgTradeInfo.MoveWindow(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());
}
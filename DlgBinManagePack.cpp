// DlgBinManagePack.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgBinManagePack.h"



// CDlgBinManagePack 对话框

IMPLEMENT_DYNAMIC(CDlgBinManagePack, CDialog)

CDlgBinManagePack::CDlgBinManagePack(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBinManagePack::IDD, pParent)
{

}

CDlgBinManagePack::~CDlgBinManagePack()
{
}

void CDlgBinManagePack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBinManagePack, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgBinManagePack 消息处理程序


BOOL CDlgBinManagePack::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBinManagePack::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcTrade(0,0,cx,cy);
	SetTradeWndPos(rcTrade);
}

void CDlgBinManagePack::InitCtrl()
{
	InitTradeWnd();
}

void CDlgBinManagePack::InitTradeWnd()
{
	m_dlgTradeInfo.SetIndex(2);
	m_dlgTradeInfo.Create(CDlgTradeInfo::IDD,this);
	m_dlgTradeInfo.ShowWindow(SW_SHOW);
}

void CDlgBinManagePack::SetTradeWndPos(CRect &rcWnd)
{
	//CPublic::Instance()->GetTradeDll()->m_pMoveDlg(rcWnd);
	if (!IsWindow(m_dlgTradeInfo.GetSafeHwnd())) return;

	m_dlgTradeInfo.MoveWindow(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());
}

BOOL CDlgBinManagePack::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	
	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
void CDlgBinManagePack::NotifyTradeBin(UINT iMsg,WPARAM wPar, LPARAM lPar)
{
	CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(iMsg,2,0);
}

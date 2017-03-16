// DlgTradeInfoPop.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgTradeInfoPop.h"


// CDlgTradeInfoPop 对话框

IMPLEMENT_DYNAMIC(CDlgTradeInfoPop, CDialog)

CDlgTradeInfoPop::CDlgTradeInfoPop(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTradeInfoPop::IDD, pParent)
{

}

CDlgTradeInfoPop::~CDlgTradeInfoPop()
{
}

void CDlgTradeInfoPop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTradeInfoPop, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgTradeInfoPop 消息处理程序


BOOL CDlgTradeInfoPop::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTradeInfoPop::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (IsWindow(m_dlgTradeInfo.GetSafeHwnd())) 
	{
		m_dlgTradeInfo.MoveWindow(0,0,cx,cy);
	}
}
void CDlgTradeInfoPop::InitCtrl()
{
	InitTradeWnd();
}
void CDlgTradeInfoPop::InitTradeWnd()
{
	m_dlgTradeInfo.SetIndex(1);
	m_dlgTradeInfo.Create(CDlgTradeInfo::IDD,this);

	m_dlgTradeInfo.ShowWindow(SW_SHOW);
}

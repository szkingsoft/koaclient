// DlgFundInfoPack.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgFundInfoPack.h"



// CDlgFundInfoPack �Ի���

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


// CDlgFundInfoPack ��Ϣ�������


BOOL CDlgFundInfoPack::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgFundInfoPack::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
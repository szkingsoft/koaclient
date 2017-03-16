// DlgBinManagePack.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgBinManagePack.h"



// CDlgBinManagePack �Ի���

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


// CDlgBinManagePack ��Ϣ�������


BOOL CDlgBinManagePack::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgBinManagePack::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

// DlgTradeInfoPop.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgTradeInfoPop.h"


// CDlgTradeInfoPop �Ի���

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


// CDlgTradeInfoPop ��Ϣ�������


BOOL CDlgTradeInfoPop::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTradeInfoPop::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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

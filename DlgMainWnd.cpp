// DlgMainWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgMainWnd.h"


// CDlgMainWnd �Ի���

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


// CDlgMainWnd ��Ϣ�������


void CDlgMainWnd::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (!IsWindow(m_DlgMainClient.GetSafeHwnd())) return;

	m_DlgMainClient.MoveWindow(0,0,cx,cy);
}

void CDlgMainWnd::InitCtrl()
{
	m_DlgMainClient.Create(CDlgMainClient::IDD,this);
	m_DlgMainClient.ShowWindow(SW_SHOW);
}
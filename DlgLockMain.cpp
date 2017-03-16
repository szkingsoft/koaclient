// DlgLockMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgLockMain.h"
//#include "afxdialogex.h"


// CDlgLockMain �Ի���

IMPLEMENT_DYNAMIC(CDlgLockMain, CDialog)

CDlgLockMain::CDlgLockMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLockMain::IDD, pParent)
{

}

CDlgLockMain::~CDlgLockMain()
{
}

void CDlgLockMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_TRADEPW, m_statTradePW);
	DDX_Control(pDX, IDC_EDIT_TRADEPW, m_edtTradePW);
	DDX_Control(pDX, ID_BTN_OK, m_btnOk);
	DDX_Control(pDX, IDC_STAT_TIPS, m_statTips);
}


BEGIN_MESSAGE_MAP(CDlgLockMain, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_BTN_OK, &CDlgLockMain::OnBnClickedBtnOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgLockMain ��Ϣ�������


BOOL CDlgLockMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	////SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDlgLockMain::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnOk();
		}
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgLockMain::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ReArrayCtrl();
}

void CDlgLockMain::ReArrayCtrl()
{
	CRect rcClt;
	GetClientRect(&rcClt);

	if (!IsWindow(m_statTradePW.GetSafeHwnd()))return;

	int iGap = 5;
	int iStatW = 80;
	int iTipsW = 200;
	int iEdtW = 120;
	int iBtnW = 60;
	int iCtrlH = 22;

	CPoint ptCur((rcClt.Width()-iStatW-iBtnW-iEdtW-iGap*5)/2, (rcClt.Height()-iCtrlH)/2);

	m_statTips.MoveWindow((rcClt.Width()-iTipsW)/2,ptCur.y - iCtrlH - 40,iTipsW,iCtrlH);

	m_statTradePW.MoveWindow(ptCur.x,ptCur.y,iStatW,iCtrlH);
	ptCur.Offset(iStatW+iGap*2,0);
	m_edtTradePW.MoveWindow(ptCur.x,ptCur.y,iEdtW,iCtrlH);
	ptCur.Offset(iEdtW+iGap*3,0);
	m_btnOk.MoveWindow(ptCur.x,ptCur.y,iBtnW,iCtrlH);
}
void CDlgLockMain::InitCtrl()
{

}

void CDlgLockMain::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTradePW;
	m_edtTradePW.GetWindowText(strTradePW);
	strTradePW.Trim();
	do 
	{
		if (strTradePW.IsEmpty())
		{
			MessageBox(_T("������Ľ������룡"),_T("ϵͳ��ʾ"));
			break;
		}

		m_edtTradePW.SetWindowText(_T(""));

		char szTradePW[100] = {0};
		memset(szTradePW,0,sizeof(szTradePW));
		int nDestLen;
		CPublic::Instance()->GetTradeDll()->m_pSHA1Encrypt((char*)(LPCTSTR)strTradePW,strTradePW.GetLength(),szTradePW,nDestLen);
		char cTadePW[100] = {0};
		CPublic::Instance()->GetTradeDll()->m_pGetTradePW(cTadePW);
		if (strcmp(cTadePW,szTradePW) == 0)
		{
			::PostMessage(GetParent()->GetSafeHwnd(),WM_NOTIFY_LOCK,0,0);
		}
		else
		{
			MessageBox(_T("��������������"),_T("ϵͳ��ʾ"));
		}
	} while (0);

	m_edtTradePW.SetFocus();
}


HBRUSH CDlgLockMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetSafeHwnd() == m_statTips.GetSafeHwnd())
	{
		pDC->SetTextColor(RGB(255,0,0));
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

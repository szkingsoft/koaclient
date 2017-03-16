// DlgWarnInfoEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgWarnInfoEx.h"
/*#include "afxdialogex.h"*/


// CDlgWarnInfoEx �Ի���

IMPLEMENT_DYNAMIC(CDlgWarnInfoEx, CDialog)

CDlgWarnInfoEx::CDlgWarnInfoEx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWarnInfoEx::IDD, pParent)
	, m_strWarn(_T(""))
{

}

CDlgWarnInfoEx::~CDlgWarnInfoEx()
{
}

void CDlgWarnInfoEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WARNINFO, m_edtWarn);
	DDX_Control(pDX, IDOK, m_btnAgree);
	DDX_Text(pDX, IDC_EDIT_WARNINFO, m_strWarn);
}


BEGIN_MESSAGE_MAP(CDlgWarnInfoEx, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgWarnInfoEx ��Ϣ�������
BOOL CDlgWarnInfoEx::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 200, 200, SWP_NOMOVE);

	InitCtrl();
	
	return TRUE;
}

void CDlgWarnInfoEx::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ReArrayWnd();
}
void CDlgWarnInfoEx::ReArrayWnd()
{
	if (!IsWindow(m_edtWarn.GetSafeHwnd()))return;

	CRect rcClt;
	GetClientRect(&rcClt);

	int iGapW = 30;
	int iGapT = 30;
	int iGapB = 80;
	int ibtnW = 120;
	int iBtnH = 50;
	CRect rcEdt(rcClt.left+iGapW,rcClt.top+iGapT,rcClt.right-iGapW,rcClt.bottom-iGapB);
	m_edtWarn.MoveWindow(&rcEdt);

	CRect rcBtn(rcEdt.right-ibtnW,rcEdt.bottom+15,rcEdt.right,rcEdt.bottom+15+iBtnH);
	m_btnAgree.MoveWindow(&rcBtn);
}

void CDlgWarnInfoEx::InitCtrl()
{
	CFile warnfile;
	CString strWarninfo;
	strWarninfo = CPublic::Instance()->GetSysPath()+_T("\\warninfo.txt");
	if(warnfile.Open(strWarninfo,CFile::modeRead|CFile::typeBinary))
	{
		int nBufferLen = warnfile.GetLength();
		char *pBuffer = new char[nBufferLen+1];
		pBuffer[nBufferLen]=0;
		warnfile.Read(pBuffer,nBufferLen);
		m_strWarn = pBuffer;
		delete []pBuffer;
	}

	UpdateData(FALSE);
}

BOOL CDlgWarnInfoEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

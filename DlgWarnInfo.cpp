// DlgWarnInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgWarnInfo.h"
#include "afxdialogex.h"


// CDlgWarnInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgWarnInfo, CDialog)

CDlgWarnInfo::CDlgWarnInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWarnInfo::IDD, pParent)
	, m_strWarnInfo(_T(""))
{

}

CDlgWarnInfo::~CDlgWarnInfo()
{
}

void CDlgWarnInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WARNINFO, m_strWarnInfo);
	DDX_Control(pDX, IDC_EDIT_WARNINFO, m_WARNINFO);
	DDX_Control(pDX, IDOK, m_btnAgree);
}


BEGIN_MESSAGE_MAP(CDlgWarnInfo, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgWarnInfo ��Ϣ�������


BOOL CDlgWarnInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	CFile warnfile;
	CString strWarninfo;
	strWarninfo = CPublic::Instance()->GetSysPath()+_T("\\warninfo.txt");
	if(warnfile.Open(strWarninfo,CFile::modeRead|CFile::typeBinary))
	{
		int nBufferLen = warnfile.GetLength();
		char *pBuffer = new char[nBufferLen+1];
		pBuffer[nBufferLen]=0;
		warnfile.Read(pBuffer,nBufferLen);
		m_strWarnInfo = pBuffer;
		delete []pBuffer;
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgWarnInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ReArrayWnd();
}
void CDlgWarnInfo::ReArrayWnd()
{
	CRect rcClt;
	GetClientRect(&rcClt);

	int iWith = rcClt.Width();
	if (iWith >600)
	{
	}
}
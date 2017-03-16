// DlgUpdateSelect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgUpdateSelect.h"
#include "afxdialogex.h"


// CDlgUpdateSelect �Ի���

IMPLEMENT_DYNAMIC(CDlgUpdateSelect, CDialog)

CDlgUpdateSelect::CDlgUpdateSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdateSelect::IDD, pParent)
{
	m_pMapInfo = NULL;
}

CDlgUpdateSelect::~CDlgUpdateSelect()
{
}

void CDlgUpdateSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERVERLIST, m_comServer);
}


BEGIN_MESSAGE_MAP(CDlgUpdateSelect, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgUpdateSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgUpdateSelect ��Ϣ�������

BOOL CDlgUpdateSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pMapInfo)
	{
		map<CString,CString>::const_iterator it;
		for(it = m_pMapInfo->begin();it != m_pMapInfo->end();it++)
			m_comServer.AddString(it->first);
	}
	m_comServer.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgUpdateSelect::OnBnClickedOk()
{
	UpdateData();
	m_comServer.GetWindowText(m_strSelectServer);
	CDialog::OnOK();
}



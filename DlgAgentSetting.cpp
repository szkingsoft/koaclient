// DlgAgentSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgAgentSetting.h"
#include "afxdialogex.h"


// CDlgAgentSetting �Ի���

IMPLEMENT_DYNAMIC(CDlgAgentSetting, CDialog)

CDlgAgentSetting::CDlgAgentSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAgentSetting::IDD, pParent)
	, m_nAgentMode(0)
	, m_bUseAgent(FALSE)
{

}

CDlgAgentSetting::~CDlgAgentSetting()
{
}

void CDlgAgentSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USEAGENT, m_bUseAgent);
	DDX_Radio(pDX, IDC_RADIO_SOCK5, m_nAgentMode);
}


BEGIN_MESSAGE_MAP(CDlgAgentSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAgentSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAgentSetting ��Ϣ�������


void CDlgAgentSetting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}

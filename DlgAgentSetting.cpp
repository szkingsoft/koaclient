// DlgAgentSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgAgentSetting.h"
#include "afxdialogex.h"


// CDlgAgentSetting 对话框

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


// CDlgAgentSetting 消息处理程序


void CDlgAgentSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

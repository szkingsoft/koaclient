// DlgUpdateSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgUpdateSelect.h"
#include "afxdialogex.h"


// CDlgUpdateSelect 对话框

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


// CDlgUpdateSelect 消息处理程序

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
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgUpdateSelect::OnBnClickedOk()
{
	UpdateData();
	m_comServer.GetWindowText(m_strSelectServer);
	CDialog::OnOK();
}



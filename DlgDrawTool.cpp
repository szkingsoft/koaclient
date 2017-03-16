// DlgDrawTool.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgDrawTool.h"


// CDlgDrawTool 对话框

IMPLEMENT_DYNAMIC(CDlgDrawTool, CDialog)

CDlgDrawTool::CDlgDrawTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDrawTool::IDD, pParent)
{

}

CDlgDrawTool::~CDlgDrawTool()
{
}

void CDlgDrawTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_DRAWTOOL, m_statDrawTool);
}


BEGIN_MESSAGE_MAP(CDlgDrawTool, CDialog)
	ON_BN_CLICKED(IDC_BTN_LINE, &CDlgDrawTool::OnBnClickedBtnLine)
END_MESSAGE_MAP()


// CDlgDrawTool 消息处理程序

BOOL CDlgDrawTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgDrawTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDrawTool::OnBnClickedBtnLine()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(ID_DRAWLINE_BEELINE, 0),0);
}
void CDlgDrawTool::InitCtrl()
{
	m_statDrawTool.ClearCtrl();
	m_statDrawTool.SetResWnd(AfxGetMainWnd());
	m_statDrawTool.SetTabType(1);
	m_statDrawTool.SetArrayType(1,0x01);
	m_statDrawTool.SetItmeSize(24,24,1);
	m_statDrawTool.SetSelTab(0);

	CString strImgPath = CDateManage::Instance()->GetImgPath();
	strImgPath += _T("DrawTool1.png");

	m_statDrawTool.AddItem(_T(""),ID_DRAWLINE_STOP,strImgPath,7,1);
}

void CDlgDrawTool::ResetSel(int iSel)
{
	m_statDrawTool.SetSelTab(iSel);
}
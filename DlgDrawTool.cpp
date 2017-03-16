// DlgDrawTool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgDrawTool.h"


// CDlgDrawTool �Ի���

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


// CDlgDrawTool ��Ϣ�������

BOOL CDlgDrawTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDlgDrawTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDrawTool::OnBnClickedBtnLine()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
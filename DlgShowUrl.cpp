// DlgShowUrl.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgShowUrl.h"



// CDlgShowUrl 对话框

IMPLEMENT_DYNAMIC(CDlgShowUrl, CDialog)

CDlgShowUrl::CDlgShowUrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowUrl::IDD, pParent)
{

}

CDlgShowUrl::~CDlgShowUrl()
{
}

void CDlgShowUrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BTN_MAX, m_btnMax);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgShowUrl, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_MIN, &CDlgShowUrl::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_MAX, &CDlgShowUrl::OnBnClickedBtnMax)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgShowUrl::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CDlgShowUrl 消息处理程序


void CDlgShowUrl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ReArrayWnd();
}


BOOL CDlgShowUrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgShowUrl::ReArrayWnd()
{
	CRect rcClt;
	GetClientRect(&rcClt);

	int iOffT = 30;
	int iOffLR = 5;
	int iOffB = 5;
	if (!IsWindow(m_dlgHtmlShow.GetSafeHwnd()))return;

	CRect rcHS = rcClt;
	rcHS.InflateRect(-iOffLR,-iOffT,-iOffLR,-iOffB);
	m_dlgHtmlShow.MoveWindow(rcHS);

	int iSysBtnWith = 16;
	int iSysBtnHight = 16;
	int iGapX = 5;
	int iGapY = 6;
	CPoint ptBg;
	ptBg.SetPoint(rcClt.right-iGapX-iSysBtnHight,rcClt.top+iGapY);
	m_btnClose.MoveWindow(ptBg.x,ptBg.y,iSysBtnWith,iSysBtnHight);
	ptBg.Offset(-iSysBtnWith-iGapX,0);
	m_btnMax.MoveWindow(ptBg.x,ptBg.y,iSysBtnWith,iSysBtnHight);
	ptBg.Offset(-iSysBtnWith-iGapX,0);
	m_btnMin.MoveWindow(ptBg.x,ptBg.y,iSysBtnWith,iSysBtnHight);
}
void CDlgShowUrl::InitCtrl()
{
	m_dlgHtmlShow.CreateShowWnd(0x1111,this);
	m_dlgHtmlShow.Navigate(_T("about:blank"), NULL, NULL);
	m_dlgHtmlShow.ShowWindow(SW_SHOW);

	CString strPath,strPathTemp;
	strPathTemp = CPublic::Instance()->GetSysPath();
	strPath = strPathTemp + _T("interface\\sysMin.png");
	m_btnMin.SetMode(0,strPath,4,2);
	m_btnMin.SetFontStyle(12,1,Color::MakeARGB(255,211,211,211),1,3);
	strPath = strPathTemp + _T("interface\\sysMax.png");
	m_btnMax.SetMode(0,strPath,4,2);
	m_btnMax.SetFontStyle(12,1,Color::MakeARGB(255,211,211,211),1,3);
	strPath = strPathTemp + _T("interface\\sysClose.png");
	m_btnClose.SetMode(0,strPath,4,2);
	m_btnClose.SetFontStyle(12,1,Color::MakeARGB(255,211,211,211),1,3);
}
void CDlgShowUrl::NotifyKOASys(UINT iMode)
{
	CWnd *pWnd = CPublic::Instance()->GetView(11,2);
	if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->PostMessage(WM_NOTIFY_KOA_SYS,iMode,2);
	}
}
void CDlgShowUrl::ShowUrl(CString strUrl)
{
	if (IsWindow(m_dlgHtmlShow.GetSafeHwnd())&&!strUrl.IsEmpty())
	{
		m_dlgHtmlShow.Navigate(strUrl);
		Invalidate();
	}
}

void CDlgShowUrl::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	NotifyKOASys(1);
}


void CDlgShowUrl::OnBnClickedBtnMax()
{
	// TODO: 在此添加控件通知处理程序代码
	NotifyKOASys(2);
}


void CDlgShowUrl::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	NotifyKOASys(4);
}

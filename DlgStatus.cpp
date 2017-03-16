// DlgStatus.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgStatus.h"

// CDlgStatus 对话框

IMPLEMENT_DYNAMIC(CDlgStatus, CDialog)

CDlgStatus::CDlgStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStatus::IDD, pParent)
{
	m_strImgLink = _T("");
	m_strImgUnLink = _T("");
	m_TradeLink = FALSE;
	m_QuotationLink = FALSE;
}

CDlgStatus::~CDlgStatus()
{
}

void CDlgStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_CONNECTINFO, m_statConnectInfo);
	DDX_Control(pDX, IDC_STAT_TRADESTATUS, m_statTradeStatus);
	DDX_Control(pDX, IDC_STAT_QUOTATIONSTAUS, m_statQuotationStatus);
	DDX_Control(pDX, IDC_STAT_USERINFO, m_statUserInfo);
}


BEGIN_MESSAGE_MAP(CDlgStatus, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_UPDATE_LINKSTATUS,OnUpdateLinkStatus)
	ON_MESSAGE(WM_UPDATE_USERINFO,OnUpdateUserInfo)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
	ON_MESSAGE(WM_UPDATE_SGRN,OnUpdateUserSGRN)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgStatus 消息处理程序


//void CDlgStatus::PreSubclassWindow()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	CDialog::PreSubclassWindow();
//}


BOOL CDlgStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgStatus::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ReArrayCtrl();
}

void CDlgStatus::InitCtrl()
{
	CSkinAttribute_General *pSkin = (CSkinAttribute_General *)CSkinTransform::Instance()->GetSkinAttribute(7);
	CPublic::Instance()->SetStatusWnd(this);
	m_statConnectInfo.SetTextProperty(pSkin->clrTextNomal,13,0,0,_T("")/*(bLink)?m_strImgLink:m_strImgUnLink*/);
	m_statConnectInfo.SetWindowText(CPublic::Instance()->GetProductName()+ CLanguageInterface::Instance()->GetStr(30001));
	
	TCHAR exeFullPath[MAX_PATH]; // Full path
	CString strPath;
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strPath = exeFullPath;
	int pos = strPath.ReverseFind(('\\'));
	CString strImgDirc = strPath.Left(pos);
	CString strImgPath = strImgDirc +  _T("\\interface\\");

	m_strImgLink = strImgPath + _T("Link.bmp");
	m_strImgUnLink = strImgPath + _T("UnLink.bmp");

	/////
	//CString strUser,strPwd;
	//BOOL bSave,bSaveAccount;
	//BOOL bNeedCFCA = FALSE;
	//BOOL bSupportCFCA = TRUE;
	//CPublic::Instance()->GetLoginInfo(strUser,strPwd,bSave,bSaveAccount,bNeedCFCA,bSupportCFCA);
	//CString strShowUser;
	//strShowUser = CLanguageInterface::Instance()->GetStr(10032)+ strUser;

	//char szUserName[200];
	//memset(szUserName,0,sizeof(szUserName));
	//CPublic::Instance()->GetTradeDll()->m_pGetUserInfo(szUserName,0);
	//CString strUserName;
	//strUserName = CLanguageInterface::Instance()->GetStr(10036);
	//strUserName += szUserName;
	//m_statUserInfo.SetWindowText(strShowUser + _T(" ")+ strUserName);
	m_statUserInfo.SetTextProperty(pSkin->clrTextNomal,13,0,0,_T("")/*,StringAlignmentFar*/);
	//m_statUser.UpDateCtrl();
	OnUpdateUserInfo(0,0);
	/////

	SetTradeStatus(FALSE);
	SetQuotationStatus(FALSE);
	ReArrayCtrl();	
}

void CDlgStatus::SetTradeStatus(BOOL bLink)
{
	//if (bLink == m_TradeLink)return;
	m_TradeLink = bLink;
	m_statTradeStatus.SetTextProperty(Color::MakeARGB(255,99,98,98),13,0,5,_T("")/*(bLink)?m_strImgLink:m_strImgUnLink*/);
	m_statTradeStatus.SetStatus(bLink);
	m_statTradeStatus.UpDateCtrl();
}

void CDlgStatus::SetQuotationStatus(BOOL bLink)
{
	//if (bLink == m_QuotationLink)return;
	m_QuotationLink = bLink;
	m_statQuotationStatus.SetTextProperty(Color::MakeARGB(255,99,98,98),13,0,5,_T("")/*(bLink)?m_strImgLink:m_strImgUnLink*/);
	m_statQuotationStatus.SetStatus(bLink);
	m_statQuotationStatus.UpDateCtrl();
}

void CDlgStatus::ReArrayCtrl()
{
	if (!IsWindow(m_statConnectInfo.GetSafeHwnd()))return;
	CRect rcClt;
	GetClientRect(&rcClt);
	int iGapT = 8;
	int iGapL = 16;
	int iGapR = 5;
	int iStatHight = 20;
	int istatConnectInfoWith = 300;
	int iStatLoginWith = 90;
	CPoint pt(rcClt.left,rcClt.top);

	pt.Offset(iGapL,iGapT);
	m_statConnectInfo.MoveWindow(pt.x,pt.y,istatConnectInfoWith,iStatHight);
	int iUsLeft = pt.x+istatConnectInfoWith;
	pt.SetPoint(rcClt.right-iGapR-iStatLoginWith,rcClt.top+iGapT);
	m_statTradeStatus.MoveWindow(pt.x,pt.y,iStatLoginWith,iStatHight);
	pt.Offset(-iStatLoginWith,0);
	m_statQuotationStatus.MoveWindow(pt.x,pt.y,iStatLoginWith,iStatHight);

	m_statUserInfo.MoveWindow(iUsLeft,pt.y,pt.x-iUsLeft,iStatHight);
}

BOOL CDlgStatus::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	//Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	//Graphics graphicsDraw(pDC->m_hDC);
	//Graphics graphics(&cachBmp);
	
	CDateManage::Instance()->DrawBkRect(pDC/*graphics*/,rcClt);

	//CSkinAttribute_FrameLine * pSkinFl = (CSkinAttribute_FrameLine *)CSkinTransform::Instance()->GetSkinAttribute(3);
	//Pen penBrd(pSkinFl->clrBkLineOut);
	////graphics.DrawRectangle(&penBrd,Rect(rcClt.left,rcClt.top,rcClt.Width()-1,rcClt.Height()-1));
	//Pen penBrd2(pSkinFl->clrBkLineIn);
	////graphics.DrawRectangle(&penBrd2,Rect(rcClt.left+1,rcClt.top+1,rcClt.Width()-3,rcClt.Height()-3));

	//graphicsDraw.DrawImage(&cachBmp,0,0/*rcClt.top*/);
	//graphicsDraw.ReleaseHDC(pDC->m_hDC);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CDlgStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//if ( nCtlColor == CTLCOLOR_STATIC )
	//{
	//	pDC->SetTextColor(RGB(255,255,255));
	//	pDC->SetBkColor(TRANSPARENT);
	//	pDC->SetBkMode(TRANSPARENT);//透明 

	//	//return CDialogMultiLan::OnCtlColor(pDC, pWnd, nCtlColor);
	//	return HBRUSH(GetStockObject(HOLLOW_BRUSH));//hbr;
	//}
	//else
	{
		//return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}	
	//return CDialogMultiLan::OnCtlColor(pDC, pWnd, nCtlColor);
	return NULL;
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  在此更改 DC 的任何特性

	//// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
}

LRESULT CDlgStatus::OnUpdateLinkStatus(WPARAM Wpar,LPARAM lPar)
{
	if (Wpar == 3)
	{
		SetTradeStatus(lPar);
	}
	else if (Wpar == 1)
	{
		SetQuotationStatus(lPar);
	}
	return TRUE;
}

LRESULT CDlgStatus::OnUpdateUserSGRN(WPARAM Wpar,LPARAM lPar)
{
	int iRet = 1;
	OnUpdateUserInfo(Wpar,lPar);
	return iRet;
}

LRESULT CDlgStatus::OnUpdateUserInfo(WPARAM Wpar,LPARAM lPar)
{
	int iRet = 1;

	CString strUser,strPwd;
	BOOL bSave,bSaveAccount;
	BOOL bNeedCFCA = FALSE;
	BOOL bSupportCFCA = TRUE;
	CPublic::Instance()->GetLoginInfo(strUser,strPwd,bSave,bSaveAccount,bNeedCFCA,bSupportCFCA);
	CString strShowUser;
	strShowUser = CLanguageInterface::Instance()->GetStr(10032)+ strUser;

	char szUserName[200];
	memset(szUserName,0,sizeof(szUserName));
	CPublic::Instance()->GetTradeDll()->m_pGetUserInfo(szUserName,0);
	CString strUserName;
	strUserName = CLanguageInterface::Instance()->GetStr(10036);
	strUserName += szUserName;

	CString strUserSGRN;
	char szUserSGRN[300];
	memset(szUserSGRN,0,sizeof(szUserSGRN));
	CPublic::Instance()->GetTradeDll()->m_pGetUserInfo(szUserSGRN,1);
	
	if (strlen(szUserSGRN) > 0)
	{
		strUserSGRN = CLanguageInterface::Instance()->GetStr(10042);
		strUserSGRN += szUserSGRN;
	}
	
	m_statUserInfo.SetWindowText(strShowUser + _T(" ") + strUserName + _T(" ") + strUserSGRN);
	m_statUserInfo.UpDateCtrl(0);
	return iRet;
}
LRESULT CDlgStatus::OnSkinChange(WPARAM Wpar,LPARAM lPar)
{
	Invalidate();
	CSkinAttribute_General *pSkin = (CSkinAttribute_General *)CSkinTransform::Instance()->GetSkinAttribute(7);
	m_statConnectInfo.SetTextProperty(pSkin->clrTextNomal,13,0,0,_T("")/*(bLink)?m_strImgLink:m_strImgUnLink*/);
	m_statUserInfo.SetTextProperty(pSkin->clrTextNomal,13,0,0,_T("")/*,StringAlignmentFar*/);
	
	m_statConnectInfo.UpDateCtrl();
	m_statTradeStatus.UpDateCtrl();
	m_statQuotationStatus.UpDateCtrl();
	m_statUserInfo.UpDateCtrl();
	return 1;
}

void CDlgStatus::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

// DlgTopToolbar.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgTopToolbar.h"
#include "afxdialogex.h"

// CDlgTopToolbar 对话框

IMPLEMENT_DYNAMIC(CDlgTopToolbar, CDialog)

CDlgTopToolbar::CDlgTopToolbar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTopToolbar::IDD, pParent)
{
	m_pImgBk = NULL;
}

CDlgTopToolbar::~CDlgTopToolbar()
{
	if (m_pImgBk)
	{
		delete m_pImgBk;
		m_pImgBk = NULL;
	}
}

void CDlgTopToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CALCULATOR, m_BtnCalculator);
	DDX_Control(pDX, IDC_BTN_DELIVERYREPORT, m_btnDeliveryReport);
	DDX_Control(pDX, IDC_BTN_APPLYGOLD, m_btnApplyGold);
	DDX_Control(pDX, IDC_BTN_APPLYGOLDEN, m_btnApplyGolden);
	DDX_Control(pDX, IDC_BTN_MODIFYPW, m_btnModifyPW);
	DDX_Control(pDX, IDC_BTN_BOSSKEY, m_btnBossKey);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_FULLSCREEN, m_btnFullScreen);
	DDX_Control(pDX, IDC_BTN_USER, m_btnUser);
	DDX_Control(pDX, IDC_STAT_USER, m_statUser);
	DDX_Control(pDX, IDC_BTN_MALL, m_btnMall);
}


BEGIN_MESSAGE_MAP(CDlgTopToolbar, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(IDC_BTN_CALCULATOR, &CDlgTopToolbar::OnBnClickedBtnCalculator)
	ON_CONTROL_RANGE(BN_CLICKED,ID_SYS_BEGIN,ID_SYS_END,OnBnClickedBtnRange)
	ON_MESSAGE(WM_UPDATE_USERINFO,OnUpdateUserInfo)
	ON_MESSAGE(WM_UPDATE_SGRN,OnUpdateUserSGRN)
	ON_BN_CLICKED(IDC_BTN_DELIVERYREPORT, &CDlgTopToolbar::OnBnClickedBtnDeliveryreport)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
	ON_BN_CLICKED(IDC_BTN_MALL, &CDlgTopToolbar::OnBnClickedBtnMall)
END_MESSAGE_MAP()


// CDlgTopToolbar 消息处理程序


BOOL CDlgTopToolbar::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	IntiCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTopToolbar::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	ArrayCtrl();
}

void CDlgTopToolbar::IntiCtrl()
{
	CPublic::Instance()->SetToolBarWnd(this);

	TCHAR exeFullPath[MAX_PATH]; // Full path
	CString strPath;
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strPath = exeFullPath;
	int pos = strPath.ReverseFind(('\\'));
	CString strImgDirc = strPath.Left(pos);
	CString strImgPath = strImgDirc +  _T("\\interface\\");
	USES_CONVERSION;

	m_BtnCalculator.SetMode(5,(strImgPath + _T("01.png")),1);
	m_BtnCalculator.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnDeliveryReport.SetMode(5,(strImgPath + _T("02.png")),1);
	m_btnDeliveryReport.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnApplyGold.SetMode(5,(strImgPath + _T("03.png")),1);
	m_btnApplyGold.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnApplyGolden.SetMode(5,(strImgPath + _T("04.png")),1);
	m_btnApplyGolden.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnModifyPW.SetMode(5,(strImgPath + _T("05.png")),1);
	m_btnModifyPW.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnBossKey.SetMode(5,(strImgPath + _T("06.png")),1);
	m_btnBossKey.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnRefresh.SetMode(5,(strImgPath + _T("07.png")),1);
	m_btnRefresh.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));
	m_btnFullScreen.SetMode(5,(strImgPath + _T("08.png")),1);
	m_btnFullScreen.SetFontStyle(15,FontStyleRegular,Color::MakeARGB(255,245,245,245));

	m_btnMall.SetMode(5,(strImgPath + _T("09.png")),1);
	if (!(CPublic::Instance()->GetMallName()).IsEmpty())
	{
		m_btnMall.SetWindowText(CPublic::Instance()->GetMallName());
	}
	m_btnMall.SetWetherUseSignColor(true);
//	m_btnMall.SetFontStyle(15,FontStyleBold,Color::MakeARGB(255,253,218,89));
	m_btnMall.SetFontStyle(15,FontStyleBold,CPublic::Instance()->GetMallFontColor().GetValue());
	m_btnUser.SetMode(5,_T(""),1);
	m_btnUser.SetFontStyle(13,FontStyleRegular,Color::MakeARGB(255,88,88,245));
	///////
	//CString strUser,strPwd;
	//BOOL bSave,bSaveAccount;
	//BOOL bNeedCFCA = FALSE;
	//BOOL bSupportCFCA = TRUE;
	//CPublic::Instance()->GetLoginInfo(strUser,strPwd,bSave,bSaveAccount,bNeedCFCA,bSupportCFCA);
	//CString strShowUser;
	//strShowUser = CLanguageInterface::Instance()->GetStr(10032)+ strUser;

	//char szUserName[100];
	//memset(szUserName,0,sizeof(szUserName));
	//CPublic::Instance()->GetTradeDll()->m_pGetUserName(szUserName);
	//CString strUserName;
	//strUserName = CLanguageInterface::Instance()->GetStr(10036);
	//strUserName += szUserName;
	//m_btnUser.SetWindowText(strShowUser + _T(" ") + strUserName);
	//m_statUser.SetWindowText(strShowUser + _T(" ")+ strUserName);
	///////
	//m_btnDeliveryReport.ShowWindow(SW_HIDE);
	m_pImgBk = Image::FromFile(T2W(strImgPath + _T("TitleRight1.png")));

	//ArrayCtrl();
	SetShowMode();
}

void CDlgTopToolbar::SetShowMode(int iMode)
{
	if (iMode == 0)
	{
		m_btnDeliveryReport.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnDeliveryReport.ShowWindow(SW_SHOW);
	}
	ArrayCtrl();
}

void CDlgTopToolbar::CalculateWndStatus(CWnd *pWnd,BOOL iShow)
{
	if (iShow)
	{
		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}

void CDlgTopToolbar::ArrayCtrl()
{
	if (!IsWindow(m_btnUser.GetSafeHwnd()))return;

	CRect rcClt;
	GetClientRect(&rcClt);
	
	int iUserBtnWith = 300;
	int iEveryBtnWiht = 100;
	int iEveryBtnHight = 36;
	int iSpace = 2;
	CPoint pt(0,0);
	pt.Offset(iSpace,iSpace+5);

	int iUserBtnLeft = rcClt.right-iUserBtnWith-1;
	m_btnUser.MoveWindow(iUserBtnLeft,pt.y,iUserBtnWith,iEveryBtnHight);
	m_btnUser.ShowWindow(SW_HIDE);
	m_statUser.MoveWindow(iUserBtnLeft,pt.y,iUserBtnWith,iEveryBtnHight);
	m_statUser.SetTextProperty(Color::MakeARGB(255,99,98,255),12,0,0,_T("")/*(bLink)?m_strImgLink:m_strImgUnLink*/);
	m_statUser.UpDateCtrl();

	m_BtnCalculator.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_BtnCalculator,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	pt.Offset(iSpace+iEveryBtnWiht,0);
	if (m_btnDeliveryReport.IsWindowVisible())
	{
		m_btnDeliveryReport.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
		CalculateWndStatus(&m_btnDeliveryReport,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
		pt.Offset(iSpace+iEveryBtnWiht,0);
	}	
	m_btnApplyGold.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_btnApplyGold,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	pt.Offset(iSpace+iEveryBtnWiht,0);
	m_btnApplyGolden.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_btnApplyGolden,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	pt.Offset(iSpace+iEveryBtnWiht,0);
	m_btnModifyPW.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_btnModifyPW,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	pt.Offset(iSpace+iEveryBtnWiht,0);
	m_btnBossKey.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_btnBossKey,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	pt.Offset(iSpace+iEveryBtnWiht,0);
	m_btnRefresh.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_btnRefresh,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	pt.Offset(iSpace+iEveryBtnWiht,0);
	m_btnFullScreen.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
	CalculateWndStatus(&m_btnFullScreen,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);

	if (CPublic::Instance()->m_bIsShowMall)
	{
		pt.Offset(iSpace+iEveryBtnWiht,0);
		m_btnMall.MoveWindow(pt.x,pt.y,iEveryBtnWiht,iEveryBtnHight);
		CalculateWndStatus(&m_btnMall,((pt.x+iEveryBtnWiht)-iUserBtnLeft)>0?FALSE:TRUE);
	}
	else m_btnMall.ShowWindow(SW_HIDE);
}

BOOL CDlgTopToolbar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    CRect rcClt;
	GetClientRect(&rcClt);
	CRect rcBottomBorde(rcClt.left,rcClt.top,rcClt.right,rcClt.bottom);
	Color clrTop(255,227,239,255);
	Color clrMid(255,191,219,255);
	Color clrBottom(255,227,239,255);

	//Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	//Graphics graphicsDraw(pDC->m_hDC);
	//Graphics gpDraw(&cachBmp);
	///////////////////////
	HDC hMemDc = CreateCompatibleDC(pDC->m_hDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(pDC->m_hDC, rcClt.Width(), rcClt.Height());
	SelectObject(hMemDc,hMemBitmap);
	Graphics gpDraw(hMemDc);
	//////////////////////////////
	//Gdiplus::LinearGradientBrush   linGrBrush(Rect(rcBottomBorde.left,rcBottomBorde.top,rcBottomBorde.Width(),
	//	rcBottomBorde.Height()/2),clrTop,clrMid,(Gdiplus::REAL)(90));
	//Gdiplus::LinearGradientBrush   linGrBrush2(Rect(rcBottomBorde.left,rcBottomBorde.top+rcBottomBorde.Height()/2-1,rcBottomBorde.Width(),
	//	rcBottomBorde.Height()/2),clrMid,clrBottom,(Gdiplus::REAL)(90));
	//gpDraw.FillRectangle(&linGrBrush,Rect(rcBottomBorde.left,rcBottomBorde.top,rcBottomBorde.Width(),rcBottomBorde.Height()/2));
	//gpDraw.FillRectangle(&linGrBrush2,Rect(rcBottomBorde.left,rcBottomBorde.top+rcBottomBorde.Height()/2,rcBottomBorde.Width(),rcBottomBorde.Height()/2));
	CSkinAttribute_General *pSkin = (CSkinAttribute_General*)CSkinTransform::Instance()->GetSkinAttribute(8);
	SolidBrush sBk(pSkin->clrBkAll/*Color(255,21,21,21)*/);
	gpDraw.FillRectangle(&sBk,Rect(rcClt.left,rcClt.top,rcClt.Width(),rcClt.Height()));
	if (m_pImgBk)gpDraw.DrawImage(m_pImgBk,Rect(rcClt.right-m_pImgBk->GetWidth()+4,rcClt.top+1,m_pImgBk->GetWidth(),m_pImgBk->GetHeight()));
	Pen penBorde1(pSkin->clrAllFrameOut);
	Pen penBorde2(pSkin->clrAllFrameIn);
	gpDraw.DrawLine(&penBorde1,Point(rcClt.left,rcClt.top),Point(rcClt.right,rcClt.top));
	gpDraw.DrawLine(&penBorde2,Point(rcClt.left,rcClt.top+1),Point(rcClt.right,rcClt.top+1));

	::BitBlt(pDC->m_hDC,0,0,rcClt.Width(),rcClt.Height(),
		hMemDc,0,0,SRCCOPY);
	DeleteObject(hMemBitmap);
	DeleteDC(hMemDc);
	//graphicsDraw.DrawImage(&cachBmp,0,0/*rcClt.top*/);
	//graphicsDraw.ReleaseHDC(pDC->m_hDC);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}



BOOL CDlgTopToolbar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	//if (WM_NCPAINT == pMsg->wParam)
	//{
	//	return 1;
	//}
	return CDialog::PreTranslateMessage(pMsg);
}


//void CDlgTopToolbar::OnBnClickedBtnCalculator()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

void CDlgTopToolbar::OnBnClickedBtnRange(UINT iId)
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(iId, 0),0);
}

LRESULT CDlgTopToolbar::OnUpdateUserSGRN(WPARAM Wpar,LPARAM lPar)
{
	int iRet = 1;


	return iRet;
}

LRESULT CDlgTopToolbar::OnUpdateUserInfo(WPARAM Wpar,LPARAM lPar)
{
	int iRet = 1;

	CString strUser,strPwd;
	BOOL bSave,bSaveAccount;
	BOOL bNeedCFCA = FALSE;
	BOOL bSupportCFCA = TRUE;
	CPublic::Instance()->GetLoginInfo(strUser,strPwd,bSave,bSaveAccount,bNeedCFCA,bSupportCFCA);
	CString strShowUser;
	strShowUser = CLanguageInterface::Instance()->GetStr(10032)+ strUser;

	char szUserName[100];
	memset(szUserName,0,sizeof(szUserName));
	CPublic::Instance()->GetTradeDll()->m_pGetUserInfo(szUserName,0);
	CString strUserName;
	strUserName = CLanguageInterface::Instance()->GetStr(10036);
	strUserName += szUserName;
	m_btnUser.SetWindowText(strShowUser + _T(" ") + strUserName);
	m_statUser.SetWindowText(strShowUser + _T(" ")+ strUserName);
	m_statUser.UpDateCtrl(1);
	return iRet;
}

void CDlgTopToolbar::OnBnClickedBtnDeliveryreport()
{
	// TODO: 在此添加控件通知处理程序代码
	CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(WM_NOTIFY_DELIVERY,1,0);
}
LRESULT CDlgTopToolbar::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	m_BtnCalculator.Invalidate();
	m_btnDeliveryReport.Invalidate();
	m_btnApplyGold.Invalidate();
	m_btnApplyGolden.Invalidate();
	m_btnModifyPW.Invalidate();
	m_btnBossKey.Invalidate();
	m_btnRefresh.Invalidate();
	m_btnFullScreen.Invalidate();
	m_btnMall.Invalidate();
	m_btnUser.Invalidate();
    m_statUser.Invalidate();
	return 1;
}

void CDlgTopToolbar::OnBnClickedBtnMall()
{
    CDateManage::Instance()->ExecuteUrl(CPublic::Instance()->m_strMallUrl);
}

void CDlgTopToolbar::UpdateButtonStatus()
{
	if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
	{
		char cChannelCode[10] = {0};
		int iRet = CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus(cChannelCode);

		if (cChannelCode[0] == '4' || cChannelCode[0] == 'C'
			|| cChannelCode[0] == '9' || cChannelCode[0] == 'A'
			|| cChannelCode[0] == 'E' || cChannelCode[0] == 'G'
			|| cChannelCode[0] == '1' || cChannelCode[0] == 'B'
			|| cChannelCode[0] == '7'|| cChannelCode[0] == '8')
			m_btnApplyGold.EnableWindow(TRUE);
		else
			m_btnApplyGold.EnableWindow(FALSE);

		if (cChannelCode[0] == '4' || cChannelCode[0] == 'C'
			|| cChannelCode[0] == '9' || cChannelCode[0] == 'A'
			|| cChannelCode[0] == 'E' || cChannelCode[0] == 'G'
			|| cChannelCode[0] == '1' || cChannelCode[0] == 'B'
			|| cChannelCode[0] == '7'|| cChannelCode[0] == '8')
			m_btnApplyGolden.EnableWindow(TRUE);
		else
			m_btnApplyGolden.EnableWindow(FALSE);
	}
	else
	{
		m_btnApplyGold.EnableWindow(FALSE);
		m_btnApplyGolden.EnableWindow(FALSE);
	}
}
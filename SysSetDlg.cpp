#include "stdafx.h"
#include "KOAClient.h"
#include "SysSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNCREATE(CSysSetDlg, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Construction/Destruction /////////////////////////////////////////

CSysSetDlg::CSysSetDlg() : CPropertyPage(CSysSetDlg::IDD)
{
	//{{AFX_DATA_INIT(CSysSetDlg)
	//}}AFX_DATA_INIT
	m_bActive = FALSE;
}

CSysSetDlg::~CSysSetDlg()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Overrides ///////////////////////////////////////////////////////////////////

////**** public

////**** protected
BOOL CSysSetDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CLanguageInterface::Instance()->InitDialog(IDD,this);
	int nNum = CInterfaceManager::Instance()->GetSysStyleNum();
	CString strName;
	for(int i = 0;i<nNum;i++)
	{
		CInterfaceManager::Instance()->GetSysStyleName(i,strName);
		m_cmbSysStyle.AddString(strName);
		m_cmbSysStyle.SetItemData(i,i);
	}
	int nSel = CInterfaceManager::Instance()->GetSysStyle();
	m_cmbSysStyle.SetCurSel(nSel);
	
	
	//颜色设置相关
	m_lbSysColorName.ResetContent();
	const vector<TColorDefInfo>& vecSrcSysColor = CInterfaceManager::Instance()->GetSysColorList();
	vector<TColorDefInfo>::const_iterator iterSrcColor;
	for( iterSrcColor = vecSrcSysColor.begin();
	iterSrcColor != vecSrcSysColor.end();
	iterSrcColor++)
	{
		int nCurIndex = m_lbSysColorName.AddString(CString(iterSrcColor->szName));
		m_lbSysColorName.SetItemData(nCurIndex,iterSrcColor->uiID);
	}	
	m_lbSysColorName.SetCurSel(0);
	
	m_slidR.SetBuddy(GetDlgItem(IDC_EDIT_R),FALSE);
	m_slidR.SetRange(0,255,TRUE);
	m_slidB.SetBuddy(GetDlgItem(IDC_EDIT_B),FALSE);
	m_slidB.SetRange(0,255,TRUE);	
	m_slidG.SetBuddy(GetDlgItem(IDC_EDIT_G),FALSE);
	m_slidG.SetRange(0,255,TRUE);
	OnListSelchangeColorName();
	
	// 字体设置相关
	m_cmbSysFontName.ResetContent();
	const vector<TFontDefInfo>& vecSrcFontColor = CInterfaceManager::Instance()->GetSysFontList();
	vector<TFontDefInfo>::const_iterator iterSrcFont;
	for( iterSrcFont = vecSrcFontColor.begin();
	iterSrcFont != vecSrcFontColor.end();
	iterSrcFont++)
	{
		int nCurIndex = m_cmbSysFontName.AddString(CString(iterSrcFont->szName));
		m_cmbSysFontName.SetItemData(nCurIndex,iterSrcFont->uiID);
	}
	m_cmbSysFontName.SetCurSel(0);
	return TRUE;
}

void CSysSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSetDlg)
	DDX_Control(pDX, IDC_COLORSCHEME_COMBO, m_cmbSysStyle);
	DDX_Control(pDX, IDC_BUTTON_CURCOLOR, m_btnCurColor);
	DDX_Control(pDX, IDC_ITEMTYPE_LIST, m_lbSysColorName);
	DDX_Control(pDX, IDC_COLORDEF_STATIC, m_DrawPic);
	DDX_Control(pDX, IDC_SLIDER_R,m_slidR);
	DDX_Control(pDX, IDC_SLIDER_G,m_slidG);
	DDX_Control(pDX, IDC_SLIDER_B,m_slidB);
	DDX_Text(pDX, IDC_EDIT_R, m_nR);
	DDV_MinMaxInt(pDX,m_nR,0,255);
	DDX_Text(pDX, IDC_EDIT_G, m_nG);
	DDV_MinMaxInt(pDX,m_nG,0,255);
	DDX_Text(pDX, IDC_EDIT_B, m_nB);
	DDV_MinMaxInt(pDX,m_nB,0,255);
	
	DDX_Control(pDX, IDC_COMBO_FONTNAME,m_cmbSysFontName);
	
	//}}AFX_DATA_MAP
}

////**** private

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Messages //////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSysSetDlg, CPropertyPage)
//{{AFX_MSG_MAP(CSysSetDlg)
ON_WM_CTLCOLOR()

ON_CBN_SELCHANGE(IDC_COLORSCHEME_COMBO, OnCmbSelchangeStyle)

ON_LBN_SELCHANGE(IDC_ITEMTYPE_LIST,OnListSelchangeColorName)
ON_EN_CHANGE(IDC_EDIT_R, OnUpdateR)
ON_EN_CHANGE(IDC_EDIT_G, OnUpdateG)
ON_EN_CHANGE(IDC_EDIT_B, OnUpdateB)
ON_BN_CLICKED(IDC_BTN_FONT_DEFAULT, OnBtnDefaultFont)
ON_BN_CLICKED(IDC_BTN_SELFONT, OnBtnSelFont)

ON_BN_CLICKED(IDC_BTN_SELCOLOR, OnBtnSelColor)
ON_BN_CLICKED(IDC_BTN_COLOR_DEFAULT, OnBtnDefaultColor)
//}}AFX_MSG_MAP
//ON_BN_CLICKED(IDC_BUTTON_CURCOLOR, &CSysSetDlg::OnBnClickedButtonCurcolor)
END_MESSAGE_MAP()

////**** private
void CSysSetDlg::OnListSelchangeColorName()
{
	int nCurSel = m_lbSysColorName.GetCurSel();
	if( nCurSel == LB_ERR)
		return;

	COLORREF crCurSelItemColor = m_DrawPic.GetTempSysColor(m_lbSysColorName.GetItemData(nCurSel));
	
	// 显示根据当前选择项使用的颜色信息
    m_nR = GetRValue(crCurSelItemColor);
	m_slidR.SetPos(m_nR);
	m_nG = GetGValue(crCurSelItemColor);
	m_slidG.SetPos(m_nG);
	m_nB = GetBValue(crCurSelItemColor);
	m_slidB.SetPos(m_nB);
	
    DisplayCurColor();
	
	UpdateData(FALSE);
}

void CSysSetDlg::OnUpdateR()
{
	UpdateData(TRUE);
	m_slidR.SetPos(m_nR);
	DisplayCurColor();
	UpdateCurItemTempSysColor(RGB(m_nR,m_nG,m_nB));
}

void CSysSetDlg::OnUpdateG()
{
	UpdateData(TRUE);
	m_slidG.SetPos(m_nG);
	DisplayCurColor();
	UpdateCurItemTempSysColor(RGB(m_nR,m_nG,m_nB));
}

void CSysSetDlg::OnUpdateB()
{
	UpdateData(TRUE);
	m_slidB.SetPos(m_nB);
    DisplayCurColor();
	UpdateCurItemTempSysColor(RGB(m_nR,m_nG,m_nB));
}

void CSysSetDlg::OnBtnSelColor()
{
	CColorDialog dlgColor(RGB(m_nR, m_nG, m_nB), CC_FULLOPEN);
	int iRet = dlgColor.DoModal();
	if(iRet == IDOK)
	{
		m_nR = GetRValue(dlgColor.GetColor());
		m_nG = GetGValue(dlgColor.GetColor());
		m_nB = GetBValue(dlgColor.GetColor());
		DisplayCurColor();
		UpdateCurItemTempSysColor(dlgColor.GetColor());
	}
}

void CSysSetDlg::OnBtnDefaultColor()
{
	CString szText;
	szText = _T("您确定撤销刚才对颜色的修改吗?");
	
	if(IDYES == MessageBox(szText,NULL,MB_YESNO|MB_ICONQUESTION))
	{
		m_DrawPic.ResetTempSysColor();	
		OnListSelchangeColorName();	
	}
}

void CSysSetDlg::OnCmbSelchangeStyle()
{
	int nSel = m_cmbSysStyle.GetCurSel();
	int nID;
	nID = m_cmbSysStyle.GetItemData(nSel);
	CString strFileName;
	CInterfaceManager::Instance()->GetSysStyleFileName(nID,strFileName);

	CInterfaceManager::Instance()->ReloadSyStyle(nSel,strFileName);

	
	//重画所有视图
	CWnd* pView;
	for(int i = 1;i<=4;i++)
	{
		pView = CPublic::Instance()->GetView(i);
		if(pView)pView->PostMessage(WM_REDRAWWND,0,0);
	}



	m_DrawPic.ResetTempSysColor();
	m_DrawPic.ResetTempSysFont();
	m_DrawPic.RedrawWindow();

}

HBRUSH CSysSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nItem = pWnd->GetDlgCtrlID();
	return hbr;
}

void CSysSetDlg::OnBtnSelFont()
{
	int nCurSel = m_cmbSysFontName.GetCurSel();
	if( nCurSel == LB_ERR)
		return;

	//获取所需字体
	CFont* pFnt = m_DrawPic.GetTempSysFont(m_cmbSysFontName.GetItemData(nCurSel)).pFont;
	if(NULL == pFnt) 
		return;

	LOGFONT tLogFont;
	pFnt->GetLogFont(&tLogFont);
	CFontDialog dlgFont(&tLogFont);
	int iRet = dlgFont.DoModal();
	if(IDOK == iRet)
	{
		dlgFont.GetCurrentFont(&tLogFont);
		TFontDefInfo tUpdateFont;
		
		tUpdateFont.uiID= m_cmbSysFontName.GetItemData(nCurSel);
		tUpdateFont.nSize = abs(tLogFont.lfHeight);
		if(dlgFont.IsBold()) //粗体
			tUpdateFont.nStyle = FONT_STYLE_BOLD;
		else if(dlgFont.IsItalic()) //斜体
			tUpdateFont.nStyle = FONT_STYLE_ITALIC;
		else if(dlgFont.IsUnderline()) //下划线
			tUpdateFont.nStyle = FONT_STYLE_UNDERLINE;
		else 
			tUpdateFont.nStyle = 0;
		_tcscpy(tUpdateFont.szFontName,(TCHAR*)(LPCTSTR)dlgFont.GetFaceName());

		m_DrawPic.UpdateTempSysFont(tUpdateFont);		
	}
}

//字体恢复
void CSysSetDlg::OnBtnDefaultFont()
{
	CString szText;
	szText = _T("您确定撤销刚才对字体的修改吗?");

	if(IDYES == MessageBox(szText,NULL,MB_YESNO|MB_ICONQUESTION))
	{
		m_DrawPic.ResetTempSysFont();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  Implementation //////////////////////////////////////////////////

////**** public

BOOL CSysSetDlg::SaveInterfaceParam()
{
	BOOL bRet = m_DrawPic.SaveTemp2SysColor();

	bRet |= m_DrawPic.SaveTemp2SysFont();
	return bRet;	
}

////**** private

void CSysSetDlg::DisplayCurColor()
{
	m_btnCurColor.SetBtnBackColor(RGB(m_nR,m_nG,m_nB));
	m_btnCurColor.Invalidate();
}

void CSysSetDlg::UpdateCurItemTempSysColor(COLORREF crColor)
{
	int nCurSel = m_lbSysColorName.GetCurSel();
	if( nCurSel == LB_ERR)
		return;

	TColorDefInfo tTempColor;
	tTempColor.uiID = m_lbSysColorName.GetItemData(nCurSel);
	tTempColor.crColor = crColor;
	
	m_DrawPic.UpdateTempSysColor(tTempColor);
}

BOOL CSysSetDlg::IsSetActived()
{
	return m_bActive;
}

BOOL CSysSetDlg::OnSetActive() 
{
	m_bActive = TRUE;
	return CPropertyPage::OnSetActive();
}

//void CSysSetDlg::OnBnClickedButtonCurcolor()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

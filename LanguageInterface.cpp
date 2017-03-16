#include "StdAfx.h"
#include "LanguageInterface.h"
#include "IniFile.h"
#include "resource.h"
CLanguageInterface* CLanguageInterface::_instance = NULL;
void CLanguageInterface::ExitInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

CLanguageInterface* CLanguageInterface::Instance()
{
	if(!_instance)
	{
		_instance = new CLanguageInterface;
		_instance->LoadLanauage();
	}
	return _instance;
}

CLanguageInterface::CLanguageInterface(void)
{
}


CLanguageInterface::~CLanguageInterface(void)
{
}

CString CLanguageInterface::GetStr(unsigned int uiResourceID,int iMode)
{
	CString strTemp = _T("");
	if (iMode == 0)
	{
		map<int,CString>::iterator it;
		it = m_mapLanguage.find(uiResourceID);
		if(it == m_mapLanguage.end())return _T("");
		strTemp = it->second;
	}
	else if (iMode == 1)
	{
		map<int,CString>::iterator it;
		it = m_mapTrade.find(uiResourceID);
		if(it == m_mapTrade.end())return _T("");
		strTemp =  it->second;
	}

	return strTemp;
}

CString CLanguageInterface::GetMenuStr(unsigned int uiResourceID)
{
	map<int,CString>::iterator it;
	it = m_mapMenuLanguage.find(uiResourceID);
	if(it == m_mapMenuLanguage.end())return _T("");
	return it->second;
}


BOOL CLanguageInterface::LoadLanauage()
{
	CString strLanguagePath;
	strLanguagePath = CPublic::Instance()->GetSysPath();
	strLanguagePath +=_T("\\language.dat");
	CIniFile inifile;
	if(!inifile.Create(strLanguagePath))
	{
		AfxMessageBox(_T("Open Languager data failed"));
		return FALSE;	
	}

	m_mapLanguage.clear();
	//////////////////////////////////////////////////////////////////////////
	//读取其他文字信息
	map<CString,CString> mapVar;
	inifile.GetSection(_T("MAIN"),mapVar);

	map<CString,CString>::iterator mapit;
	int nKey;
	for(mapit = mapVar.begin();mapit != mapVar.end();mapit++)
	{
		nKey = atoi((char*)(LPCTSTR)mapit->first);
		m_mapLanguage[nKey]= mapit->second;
	}
	//////////////////////////////////////////////////////////////////////////
	//读取菜单文字信息
	mapVar.clear();
	inifile.GetSection(_T("MENU"),mapVar);
	for(mapit = mapVar.begin();mapit != mapVar.end();mapit++)
	{
		nKey = atoi((char*)(LPCTSTR)mapit->first);
		m_mapMenuLanguage[nKey]= mapit->second;
	}

	mapVar.clear();
	m_mapTrade.clear();
	inifile.GetSection(_T("TRADE"),mapVar);
	for(mapit = mapVar.begin();mapit != mapVar.end();mapit++)
	{
		nKey = atoi((char*)(LPCTSTR)mapit->first);
		m_mapTrade[nKey]= mapit->second;
	}	
	return TRUE;
}

BOOL __stdcall SetChildFont(HWND hwnd, LPARAM lparam)
{
	CFont *pFont = (CFont*)lparam;
	CWnd *pWnd = CWnd::FromHandle(hwnd);
	pWnd->SetFont(pFont);
	return TRUE;
}

void   CLanguageInterface::InitDialog(UINT uiDlgID,CDialog* pDlg)
{
	if(NULL == pDlg)return;
	switch(uiDlgID)
	{
	case IDD_ABOUTBOX:
		{
			pDlg->GetDlgItem(IDC_ABOUT_COPYRIGHT)->SetWindowText(GetStr(IDC_ABOUT_COPYRIGHT));
			pDlg->GetDlgItem(IDC_ABOUT_ALARAM)->SetWindowText(GetStr(IDC_ABOUT_ALARAM));
			pDlg->GetDlgItem(IDC_ABOUT_USERIGHT)->SetWindowText(GetStr(IDC_ABOUT_USERIGHT));
			pDlg->GetDlgItem(IDC_ABOUTDLG_REGISTER)->SetWindowText(GetStr(IDC_ABOUTDLG_REGISTER));
			pDlg->GetDlgItem(IDOK)->SetWindowText(GetStr(IDOK));
		}
		break;
	case IDD_APPERENCE:
		{
			pDlg->GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowText(GetStr(IDC_STATIC_PREVIEW));
			pDlg->GetDlgItem(IDC_STATIC_SCHEMA)->SetWindowText(GetStr(IDC_STATIC_SCHEMA));
			pDlg->GetDlgItem(IDC_STATIC_MODIFYCOLOR)->SetWindowText(GetStr(IDC_STATIC_MODIFYCOLOR));
			pDlg->GetDlgItem(IDC_BTN_SELCOLOR)->SetWindowText(GetStr(IDC_BTN_SELCOLOR));
			pDlg->GetDlgItem(IDC_BTN_COLOR_DEFAULT)->SetWindowText(GetStr(IDC_BTN_COLOR_DEFAULT));
			pDlg->GetDlgItem(IDC_STATIC_MODIFYFONT)->SetWindowText(GetStr(IDC_STATIC_MODIFYFONT));
			pDlg->GetDlgItem(IDC_BTN_SELFONT)->SetWindowText(GetStr(IDC_BTN_SELFONT));
			pDlg->GetDlgItem(IDC_BTN_FONT_DEFAULT)->SetWindowText(GetStr(IDC_BTN_FONT_DEFAULT));
		}
		break;
	case IDD_DLG_CHANGEPSWD:
		{
			pDlg->GetDlgItem(IDC_STATIC_PWDTYPE)->SetWindowText(GetStr(IDC_STATIC_PWDTYPE));
			pDlg->GetDlgItem(IDC_STATIC_CURRENTPWD)->SetWindowText(GetStr(IDC_STATIC_CURRENTPWD));
			pDlg->GetDlgItem(IDC_STATIC_NEWPWD)->SetWindowText(GetStr(IDC_STATIC_NEWPWD));
			pDlg->GetDlgItem(IDC_STATIC_CONFIRMPWD)->SetWindowText(GetStr(IDC_STATIC_CONFIRMPWD));
			pDlg->GetDlgItem(IDC_BUTTON_OK)->SetWindowText(GetStr(IDOK));
			pDlg->GetDlgItem(IDCANCEL)->SetWindowText(GetStr(IDCANCEL));
		}
		break;
	case IDD_LOGIN:
		{
			pDlg->GetDlgItem(IDC_STATIC_SERVER)->SetWindowText(GetStr(IDC_STATIC_SERVER));
			pDlg->GetDlgItem(IDC_STATIC_ACCOUNT)->SetWindowText(GetStr(IDC_STATIC_ACCOUNT));
			pDlg->GetDlgItem(IDC_STATIC_PWD)->SetWindowText(GetStr(IDC_STATIC_PWD));
			pDlg->GetDlgItem(IDC_STATIC_CHECKCODE)->SetWindowText(GetStr(IDC_STATIC_CHECKCODE));
			pDlg->GetDlgItem(IDC_STATIC_REGIST)->SetWindowText(GetStr(IDC_STATIC_REGIST));
			pDlg->GetDlgItem(IDC_CHECK_SAVEACCOUNT)->SetWindowText(GetStr(IDC_CHECK_SAVEACCOUNT));
			pDlg->GetDlgItem(IDC_CHECK_SAVEPASSWORD)->SetWindowText(GetStr(IDC_CHECK_SAVEPASSWORD));

			pDlg->GetDlgItem(IDC_RADIO1)->SetWindowText(GetStr(IDC_RADIO1));
			pDlg->GetDlgItem(IDC_RADIO2)->SetWindowText(GetStr(IDC_RADIO2));
			return;
		}
		break;
	case IDD_INPUT_DIALOG:
		{//无文字类修改
			return;
		}
		break;
	case IDD_SAVECLR:
		{
			pDlg->GetDlgItem(IDC_STATIC_SAVEAS)->SetWindowText(GetStr(IDC_STATIC_SAVEAS));
			pDlg->GetDlgItem(IDOK)->SetWindowText(GetStr(IDOK));
			pDlg->GetDlgItem(IDCANCEL)->SetWindowText(GetStr(IDCANCEL));
		}
		break;
	case IDD_SYSPROTY:
		{
			pDlg->GetDlgItem(IDOK)->SetWindowText(GetStr(IDOK));
			pDlg->GetDlgItem(IDCANCEL)->SetWindowText(GetStr(IDCANCEL));
		}
		break;
	default:
		break;
	}
// 	CFont* pFont = CInterfaceManager::Instance()->GetSysFont(ID_DLGINFOFONT);
// 	if(NULL != pFont)
// 	{
// 		::EnumChildWindows(pDlg->m_hWnd, ::SetChildFont, (LPARAM)pFont);
// 	}

	pDlg->SetWindowText(GetStr(uiDlgID));
}

void   CLanguageInterface::InitRightMenu(CMenu* pMenu,int nMenuPos)
{

	switch(nMenuPos)
	{
	case 0:
		{
			pMenu->ModifyMenu(ID_SYSTEM_NEWORDER,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_NEWORDER,GetMenuStr(10071));
			pMenu->ModifyMenu(ID_QUOTE_ANALYSISTPIC,MF_BYCOMMAND|MF_STRING,ID_QUOTE_ANALYSISTPIC,GetStr(20002));
			pMenu->ModifyMenu(ID_QUOTE_REALRUNPIC,MF_BYCOMMAND|MF_STRING,ID_QUOTE_REALRUNPIC,GetStr(20001));
			pMenu->ModifyMenu(ID_QUOTE_SHOWOTHERQUOTE,MF_BYCOMMAND|MF_STRING,ID_QUOTE_SHOWOTHERQUOTE,GetMenuStr(10093));
			pMenu->ModifyMenu(ID_QUOTE_SHOWUPDATETIME,MF_BYCOMMAND|MF_STRING,ID_QUOTE_SHOWUPDATETIME,GetMenuStr(10094));
			pMenu->ModifyMenu(ID_QUOTE_SHOWHIGHLOW,MF_BYCOMMAND|MF_STRING,ID_QUOTE_SHOWHIGHLOW,GetMenuStr(10095));
			pMenu->ModifyMenu(ID_TECHANALY_TECHTYPE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_TECHTYPE,GetMenuStr(10016));
			pMenu->ModifyMenu(ID_TECHANALY_ZOOMOUT,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_ZOOMOUT,GetMenuStr(10018));
			pMenu->ModifyMenu(ID_TECHANALY_ZOOMIN,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_ZOOMIN,GetMenuStr(10019));

			pMenu->ModifyMenu(ID_TECHANALY_1MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_1MINLINE,GetMenuStr(10021));
			pMenu->ModifyMenu(ID_TECHANALY_5MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_5MINLINE,GetMenuStr(10022));
			pMenu->ModifyMenu(ID_TECHANALY_10MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_10MINLINE,GetMenuStr(10023));
			pMenu->ModifyMenu(ID_TECHANALY_15MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_15MINLINE,GetMenuStr(10024));
			pMenu->ModifyMenu(ID_TECHANALY_30MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_30MINLINE,GetMenuStr(10025));
			pMenu->ModifyMenu(ID_TECHANALY_60MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_60MINLINE,GetMenuStr(10026));
			pMenu->ModifyMenu(ID_TECHANALY_DAYLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_DAYLINE,GetMenuStr(10027));
			pMenu->ModifyMenu(ID_TECHANALY_WEEKLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_WEEKLINE,GetMenuStr(10028));
			pMenu->ModifyMenu(ID_TECHANALY_MONLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_MONLINE,GetMenuStr(10029));
			pMenu->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_ONEPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_ONEPIC,GetMenuStr(10030));
			pMenu->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_TWOPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_TWOPIC,GetMenuStr(10031));
			pMenu->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_THREEPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_THREEPIC,GetMenuStr(10032));
			pMenu->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_FOURPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_FOURPIC,GetMenuStr(10033));
			pMenu->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_SIXPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_SIXPIC,GetMenuStr(10034));
			pMenu->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_NINEPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_NINEPIC,GetMenuStr(10035));
			pMenu->ModifyMenu(ID_TECHANALY_TOTAL,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_TOTAL,GetMenuStr(10033));
			pMenu->ModifyMenu(ID_TECHANALY_PREVSTK,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_PREVSTK,GetMenuStr(10036));
			pMenu->ModifyMenu(ID_TECHANALY_NEXTSTK,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_NEXTSTK,GetMenuStr(10037));			
			pMenu->ModifyMenu(2,MF_BYPOSITION|MF_STRING,2,GetMenuStr(10013));
			pMenu->ModifyMenu(3,MF_BYPOSITION|MF_STRING,3,GetMenuStr(10014));
			pMenu->ModifyMenu(4,MF_BYPOSITION|MF_STRING,4,_T("画线工具"));
			pMenu->ModifyMenu(5,MF_BYPOSITION|MF_STRING,5,GetMenuStr(10015));
			CString strFunctionMenu;
			strFunctionMenu = GetMenuStr(10012);
			if(strFunctionMenu.IsEmpty())
			{
				pMenu->DeleteMenu(ID_INTELLIG_FUNCTIONMANAGE,MF_BYCOMMAND);
			}
			else
			{
				pMenu->ModifyMenu(ID_INTELLIG_FUNCTIONMANAGE,MF_BYCOMMAND|MF_STRING,ID_INTELLIG_FUNCTIONMANAGE,strFunctionMenu);
			}
			strFunctionMenu = GetMenuStr(10097);
			if(strFunctionMenu.IsEmpty())
			{
				pMenu->DeleteMenu(ID_TECHANALY_TOTAL,MF_BYCOMMAND);
			}
			else
			{
				pMenu->ModifyMenu(ID_TECHANALY_TOTAL,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_TOTAL,strFunctionMenu);
			}
		}
		break;
	case 1:
		{
			pMenu->ModifyMenu(ID_INDEX_PARAM,MF_BYCOMMAND|MF_STRING,ID_INDEX_PARAM,GetMenuStr(10101));
			pMenu->ModifyMenu(ID_INDEX_INFO,MF_BYCOMMAND|MF_STRING,ID_INDEX_INFO,GetMenuStr(10102));
			pMenu->ModifyMenu(ID_INDEX_DELETE,MF_BYCOMMAND|MF_STRING,ID_INDEX_DELETE,GetMenuStr(10103));
			pMenu->ModifyMenu(ID_INDEX_MODIFYFUNCTION,MF_BYCOMMAND|MF_STRING,ID_INDEX_MODIFYFUNCTION,GetMenuStr(10104));
		}
		break;
	case 2:
		{
			pMenu->ModifyMenu(ID_SYSTEM_NEWORDER,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_NEWORDER,GetMenuStr(10071));
			pMenu->ModifyMenu(ID_QUOTE_ANALYSISTPIC,MF_BYCOMMAND|MF_STRING,ID_QUOTE_ANALYSISTPIC,GetStr(20002));
			pMenu->ModifyMenu(ID_QUOTE_REALRUNPIC,MF_BYCOMMAND|MF_STRING,ID_QUOTE_REALRUNPIC,GetStr(20001));
			pMenu->ModifyMenu(ID_QUOTE_SHOWOTHERQUOTE,MF_BYCOMMAND|MF_STRING,ID_QUOTE_SHOWOTHERQUOTE,GetMenuStr(10093));
			pMenu->ModifyMenu(ID_QUOTE_SHOWUPDATETIME,MF_BYCOMMAND|MF_STRING,ID_QUOTE_SHOWUPDATETIME,GetMenuStr(10094));
			pMenu->ModifyMenu(ID_QUOTE_SHOWHIGHLOW,MF_BYCOMMAND|MF_STRING,ID_QUOTE_SHOWHIGHLOW,GetMenuStr(10095));
		}
		break;
	case 3:
		{
			pMenu->ModifyMenu(ID_SYSTEM_NEWORDER,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_NEWORDER,GetMenuStr(10071));
		}
		break;
	case 4:
		{
			pMenu->ModifyMenu(ID_TOOL_BOSSKEY,MF_BYCOMMAND|MF_STRING,ID_TOOL_BOSSKEY,GetMenuStr(10092));
		}
		break;
	default:
		break;
	}

}
void   CLanguageInterface::InitMainMenu(CMenu* pMenu)
{
	if(NULL == pMenu)return;
	pMenu->ModifyMenu(0,MF_BYPOSITION|MF_STRING,0,GetMenuStr(10001));
	pMenu->ModifyMenu(1,MF_BYPOSITION|MF_STRING,1,GetMenuStr(10011));
	pMenu->ModifyMenu(2,MF_BYPOSITION|MF_STRING,2,GetMenuStr(10050));
	pMenu->ModifyMenu(3,MF_BYPOSITION|MF_STRING,3,GetMenuStr(10060));
	pMenu->ModifyMenu(4,MF_BYPOSITION|MF_STRING,4,GetMenuStr(10070));
	pMenu->ModifyMenu(5,MF_BYPOSITION|MF_STRING,5,GetMenuStr(10080));
	pMenu->ModifyMenu(6,MF_BYPOSITION|MF_STRING,6,GetMenuStr(10090));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_SYSTEM_CONNECTTOHOST,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_CONNECTTOHOST,GetMenuStr(10002));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_SYSTEM_DISCONNECTHOST,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_DISCONNECTHOST,GetMenuStr(10003));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_FILE_PRINT,MF_BYCOMMAND|MF_STRING,ID_FILE_PRINT,GetMenuStr(10004));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_FILE_PRINT_PREVIEW,MF_BYCOMMAND|MF_STRING,ID_FILE_PRINT_PREVIEW,GetMenuStr(10005));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_FILE_PRINT_SETUP,MF_BYCOMMAND|MF_STRING,ID_FILE_PRINT_SETUP,GetMenuStr(10006));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_SYSTEM_SAVEASPIC,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_SAVEASPIC,GetMenuStr(10007));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_SYSTEM_AUTOUPDATE,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_AUTOUPDATE,GetMenuStr(10008));
	pMenu->GetSubMenu(0)->ModifyMenu(ID_APP_EXIT,MF_BYCOMMAND|MF_STRING,ID_APP_EXIT,GetMenuStr(10009));


	pMenu->GetSubMenu(1)->ModifyMenu(1,MF_BYPOSITION|MF_STRING,1,GetMenuStr(10013));
	pMenu->GetSubMenu(1)->ModifyMenu(2,MF_BYPOSITION|MF_STRING,2,GetMenuStr(10014));
	pMenu->GetSubMenu(1)->ModifyMenu(3,MF_BYPOSITION|MF_STRING,3,GetMenuStr(10015));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_TECHTYPE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_TECHTYPE,GetMenuStr(10016));
	pMenu->GetSubMenu(1)->ModifyMenu(5,MF_BYPOSITION|MF_STRING,5,GetMenuStr(10017));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_ZOOMOUT,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_ZOOMOUT,GetMenuStr(10018));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_ZOOMIN,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_ZOOMIN,GetMenuStr(10019));

	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_1MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_1MINLINE,GetMenuStr(10021));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_5MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_5MINLINE,GetMenuStr(10022));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_10MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_10MINLINE,GetMenuStr(10023));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_15MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_15MINLINE,GetMenuStr(10024));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_30MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_30MINLINE,GetMenuStr(10025));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_60MINLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_60MINLINE,GetMenuStr(10026));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_DAYLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_DAYLINE,GetMenuStr(10027));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_WEEKLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_WEEKLINE,GetMenuStr(10028));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_MONLINE,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_MONLINE,GetMenuStr(10029));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_ONEPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_ONEPIC,GetMenuStr(10030));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_TWOPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_TWOPIC,GetMenuStr(10031));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_THREEPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_THREEPIC,GetMenuStr(10032));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_FOURPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_FOURPIC,GetMenuStr(10033));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_SIXPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_SIXPIC,GetMenuStr(10034));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_ANALYSIT_PICCOMMBIN_NINEPIC,MF_BYCOMMAND|MF_STRING,ID_ANALYSIT_PICCOMMBIN_NINEPIC,GetMenuStr(10035));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_DRAWLINE_BEELINE,MF_BYCOMMAND|MF_STRING,ID_DRAWLINE_BEELINE,GetMenuStr(10040));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_DRAWLINE_RECTANGLE,MF_BYCOMMAND|MF_STRING,ID_DRAWLINE_RECTANGLE,GetMenuStr(10041));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_DRAWLINE_PARALLEL,MF_BYCOMMAND|MF_STRING,ID_DRAWLINE_PARALLEL,GetMenuStr(10042));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_DRAWLINE_GOLDLINE,MF_BYCOMMAND|MF_STRING,ID_DRAWLINE_GOLDLINE,GetMenuStr(10043));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_DRAWLINE_GANLINE,MF_BYCOMMAND|MF_STRING,ID_DRAWLINE_GANLINE,GetMenuStr(10044));
	pMenu->GetSubMenu(1)->ModifyMenu(ID_DRAWLINE_DELETE,MF_BYCOMMAND|MF_STRING,ID_DRAWLINE_DELETE,GetMenuStr(10045));
	CString strFunctionMenu;
	strFunctionMenu = GetMenuStr(10012);
	if(strFunctionMenu.IsEmpty())
	{
		pMenu->GetSubMenu(1)->DeleteMenu(ID_INTELLIG_FUNCTIONMANAGE,MF_BYCOMMAND);
	}
	else
	{
		pMenu->GetSubMenu(1)->ModifyMenu(ID_INTELLIG_FUNCTIONMANAGE,MF_BYCOMMAND|MF_STRING,ID_INTELLIG_FUNCTIONMANAGE,strFunctionMenu);
	}
	strFunctionMenu = GetMenuStr(10020);
	if(strFunctionMenu.IsEmpty())
	{
		pMenu->GetSubMenu(1)->DeleteMenu(ID_TECHANALY_TOTAL,MF_BYCOMMAND);
	}
	else
	{
		pMenu->GetSubMenu(1)->ModifyMenu(ID_TECHANALY_TOTAL,MF_BYCOMMAND|MF_STRING,ID_TECHANALY_TOTAL,strFunctionMenu);
	}
	pMenu->GetSubMenu(2)->ModifyMenu(0,MF_BYPOSITION|MF_STRING,0,GetMenuStr(10051));
	pMenu->GetSubMenu(2)->ModifyMenu(ID_VIEW_STATUS_BAR,MF_BYCOMMAND|MF_STRING,ID_VIEW_STATUS_BAR,GetMenuStr(10052));
	pMenu->GetSubMenu(2)->ModifyMenu(ID_TOOL_FULLSCREEN,MF_BYCOMMAND|MF_STRING,ID_TOOL_FULLSCREEN,GetMenuStr(10053));

	pMenu->GetSubMenu(3)->ModifyMenu(ID_TRADE_SIGN,MF_BYCOMMAND|MF_STRING,ID_TRADE_SIGN,GetMenuStr(10061));
	pMenu->GetSubMenu(3)->ModifyMenu(ID_TRADE_TERMINATE,MF_BYCOMMAND|MF_STRING,ID_TRADE_TERMINATE,GetMenuStr(10062));
	pMenu->GetSubMenu(3)->ModifyMenu(ID_TRADE_OUTMONEY,MF_BYCOMMAND|MF_STRING,ID_TRADE_OUTMONEY,GetMenuStr(10063));
	pMenu->GetSubMenu(3)->ModifyMenu(ID_TRADE_INMONEY,MF_BYCOMMAND|MF_STRING,ID_TRADE_INMONEY,GetMenuStr(10064));


	pMenu->GetSubMenu(4)->ModifyMenu(ID_SYSTEM_NEWORDER,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_NEWORDER,GetMenuStr(10071));
	pMenu->GetSubMenu(4)->ModifyMenu(ID_TRADE_HOLD,MF_BYCOMMAND|MF_STRING,ID_TRADE_HOLD,GetMenuStr(10072));
	pMenu->GetSubMenu(4)->ModifyMenu(ID_SYSTEM_PRICEORDER,MF_BYCOMMAND|MF_STRING,ID_SYSTEM_PRICEORDER,GetMenuStr(10073));
	pMenu->GetSubMenu(4)->ModifyMenu(ID_TRADE_PRODUCT,MF_BYCOMMAND|MF_STRING,ID_TRADE_PRODUCT,GetMenuStr(10074));
	pMenu->GetSubMenu(4)->ModifyMenu(ID_TRADE_MODIFYPWD,MF_BYCOMMAND|MF_STRING,ID_TRADE_MODIFYPWD,GetMenuStr(10075));
	pMenu->GetSubMenu(4)->ModifyMenu(ID_TOOL_UPDATE,MF_BYCOMMAND|MF_STRING,ID_TOOL_UPDATE,GetMenuStr(10083));

	pMenu->GetSubMenu(5)->ModifyMenu(ID_TOOL_BOSSKEY,MF_BYCOMMAND|MF_STRING,ID_TOOL_BOSSKEY,GetMenuStr(10081));
	pMenu->GetSubMenu(5)->ModifyMenu(ID_TOOL_SYSTEMSETTING,MF_BYCOMMAND|MF_STRING,ID_TOOL_SYSTEMSETTING,GetMenuStr(10082));
	pMenu->GetSubMenu(5)->ModifyMenu(ID_TOOL_CALC,MF_BYCOMMAND|MF_STRING,ID_TOOL_CALC,GetMenuStr(10084));

	if(CPublic::Instance()->GetHow2UseWebPage().IsEmpty())
	{
		pMenu->GetSubMenu(6)->DeleteMenu(ID_HELP_HOW2USE,MF_BYCOMMAND);
	}
	else
	{
		pMenu->GetSubMenu(6)->ModifyMenu(ID_HELP_HOW2USE,MF_BYCOMMAND|MF_STRING,ID_HELP_HOW2USE,GetMenuStr(10096));
	}
	pMenu->GetSubMenu(6)->ModifyMenu(ID_APP_ABOUT,MF_BYCOMMAND|MF_STRING,ID_APP_ABOUT,GetMenuStr(10091));


	///投资者教育
	if(CPublic::Instance()->GetEducationStr().IsEmpty())
	{
		pMenu->GetSubMenu(6)->DeleteMenu(ID_HELP_EDUCATION,MF_BYCOMMAND);
	}
	else
	{
		pMenu->GetSubMenu(6)->ModifyMenu(ID_HELP_EDUCATION,MF_BYCOMMAND|MF_STRING,ID_HELP_EDUCATION,GetMenuStr(10097));
	}
}

void CLanguageInterface::InitToolBar(UINT uiToolBar,CMFCToolBar* pToolBar)
{
	if(IDR_MAINFRAME == uiToolBar)
	{
		pToolBar->SetButtonText(0,GetMenuStr(10071));
		pToolBar->SetButtonText(1,GetMenuStr(10073));
		pToolBar->SetButtonText(2,GetMenuStr(10072));
		pToolBar->SetButtonText(3,GetMenuStr(10084));

		pToolBar->SetButtonText(5,GetMenuStr(10063));
		pToolBar->SetButtonText(6,GetMenuStr(10064));
		pToolBar->SetButtonText(8,GetMenuStr(10021));
		pToolBar->SetButtonText(9,GetMenuStr(10022));
		pToolBar->SetButtonText(10,GetMenuStr(10023));
		pToolBar->SetButtonText(11,GetMenuStr(10024));
		pToolBar->SetButtonText(12,GetMenuStr(10025));
		pToolBar->SetButtonText(13,GetMenuStr(10026));
		pToolBar->SetButtonText(14,GetMenuStr(10027));
		pToolBar->SetButtonText(15,GetMenuStr(10028));
		pToolBar->SetButtonText(16,GetMenuStr(10029));
		pToolBar->SetButtonText(17,GetMenuStr(10020));
		pToolBar->SetButtonText(18,GetMenuStr(10012));
		pToolBar->SetButtonText(19,GetStr(152));
		pToolBar->SetButtonText(20,GetMenuStr(10081));
		pToolBar->SetButtonText(21,GetMenuStr(10083));
		pToolBar->SetButtonText(22,GetMenuStr(10053));
	}
	else if(IDT_DRAWLINE == uiToolBar)
	{
		pToolBar->SetButtonText(0,GetMenuStr(10046));
		pToolBar->SetButtonText(1,GetMenuStr(10040));
		pToolBar->SetButtonText(2,GetMenuStr(10041));
		pToolBar->SetButtonText(3,GetMenuStr(10042));
		pToolBar->SetButtonText(4,GetMenuStr(10043));
		pToolBar->SetButtonText(5,GetMenuStr(10044));
		pToolBar->SetButtonText(6,GetMenuStr(10045));
	}
}
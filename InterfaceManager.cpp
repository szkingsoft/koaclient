///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InterfaceManager.h"
#include <wingdi.h>
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Construction/Destruction /////////////////////////////////////////

////**** public 
CInterfaceManager* CInterfaceManager::_instance = NULL;
////**** public
CInterfaceManager* CInterfaceManager::Instance()
{
	if(!_instance)
	{
		_instance = new CInterfaceManager;
	}
	return _instance;
}

void CInterfaceManager::ExitInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

////**** private

CInterfaceManager::CInterfaceManager()
{
	//获取应用程序路径
	m_strInterfaceParamPath = CPublic::Instance()->GetSysPath() + _T("INTERFACE\\");
	m_nCurSysStyle = 0;
}

CInterfaceManager::~CInterfaceManager()
{
	DeleteColorAndFont();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CInterfaceManager::Init()
{
	DeleteColorAndFont();	

	InitSysStyle();
    CString strSysStyleName;
	if( !GetSysStyleFileName(m_nCurSysStyle,strSysStyleName))
	{
		return FALSE;
	}
	ReloadSyStyle(m_nCurSysStyle,strSysStyleName);
	return TRUE;
}

BOOL CInterfaceManager::ReloadSyStyle(int nSel,CString strStyle)
{


	LoadSysColor(strStyle);
	LoadSysFont(strStyle);


	m_nCurSysStyle = nSel;
	CIniFile iniFile;
	if( !iniFile.Create(m_strInterfaceParamPath + _T("colorscheme.ini")))
		return FALSE;

	struct stuDefFontColor
	{	
		LOGFONT lfNumber;
		LOGFONT	lfDlg;
	};
	stuDefFontColor tradecolordef;
//	GetSysFont(ID_PROPORTION_FONT)->GetLogFont(&tradecolordef.lfProp);
	GetSysFont(ID_NUMBER_FONT)->GetLogFont(&tradecolordef.lfNumber);
	GetSysFont(ID_DLGINFOFONT)->GetLogFont(&tradecolordef.lfDlg);
	CPublic::Instance()->GetTradeDll()->m_pSetInterfaceColor(&tradecolordef);
	if(m_nCurSysStyle == nSel)return FALSE;
	// 设置登录时选择的系统风格
	iniFile.SetVarInt(_T("CURRENT"),_T("CURRENT"),m_nCurSysStyle);	
	return TRUE;
}

int CInterfaceManager::GetSysStyle()
{
	return m_nCurSysStyle;
}

// 获取系统类型名称
BOOL CInterfaceManager::GetSysStyleFileName(int nSysStyle,CString &rstrSysStyleName)
{	
	vector<TSysStyleDefInfo>::iterator iterSysStyle;
	for( iterSysStyle = m_vecSysStyle.begin();
	iterSysStyle != m_vecSysStyle.end();
	iterSysStyle++)
	{
		if( iterSysStyle->nStyle == nSysStyle)
		{
			rstrSysStyleName = iterSysStyle->szFileName;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL  CInterfaceManager::GetSysStyleName(int nStyle,CString& strName)
{
	vector<TSysStyleDefInfo>::iterator iterSysStyle;
	for( iterSysStyle = m_vecSysStyle.begin();
		iterSysStyle != m_vecSysStyle.end();
		iterSysStyle++)
	{
		if( iterSysStyle->nStyle == nStyle)
		{
			strName = iterSysStyle->szName;
			return TRUE;
		}
	}

	return FALSE;
}

COLORREF CInterfaceManager::GetSysColor(UINT uiColorID)
{
	vector<TColorDefInfo>::iterator iterColorDef;
	for( iterColorDef = m_vecSysColor.begin();
	iterColorDef != m_vecSysColor.end();
	iterColorDef++)
	{
		if( iterColorDef->uiID == uiColorID)
		{
			return iterColorDef->crColor;
		}
	}

	return -1;
}

BOOL CInterfaceManager::UpdateSysColor(const TColorDefInfo& tColorDef)
{
	vector<TColorDefInfo>::iterator iterColorDef;
	for( iterColorDef = m_vecSysColor.begin();
	iterColorDef != m_vecSysColor.end();
	iterColorDef++)
	{
		if( iterColorDef->uiID == tColorDef.uiID)
		{
			iterColorDef->crColor = tColorDef.crColor;
			return TRUE;
		}
	}
	
	return FALSE;
}

CFont* CInterfaceManager::GetSysFont(UINT uiFontID)
{
	vector<TFontDefInfo>::iterator iterFont;
	for( iterFont = m_vecSysFont.begin();
	iterFont != m_vecSysFont.end();
	iterFont++)
	{
		if( uiFontID == iterFont->uiID)
			return iterFont->pFont;
	}

	return NULL;
}

//字体更新
BOOL CInterfaceManager::UpdateSysFont(const TFontDefInfo& tFontDef)
{
	vector<TFontDefInfo>::iterator iterFont;
	for( iterFont = m_vecSysFont.begin();
	iterFont != m_vecSysFont.end();
	iterFont++)
	{
		if( iterFont->uiID == tFontDef.uiID)
		{
			if( iterFont->pFont != NULL)
			{
				iterFont->pFont->DeleteObject();
				delete iterFont->pFont;
			}
			iterFont->pFont = CreateFontEx(tFontDef.szFontName, tFontDef.nSize, tFontDef.nStyle);
			iterFont->nStyle = tFontDef.nStyle;
			iterFont->nSize = tFontDef.nSize;
			_tcscpy(iterFont->szFontName,tFontDef.szFontName);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CInterfaceManager::SaveSysColorList(int nSysStyle,const vector<TColorDefInfo>& vecRefColor)
{  
    CString strSysStyleName;
	if( !GetSysStyleFileName(nSysStyle,strSysStyleName))
		return FALSE;

	strSysStyleName = m_strInterfaceParamPath + strSysStyleName;
	strSysStyleName += ".clr";

	CIniFile iniFile;
	iniFile.Create(strSysStyleName,TRUE);

	CString strValue;
	CString strTag;
	vector<TColorDefInfo>::iterator iterColor;
	for( iterColor = m_vecSysColor.begin();
	iterColor != m_vecSysColor.end();
	iterColor++)
	{
		vector<TColorDefInfo>::const_iterator iterRefColor;
		for( iterRefColor = vecRefColor.begin();
		iterRefColor != vecRefColor.end();
		iterRefColor++)
		{
			if( iterColor->uiID == iterRefColor->uiID)
			{
				iterColor->crColor = iterRefColor->crColor;
		        break;
			}
		}
		strTag.Format(_T("%d"),iterColor->uiID);
		strValue.Format(_T("%d,%s"),iterColor->crColor,iterColor->szName);
		iniFile.SetVarStr(_T("COLOR"),strTag,strValue);		
	}
    return TRUE;
}

BOOL CInterfaceManager::SaveSysFontList(int nSysStyle,const vector<TFontDefInfo>& vecRefFont)
{
	CString strSysStyleName;
	if( !GetSysStyleFileName(nSysStyle,strSysStyleName))
		return FALSE;
	
	strSysStyleName = m_strInterfaceParamPath + strSysStyleName;
	strSysStyleName += ".CLR";

	CIniFile iniFile;
	iniFile.Create(strSysStyleName,TRUE);

	CString strValue;
	CString strTag;

	vector<TFontDefInfo>::iterator iterFont;
	for( iterFont = m_vecSysFont.begin();
	iterFont != m_vecSysFont.end();
	iterFont++)
	{
		vector<TFontDefInfo>::const_iterator iterRefFont;
		for( iterRefFont = vecRefFont.begin();
		iterRefFont != vecRefFont.end();
		iterRefFont++)
		{
			if( iterFont->uiID == iterRefFont->uiID)
			{
				if( *iterFont == *iterRefFont)
				{
					break;
				}
				else
				{
					iterFont->nSize = iterRefFont->nSize;
					iterFont->nStyle = iterRefFont->nStyle;
					_tcscpy(iterFont->szFontName,iterRefFont->szFontName);     
					if( iterFont->pFont != NULL)
					{
						iterFont->pFont->DeleteObject();
						delete iterFont->pFont;
					}
					iterFont->pFont = CreateFontEx(iterFont->szFontName,iterFont->nSize,
						iterFont->nStyle);
				}
				break;
			}
		}
		strTag.Format(_T("%d"),iterFont->uiID);
		strValue.Format(_T("%d,%d,%s,%s"),iterFont->nSize,iterFont->nStyle,iterFont->szFontName,iterFont->szName);
		iniFile.SetVarStr(_T("FONT"),strTag,strValue);
	}
	
    return TRUE;
}

const vector<TColorDefInfo>& CInterfaceManager::GetSysColorList()
{
	return m_vecSysColor;
}

// 获取系统字体列表
const vector<TFontDefInfo>& CInterfaceManager::GetSysFontList()
{
	return m_vecSysFont;
}

// 依据参照价格，获取目标价格的背景颜色
COLORREF CInterfaceManager::GetPriceBackColor(float fDesPrice,float fRefPrice)
{
	COLORREF crDesPrice;
	
	if(fDesPrice > fRefPrice)
		crDesPrice = GetSysColor(ID_RISE_COLOR);
	else if(fDesPrice < fRefPrice)
		crDesPrice = GetSysColor(ID_DOWN_COLOR);
	else
		crDesPrice = GetSysColor(ID_EQUAL_COLOR);
	
	return crDesPrice;	
}

//字体创建(含系统字体的调整)
CFont* CInterfaceManager::CreateFontEx(const TCHAR* cFontName, int nFontSize, int FontStyle
									   , int nCreateType /* = CREATE_INDIRECT */)
{
	CFont* ft = NULL;
	TEXTMETRIC stuTextMet;
	
	ft = CreateFont(cFontName, nFontSize, FontStyle);
	if(0 == _tcscmp(cFontName, _T("System")))
	{//系统字体
		//检测系统显示模式是大字体还是小字体		
		HWND hW = ::GetFocus();
		HDC  hdc = ::GetDC(hW);
		::GetTextMetrics(hdc, &stuTextMet);
		if(stuTextMet.tmHeight > 16)//大字体
		{
			//如果系统采用大字体，则替换掉系统字体
			delete ft;
			ft = CreateFont(_T("GB321"), 15, 0);
		}
		ReleaseDC(hW,hdc);
	}
	return ft;
}

////**** private

BOOL CInterfaceManager::InitSysStyle()
{
	CIniFile iniFile;
	if( !iniFile.Create(m_strInterfaceParamPath + _T("colorscheme.ini")))
		return FALSE;
	
	TSysStyleDefInfo tSysStyleDef;
	int nStyleCount = iniFile.CalcItemNums(_T("SCHEME"));	   	   
	CString strTemp,strIndex;
	int nPos;
	CString strName,strFileName;
	for( int i=0; i<nStyleCount; i++)
	{
		strIndex.Format(_T("%d"),i);
		iniFile.GetVarStr(_T("SCHEME"),strIndex,strTemp);
		tSysStyleDef.nStyle = i;
		nPos = strTemp.Find(_T(","));
		strName = strTemp.Left(nPos);
		if(-1 == nPos)continue;
		strFileName = strTemp;
		strFileName.Delete(0,nPos+1);
		_tcsncpy(tSysStyleDef.szName,(TCHAR*)(LPCTSTR)strName,20);
		_tcsncpy(tSysStyleDef.szFileName,(TCHAR*)(LPCTSTR)strFileName,20);
		m_vecSysStyle.push_back(tSysStyleDef);
	}
    // 设置登录时选择的系统风格
	iniFile.GetVarInt(_T("CURRENT"),_T("CURRENT"),m_nCurSysStyle);	

	return TRUE;
}

BOOL CInterfaceManager::LoadSysColor(CString strSysStyleName)
{
	TColorDefInfo tColorDefList[] =
	{
		{_T("背景色"),       ID_SYSBACK_COLOR,RGB(0,0,0)},		
		{_T("阳线及上涨色"), ID_RISE_COLOR,RGB(230,65,0)},
		{_T("平线及平字色"), ID_EQUAL_COLOR,RGB(192,192,192)},
		{_T("下跌色"),       ID_DOWN_COLOR,RGB(90,/*94*/255,255)},
		{_T("坐标刻度色"),   ID_COORDTEXT_COLOR,RGB(168,168,168)},	
		{_T("坐标轴色"),     ID_COORDINATE_COLOR,RGB(0,88,108)},
		{_T("指标1颜色"),    ID_INDEXLINE1_COLOR,RGB(255,255,255)},
		{_T("指标2颜色"),    ID_INDEXLINE2_COLOR,RGB(255,248,80)},
		{_T("指标3颜色"),    ID_INDEXLINE3_COLOR,RGB(255,84,255)},
		{_T("指标4颜色"),    ID_INDEXLINE4_COLOR,RGB(84,84,255)},
		{_T("指标5颜色"),    ID_INDEXLINE5_COLOR,RGB(168,168,168)},
		{_T("指标6颜色"),    ID_INDEXLINE6_COLOR,RGB(168,168,0)},
		{_T("指标7颜色"),    ID_INDEXLINE7_COLOR,RGB(128,128,255)},
		{_T("成交量色"),     ID_TOTALVOL_COLOR,RGB(192,192,0)},
		{_T("大标题色"),     ID_TITLE_COLOR,RGB(2,226,244)},
		{_T("资讯文本色"),ID_INFOTEXT_COLOR,RGB(192,192,192)},
		{_T("图框线色"),     ID_GRIDLINE_COLOR,RGB(0,88,108)},
		{_T("高亮背景色"),   ID_HIGHLIGHT_COLOR,RGB(0,0,128)},
		{_T("阴线色"),       ID_KLINEDOWN_COLOR,RGB(90,255,255)},
		{_T("固定行/列文本色"), ID_GRIDFIXED_COLOR,RGB(192,192,192)},	
		{_T("产品代码颜色"),ID_STOCK_COLOR,RGB(232,232,232)},
		{_T("其它颜色"),ID_OTHER_COLOR,RGB(128,192,0)},	
		{_T("固定列排序箭头色"),ID_SORTARROR_COLOR,RGB(255,0,0)},
		{_T("选中行/列色"),  ID_GRIDROWSEL_COLOR,RGB(10,36,106)},
		{_T("报价板价格底色"),ID_PRICEBOARD_SINGLEBK,RGB(255,255,255)},
		{_T("报价板选择色"),ID_PRICEBOARD_SELECTCOLOR,RGB(255,0,0)},
		{_T("报价板背景色"),ID_PRICEBOARD_BACKGROUNDCOLOR,RGB(146,136,126)},
		{_T("报价板文字色"),ID_PRICEBOARD_TEXTCOLOR,RGB(0,0,0)},
		{_T("系统时间色"),ID_TIMELETTER_COLOR,RGB(255,255,255)},
		{_T("状态栏文本色"),ID_INFOLETTER_COLOR,RGB(255,255,84)},
		{_T("技术分析标题标题色"),ID_FORMTITLE_COLOR,RGB(255,255,84)},
		{_T("现手色"),ID_ColorNowHandLetter,RGB(255,255,0)},
		{_T("备选色"),ID_STATCOLOR,RGB(192,192,192)},
		{_T("买量色"),ID_BUYVOLCOLOR,RGB(255,0,0)},
		{_T("卖量色"),ID_SELLVOLCOLOR,RGB(0,255,0)},	
		{_T("资讯背景色"),ID_INFOBACKGROUND_COLOR,RGB(10,10,10)},
		{_T("行情变化色"),ID_CHANGE_COLOR,RGB(0,0,255)},
		{_T("可交易商品底色"),ID_TRADESYMBOL_COLOR,RGB(0,255,255)},
		{_T("不可交易商品底色"),ID_UNTRADESYMBOL_COLOR,RGB(175,238,238)},
		{_T("画线颜色"),ID_DRAWLINE_COLOR,RGB(255,255,0)},
		{_T("趋势跟踪线颜色"),ID_TREND_TRACECOLOR,RGB(255,255,0)},
		{_T("未来价格虚线色"),ID_FUTURE_DOTCOLOR,RGB(0,128,128)},
		{_T("未来价格实线色"),ID_FUTURE_SOLIDCLOLOR,RGB(255,0,0)},
		{_T("趋势进场色"),ID_TREND_COMECOLOR,RGB(255,0,0)},
		{_T("趋势观察色"),ID_TREND_WATCHCLOLOR,RGB(255,255,0)},
		{_T("趋势离场色"),ID_TREND_GOCOLOR,RGB(0,0,255)},
		{_T("趋势提示信息表格"),ID_FUTURETREND_INFOTITLE,RGB(192,192,192)}
	};
	int nListCount = _countof(tColorDefList);
	strSysStyleName = m_strInterfaceParamPath +strSysStyleName + _T(".CLR");
	CIniFile iniFile;
	CString strTag;
	CString strValue;
	CString strName;
	CString strColor;
	int nPos;
	if(iniFile.Create(strSysStyleName))
	{
		for( int i=0; i < nListCount; i++)
		{
			strTag.Format(_T("%d"),tColorDefList[i].uiID);
			if(!iniFile.GetVarStr(_T("COLOR"),strTag,strValue))continue;	
			nPos = strValue.Find(_T(","));
			strColor = strValue.Left(nPos);		
			strName = strValue;	
			strName.Delete(0,nPos+1);
			tColorDefList[i].crColor = COLORREF(atoi((char*)(LPCTSTR)strColor));
			_tcsncpy(tColorDefList[i].szName,(TCHAR*)(LPCTSTR)strName,24);
		}
	}
	ResetSysColor(nListCount,tColorDefList);

	iniFile.GetVarStr("PIC","PRICEBOARDPIC",m_strPriceBoardBkPic);	
	
	return TRUE;
}

BOOL CInterfaceManager::LoadSysFont(CString strSysStyleName)
{	
	TFontDefInfo tFontDefList[] = 
	{ 
		{_T("列表字体"),ID_TABLE_FONT,      14, FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("图形字体"),ID_NUMBER_FONT,     14, FONT_STYLE_BOLD,_T("Tohoma"),NULL},
		{_T("资讯字体"),ID_INFOTEXT_FONT,   15, 0,              _T("System"),NULL},
		{_T("标题字体"),ID_TITLE_FONT,      22, FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("涨跌比例字体"),ID_PROPORTION_FONT,10,FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("状态栏字体"),ID_STOCKTRACE_FONT, 14, FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("报价板大字体"),ID_PRICEBOARD_PRICEBIGFONT,16,FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("报价板字体"),ID_PRICEBOARD_PRICEBOARDFONT,6,FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("分析抬头字体"),ID_HISTORYPICTITLEFONT,14,0,_T("System"),NULL},
		{_T("对话框字体"),ID_DLGINFOFONT,16,0,_T("System"),NULL}
	};
	int nListCount = _countof(tFontDefList);
	strSysStyleName = m_strInterfaceParamPath +strSysStyleName + _T(".CLR");
	CIniFile iniFile;
	CString strTag;
	CString strValue;
	CString strName;
	CString strSize;
	CString strStyle;
	CString strFontName;

	int nPos;
	if(iniFile.Create(strSysStyleName))
	{
		for( int i=0; i < nListCount; i++)
		{
			strTag.Format(_T("%d"),tFontDefList[i].uiID);
			if( iniFile.GetVarStr(_T("FONT"),strTag,strValue))
			{
				nPos = strValue.Find(_T(","));
				strSize = strValue.Left(nPos);
				strValue.Delete(0,nPos+1);
				nPos = strValue.Find(_T(","));
				strStyle = strValue.Left(nPos);
				strValue.Delete(0,nPos+1);
				nPos = strValue.Find(_T(","));
				strFontName = strValue.Left(nPos);
				strValue.Delete(0,nPos+1);
				strName = strValue;
				tFontDefList[i].nSize = atoi((char*)(LPCTSTR)strSize);
				tFontDefList[i].nStyle = atoi((char*)(LPCTSTR)strStyle);
				_tcsncpy(tFontDefList[i].szFontName,(TCHAR*)(LPCTSTR)strFontName,36);
				_tcsncpy(tFontDefList[i].szName,(TCHAR*)(LPCTSTR)strName,24);
			}
		}
	}
	ResetSysFont(nListCount,tFontDefList);	
	return TRUE;
}

void CInterfaceManager::DeleteColorAndFont()
{
    m_vecSysColor.clear();

	FreeSysFont();
}

void CInterfaceManager::FreeSysFont()
{
	vector<TFontDefInfo>::iterator iterFont;
	for( iterFont = m_vecSysFont.begin();
	iterFont != m_vecSysFont.end();
	iterFont++)
	{
		if( iterFont->pFont != NULL)
		{
			iterFont->pFont->DeleteObject();
			delete iterFont->pFont;
			iterFont->pFont = NULL;
		}
	}	
	
	m_vecSysFont.clear();
}

CFont* CInterfaceManager::CreateFont(const TCHAR *cFontName, int nFontSize, int nFontStyle, 
									 int nCreateType)
{
	CFont *pNewFont = new CFont;
	if (pNewFont == NULL)
	{
		return NULL;
	}
	if((cFontName == NULL) || cFontName[0] == '\0')
	{
		cFontName = _T("System");
		nFontSize = 16;
		nFontStyle = 0;
	}
	LOGFONT font;
	memset(&font, 0, sizeof(font));
	font.lfHeight = -nFontSize;
	font.lfWeight = FW_NORMAL;
	font.lfCharSet = DEFAULT_CHARSET;
	font.lfOutPrecision = OUT_CHARACTER_PRECIS;
	_tcscpy(font.lfFaceName, cFontName);
	if (nFontStyle & FONT_STYLE_BOLD)
	{
		font.lfWeight = FW_BOLD;
	}
	if (nFontStyle & FONT_STYLE_ITALIC)
	{
		font.lfItalic = TRUE;
	}
	if (nFontStyle & FONT_STYLE_UNDERLINE)
	{
		font.lfUnderline = TRUE;
	}
	if (nFontStyle & FONT_STYLE_UNDERLINE)
	{
		font.lfUnderline = TRUE;
	}
	if ( nCreateType  == CREATE_INDIRECT )
	{
		if(!pNewFont->CreateFontIndirect(&font))
		{
			delete pNewFont;
			pNewFont = NULL;
		}
	}
	else if ( nCreateType  == CREATE_POINT )
	{
		if(!pNewFont->CreatePointFont( 120 , cFontName ) )
		{
			delete pNewFont;
			pNewFont = NULL;
		}
	}
	
	return pNewFont;
}

void CInterfaceManager::ResetSysColor(int nCount, TColorDefInfo *pColorDefList)
{
	m_vecSysColor.clear();
	for( int i=0; i<nCount; i++)
	{
		m_vecSysColor.push_back(pColorDefList[i]);
	}
}

void CInterfaceManager::ResetSysFont(int nCount, TFontDefInfo *pFontDefList)
{   
	FreeSysFont();
	
	for( int i=0; i<nCount; i++)
	{
		pFontDefList[i].pFont = CreateFontEx(pFontDefList[i].szFontName, pFontDefList[i].nSize, 
			                       pFontDefList[i].nStyle);
	    m_vecSysFont.push_back(pFontDefList[i]);
	}
}

BOOL CInterfaceManager::ConvertColorParaFile()
{	
	struct ColorDefine
	{
		UINT		m_nID;
		COLORREF	m_lColor;
	};

    CString strStyleName = m_strInterfaceParamPath;

	CFile stockFile;
	
	vector<TSysStyleDefInfo>::iterator iterStyle;
	for( iterStyle = m_vecSysStyle.begin();
	iterStyle != m_vecSysStyle.end();
	iterStyle++)
	{
        strStyleName = m_strInterfaceParamPath + iterStyle->szName;
		if(stockFile.Open((strStyleName+".CLR"),CFile::modeRead|CFile::typeBinary))
		{
			int nFileListCount = stockFile.GetLength()/sizeof(ColorDefine);
			ColorDefine* pFileColorDef = new ColorDefine[nFileListCount];
			stockFile.Read(pFileColorDef,nFileListCount*sizeof(ColorDefine));
			stockFile.Close();
			
			vector<TColorDefInfo>::iterator iterColor;
			for( iterColor = m_vecSysColor.begin();
			iterColor != m_vecSysColor.end();
			iterColor++)
			{
				for(int i = 0; i < nFileListCount; i++)
				{
					if( iterColor->uiID == pFileColorDef[i].m_nID )
					{
						iterColor->crColor = pFileColorDef[i].m_lColor;
						break;
					}
				}
			}
			delete []pFileColorDef;

			SaveSysColorList(iterStyle->nStyle,m_vecSysColor);
		}
	}

	return TRUE;
}

BOOL CInterfaceManager::ConvertFontParaFile()
{
	struct FontItem
	{
		UINT m_nID;       //字体ID
		int  m_nSize;     //字体大小
		int  m_nStyle;    //字体风格(字形)
		TCHAR m_szName[50];//字体名称
	};

	CString strStyleName = m_strInterfaceParamPath;
	
	CFile stockFile;
	
	vector<TSysStyleDefInfo>::iterator iterStyle;
	for( iterStyle = m_vecSysStyle.begin();
	iterStyle != m_vecSysStyle.end();
	iterStyle++)
	{
        strStyleName = m_strInterfaceParamPath + iterStyle->szName;
		if(stockFile.Open((strStyleName+".FNT"),CFile::modeRead|CFile::typeBinary))
		{
			int nFileListCount = stockFile.GetLength()/sizeof(FontItem);
			FontItem* pFileFontDef = new FontItem[nFileListCount];
			stockFile.Read(pFileFontDef,nFileListCount*sizeof(FontItem));
			stockFile.Close();
			
			vector<TFontDefInfo>::iterator iterFont;
			for( iterFont = m_vecSysFont.begin();
			iterFont != m_vecSysFont.end();
			iterFont++)
			{
				for(int i = 0; i < nFileListCount; i++)
				{
					if( iterFont->uiID == pFileFontDef[i].m_nID )
					{
						iterFont->nSize  = pFileFontDef[i].m_nSize;
						iterFont->nStyle  = pFileFontDef[i].m_nStyle;
						_tcscpy(iterFont->szFontName,pFileFontDef[i].m_szName);
						break;
					}
				}
			}
			delete []pFileFontDef;			
			SaveSysFontList(iterStyle->nStyle,m_vecSysFont);
		}
	}

	return TRUE;
}


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
	//��ȡӦ�ó���·��
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
	// ���õ�¼ʱѡ���ϵͳ���
	iniFile.SetVarInt(_T("CURRENT"),_T("CURRENT"),m_nCurSysStyle);	
	return TRUE;
}

int CInterfaceManager::GetSysStyle()
{
	return m_nCurSysStyle;
}

// ��ȡϵͳ��������
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

//�������
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

// ��ȡϵͳ�����б�
const vector<TFontDefInfo>& CInterfaceManager::GetSysFontList()
{
	return m_vecSysFont;
}

// ���ݲ��ռ۸񣬻�ȡĿ��۸�ı�����ɫ
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

//���崴��(��ϵͳ����ĵ���)
CFont* CInterfaceManager::CreateFontEx(const TCHAR* cFontName, int nFontSize, int FontStyle
									   , int nCreateType /* = CREATE_INDIRECT */)
{
	CFont* ft = NULL;
	TEXTMETRIC stuTextMet;
	
	ft = CreateFont(cFontName, nFontSize, FontStyle);
	if(0 == _tcscmp(cFontName, _T("System")))
	{//ϵͳ����
		//���ϵͳ��ʾģʽ�Ǵ����廹��С����		
		HWND hW = ::GetFocus();
		HDC  hdc = ::GetDC(hW);
		::GetTextMetrics(hdc, &stuTextMet);
		if(stuTextMet.tmHeight > 16)//������
		{
			//���ϵͳ���ô����壬���滻��ϵͳ����
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
    // ���õ�¼ʱѡ���ϵͳ���
	iniFile.GetVarInt(_T("CURRENT"),_T("CURRENT"),m_nCurSysStyle);	

	return TRUE;
}

BOOL CInterfaceManager::LoadSysColor(CString strSysStyleName)
{
	TColorDefInfo tColorDefList[] =
	{
		{_T("����ɫ"),       ID_SYSBACK_COLOR,RGB(0,0,0)},		
		{_T("���߼�����ɫ"), ID_RISE_COLOR,RGB(230,65,0)},
		{_T("ƽ�߼�ƽ��ɫ"), ID_EQUAL_COLOR,RGB(192,192,192)},
		{_T("�µ�ɫ"),       ID_DOWN_COLOR,RGB(90,/*94*/255,255)},
		{_T("����̶�ɫ"),   ID_COORDTEXT_COLOR,RGB(168,168,168)},	
		{_T("������ɫ"),     ID_COORDINATE_COLOR,RGB(0,88,108)},
		{_T("ָ��1��ɫ"),    ID_INDEXLINE1_COLOR,RGB(255,255,255)},
		{_T("ָ��2��ɫ"),    ID_INDEXLINE2_COLOR,RGB(255,248,80)},
		{_T("ָ��3��ɫ"),    ID_INDEXLINE3_COLOR,RGB(255,84,255)},
		{_T("ָ��4��ɫ"),    ID_INDEXLINE4_COLOR,RGB(84,84,255)},
		{_T("ָ��5��ɫ"),    ID_INDEXLINE5_COLOR,RGB(168,168,168)},
		{_T("ָ��6��ɫ"),    ID_INDEXLINE6_COLOR,RGB(168,168,0)},
		{_T("ָ��7��ɫ"),    ID_INDEXLINE7_COLOR,RGB(128,128,255)},
		{_T("�ɽ���ɫ"),     ID_TOTALVOL_COLOR,RGB(192,192,0)},
		{_T("�����ɫ"),     ID_TITLE_COLOR,RGB(2,226,244)},
		{_T("��Ѷ�ı�ɫ"),ID_INFOTEXT_COLOR,RGB(192,192,192)},
		{_T("ͼ����ɫ"),     ID_GRIDLINE_COLOR,RGB(0,88,108)},
		{_T("��������ɫ"),   ID_HIGHLIGHT_COLOR,RGB(0,0,128)},
		{_T("����ɫ"),       ID_KLINEDOWN_COLOR,RGB(90,255,255)},
		{_T("�̶���/���ı�ɫ"), ID_GRIDFIXED_COLOR,RGB(192,192,192)},	
		{_T("��Ʒ������ɫ"),ID_STOCK_COLOR,RGB(232,232,232)},
		{_T("������ɫ"),ID_OTHER_COLOR,RGB(128,192,0)},	
		{_T("�̶��������ͷɫ"),ID_SORTARROR_COLOR,RGB(255,0,0)},
		{_T("ѡ����/��ɫ"),  ID_GRIDROWSEL_COLOR,RGB(10,36,106)},
		{_T("���۰�۸��ɫ"),ID_PRICEBOARD_SINGLEBK,RGB(255,255,255)},
		{_T("���۰�ѡ��ɫ"),ID_PRICEBOARD_SELECTCOLOR,RGB(255,0,0)},
		{_T("���۰屳��ɫ"),ID_PRICEBOARD_BACKGROUNDCOLOR,RGB(146,136,126)},
		{_T("���۰�����ɫ"),ID_PRICEBOARD_TEXTCOLOR,RGB(0,0,0)},
		{_T("ϵͳʱ��ɫ"),ID_TIMELETTER_COLOR,RGB(255,255,255)},
		{_T("״̬���ı�ɫ"),ID_INFOLETTER_COLOR,RGB(255,255,84)},
		{_T("���������������ɫ"),ID_FORMTITLE_COLOR,RGB(255,255,84)},
		{_T("����ɫ"),ID_ColorNowHandLetter,RGB(255,255,0)},
		{_T("��ѡɫ"),ID_STATCOLOR,RGB(192,192,192)},
		{_T("����ɫ"),ID_BUYVOLCOLOR,RGB(255,0,0)},
		{_T("����ɫ"),ID_SELLVOLCOLOR,RGB(0,255,0)},	
		{_T("��Ѷ����ɫ"),ID_INFOBACKGROUND_COLOR,RGB(10,10,10)},
		{_T("����仯ɫ"),ID_CHANGE_COLOR,RGB(0,0,255)},
		{_T("�ɽ�����Ʒ��ɫ"),ID_TRADESYMBOL_COLOR,RGB(0,255,255)},
		{_T("���ɽ�����Ʒ��ɫ"),ID_UNTRADESYMBOL_COLOR,RGB(175,238,238)},
		{_T("������ɫ"),ID_DRAWLINE_COLOR,RGB(255,255,0)},
		{_T("���Ƹ�������ɫ"),ID_TREND_TRACECOLOR,RGB(255,255,0)},
		{_T("δ���۸�����ɫ"),ID_FUTURE_DOTCOLOR,RGB(0,128,128)},
		{_T("δ���۸�ʵ��ɫ"),ID_FUTURE_SOLIDCLOLOR,RGB(255,0,0)},
		{_T("���ƽ���ɫ"),ID_TREND_COMECOLOR,RGB(255,0,0)},
		{_T("���ƹ۲�ɫ"),ID_TREND_WATCHCLOLOR,RGB(255,255,0)},
		{_T("�����볡ɫ"),ID_TREND_GOCOLOR,RGB(0,0,255)},
		{_T("������ʾ��Ϣ���"),ID_FUTURETREND_INFOTITLE,RGB(192,192,192)}
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
		{_T("�б�����"),ID_TABLE_FONT,      14, FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("ͼ������"),ID_NUMBER_FONT,     14, FONT_STYLE_BOLD,_T("Tohoma"),NULL},
		{_T("��Ѷ����"),ID_INFOTEXT_FONT,   15, 0,              _T("System"),NULL},
		{_T("��������"),ID_TITLE_FONT,      22, FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("�ǵ���������"),ID_PROPORTION_FONT,10,FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("״̬������"),ID_STOCKTRACE_FONT, 14, FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("���۰������"),ID_PRICEBOARD_PRICEBIGFONT,16,FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("���۰�����"),ID_PRICEBOARD_PRICEBOARDFONT,6,FONT_STYLE_BOLD,_T("System"),NULL},
		{_T("����̧ͷ����"),ID_HISTORYPICTITLEFONT,14,0,_T("System"),NULL},
		{_T("�Ի�������"),ID_DLGINFOFONT,16,0,_T("System"),NULL}
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
		UINT m_nID;       //����ID
		int  m_nSize;     //�����С
		int  m_nStyle;    //������(����)
		TCHAR m_szName[50];//��������
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


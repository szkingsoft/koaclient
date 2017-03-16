// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "Public.h"
#include "IniFile.h"
#include "MainFrm.h"
#include "StockView.h"
#include "DlgTrendChart.h"
#include "resource.h"
#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPublic* CPublic::_instance = NULL;


void CPublic::ExitInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

CPublic* CPublic::Instance()
{
	if(!_instance)
	{
		_instance = new CPublic;
	}
	return _instance;
}

CPublic::CPublic()
{
	m_sRequestIndex = 1;
	m_iTodayDate = 0;
	m_hLoginWnd = NULL;
	m_nNeedKeyGhost = 1;
	m_nSelServer = -1;
	m_nQuotePort = 0;
	m_nTradePort = 0;
	m_nQuotePort1 = 0;
	m_nTradePort1 = 0;
	m_bSaveAccount =FALSE;

	m_nLanguageType = 0;
	m_nShowWarningInfo = 0;
	m_nLoginMode = 0;
	m_nMainViewMode = 0;
	m_nLoginMode = 0;
	m_nMoneyInOutEnable = 1;

	m_nQuotePort = 0;//行情端口
	m_nTradePort = 0;//交易端口
	m_nQuotePort1 = 0;//行情端口
	m_nTradePort1 = 0;//交易端口
	m_nQuotePort2 = 0;//行情端口
	m_nTradePort2 = 0;//交易端口
	m_nQuotePort3 = 0;//行情端口
	m_nTradePort3 = 0;//交易端口
	m_nQuotePort4 = 0;//行情端口
	m_nTradePort4 = 0;//交易端口
	m_bNeedCFCA = 0;

	m_pStatusWnd = NULL;
	m_pToolBarWnd = NULL;
	m_iClientType = 0;
	m_pCusUser = NULL;
	m_bIsGettingHisData = false;

	InitSysPath();
	memset(&m_oCurrentCode,0,sizeof(SCodeInfo));
	ReadIniInfo();//读取配置文件

	//fLog = CFile::hFileNull;
}

CPublic::~CPublic()
{
	m_vecSymbolInfo.clear();
	//m_vecViewInfo.clear();

	m_mapNotifyWnd.clear();
	if (m_pCusUser)
	{
		delete m_pCusUser;
		m_pCusUser = NULL;
	}
}

const StuSymbolData* CPublic::GetStockInfo(const SCodeInfo* pCode)
{
	int nStockNum;
	nStockNum = m_vecSymbolInfo.size();
	if( nStockNum == 0)return NULL;

	for(int nCount =0;nCount <nStockNum ;nCount++)
	{
		if(0 == strcmp(m_vecSymbolInfo[nCount].sCode.szCode,pCode->szCode))
		{
			return &m_vecSymbolInfo[nCount];
		}
	}
	return NULL;	
}

void CPublic::SetStockTradeStatus(SCodeInfo* pCode)
{
	int nStockNum;
	nStockNum = m_vecSymbolInfo.size();
	if( nStockNum == 0)return;

	for(int nCount =0;nCount <nStockNum ;nCount++)
	{
		if(0 == strcmp(m_vecSymbolInfo[nCount].sCode.szCode,pCode->szCode))
		{
			m_vecSymbolInfo[nCount].cTrade = '0';
			return;
		}
	}
	return;	
}

/////////////////////////////////////////////////函数功能?
//得到当前证券的前一个证券代码
//函数参数: CurrentCode 当前的证券代码
//函数返回：当前证券的前一个代码
//函数异常：无
const SCodeInfo* CPublic::GetPrevStockInfo(const SCodeInfo* pCurrentStock)
{	
	return GetNextSymbol(pCurrentStock,-1);
	int nStockNum;
	nStockNum = m_vecSymbolInfo.size();
	if( nStockNum == 0)return NULL;

	for(int nCount =0;nCount <nStockNum ;nCount++)
	{
		if(0 == strcmp(m_vecSymbolInfo[nCount].sCode.szCode,pCurrentStock->szCode))
		{
			if(nCount == 0)
				nCount = nStockNum-1;//最后一个证券
			else
				nCount -=1;//当前证券下一个
			return &m_vecSymbolInfo[nCount].sCode;
		}
	}
	return NULL;
}

////////////////////////////////////////////////
//函数功能：得到当前证券的后一个代码
//函数参数：当前证券代码
//函数返回：当前证券的后一个代码
//函数异常:无
const SCodeInfo* CPublic::GetNextStockInfo(const SCodeInfo* pCurrentStock)
{
	return GetNextSymbol(pCurrentStock,1);
	int nStockNum;
	nStockNum = m_vecSymbolInfo.size();
	if( nStockNum == 0)return NULL;

	for(int nCount =0;nCount <nStockNum ;nCount++)
	{
		if(0 == strcmp(m_vecSymbolInfo[nCount].sCode.szCode,pCurrentStock->szCode))
		{
			if(nCount == nStockNum-1)
				nCount = 0;//最后一个证券
			else
				nCount +=1;//当前证券下一个
			return &m_vecSymbolInfo[nCount].sCode;
		}
	}
	return NULL;
}
const SCodeInfo*  CPublic::GetNextSymbol(const SCodeInfo* pCurStock,int nDec)
{
	CDlgPriceList * pWnd = (CDlgPriceList *)GetView(6);	
	return pWnd->GetNextSelCodeInfo(pCurStock,nDec);
}

////////////////////////////////////////////////
//函数功能：得到所有市场代码
//函数参数：lpCodeInfo 得到的市场代码
//　　　　　dwCodeNum 返回的代码数量
//函数返回：成功为TRUE,错误为FALSE
//函数异常:无
BOOL CPublic::GetAllMarketCode(SCodeInfo* pCode,int& nCodeNum)
{
	nCodeNum = m_vecSymbolInfo.size();
	if( NULL == pCode)return FALSE;
	for(int nCount =0;nCount <nCodeNum ;nCount++)
	{
		memcpy(&pCode[nCount],&(m_vecSymbolInfo[nCount].sCode),sizeof(SCodeInfo));

	}
	return TRUE;
}

//函数功能：得到某指定市场的证券总数量
//函数参数: nMarket 市场代码
//函数返回：市场数量
//函数异常：无
int CPublic::GetStockTotal()
{
	return m_vecSymbolInfo.size();
}


//功能：初始化股票
//参数：lpData 数据块
//返回：无
void CPublic::InitialStockInfo(const char* lpData)
{
	mapQSA mapGt;
	QueteSymbolAttribute stcQSA[500];
	int iMun = CPublic::Instance()->GetTradeDll()->m_pGetQuoteSymbolAttrib(stcQSA);
	for (int i = 0; i < iMun; i++)
	{
		mapGt[stcQSA[i].strCode] = stcQSA[i];
	}
	if (mapGt.size() <=0 )return;

	const SNormaHead* pNoramlHead = (SNormaHead*)lpData;
	const StuSymbolData* pCodeInfo =(StuSymbolData*)(pNoramlHead+1);
	for(int nCounter = 0;nCounter<pNoramlHead->m_usSize;nCounter++)
	{
		StuSymbolData stcCur = pCodeInfo[nCounter];
		if((pCodeInfo[nCounter].sCode.cPriceUnit<0)||(pCodeInfo[nCounter].nBeginTime<0)||(pCodeInfo[nCounter].nTotalTradeTime<=0))continue;
		if((pCodeInfo[nCounter].cTrade !='0')&&(pCodeInfo[nCounter].cTrade !='1'))continue;
		if(strlen(pCodeInfo[nCounter].szName) == 0)continue;
		//if ((m_iDPUnTrade == 0)&&(pCodeInfo[nCounter].cTrade =='0'))continue;
		//if (pCodeInfo[nCounter].cTrade =='0')
		//{
		//	m_vecSymbolInfo.push_back(pCodeInfo[nCounter]);
		//}
		//else
		{
			if (m_iDPUnTrade == 0)
			{
				if (mapGt.find(pCodeInfo[nCounter].sCode.szCode) != mapGt.end())
				{
					m_vecSymbolInfo.push_back(pCodeInfo[nCounter]);
				}
			}
			else
			{
				m_vecSymbolInfo.push_back(pCodeInfo[nCounter]);
			}
		}	
	}
}



//功能：返回今日日期
//参数：无
//返回：当日日期
int CPublic::GetDate()
{
	return m_iTodayDate;
}


//功能：根据市场类型得到证券小数点位数
//参数：sMarketType 市场类型
//返回：小数点位数
unsigned short CPublic::GetDecimal(const SCodeInfo* pCodeInfo)
{ 
	if(NULL== pCodeInfo)return 0;
	return pCodeInfo->cPriceUnit;
}

// 格式CPublic
CString CPublic::GetFormateValue(float fValue,int nDeimal)
{
	CString strResult;
	CString strFormat;
	strFormat.Format(_T("%%.%df"),nDeimal);
	strResult.Format(strFormat,fValue);
	return strResult;
}

unsigned short CPublic::GetTechIndexValueDecimal(CString strTypeName)
{
	return 2;
}

BOOL CPublic::SendRequestData(const char* lpData, unsigned int uiDataNum)
{
	m_QuoteDataSource.AskData(lpData,uiDataNum);
	return TRUE;
}

//发送登陆验证信息
void CPublic::ReqLoginReq()
{
	int nDataLen;
	nDataLen = sizeof(SNormaHead)+sizeof(SUserPwdDataReq);
	char * pData = new char[nDataLen];
	memset(pData,'\0',nDataLen);
	SNormaHead* pHead = (SNormaHead*)(pData);
	SUserPwdDataReq* pUserPwd = (SUserPwdDataReq*)(pHead+1);
	pHead->m_usIndex = GetRequestIndex();
	pHead->m_usSize = sizeof(SUserPwdDataReq);
	pHead->m_usType = USERPWDVALID;
	pUserPwd->m_uiSoftWareVersion = SOFTWARE_VERSION;
	m_strUserName.Trim();
	strcpy(pUserPwd->m_szUser,(char*)(LPCTSTR)m_strUserName);
//	strcpy(pUserPwd->m_szPwd,(char*)(LPCTSTR)m_strPassword);
	SendRequestData(pData,nDataLen);
	delete []pData;
}

//发送初始化信息
void CPublic::ReqInitReq()
{
	int nDataLen;
	nDataLen = sizeof(SNormaHead);
	char * pData = new char[nDataLen];
	SNormaHead* pHead = (SNormaHead*)(pData);
	pHead->m_usIndex = GetRequestIndex();
	pHead->m_usSize = 0;
	pHead->m_usType = INITMARKET;
	SendRequestData(pData,nDataLen);
	delete []pData;
}

//发送文本信息请求
void CPublic::ReqTxtReq()
{
	int nDataLen;
	nDataLen = sizeof(SNormaHead);
	char * pData = new char[nDataLen];
	SNormaHead* pHead = (SNormaHead*)(pData);
	pHead->m_usIndex = GetRequestIndex();
	pHead->m_usSize = 0;
	pHead->m_usType = TEXTINFO;
	SendRequestData(pData,nDataLen);
	delete []pData;
}


//请求报价信息
void CPublic::ReqReportInfo()
{
	if(GetStockTotal() <=0)return;
	SNormaHead  tRequest;
	tRequest.m_usIndex = AddRequestIndex();
	tRequest.m_usSize = 0;
	tRequest.m_usType = REPORTDATA;
	SendRequestData((char*)&tRequest,sizeof(SNormaHead));//请求报价表数据
}
//请求证券信息
void CPublic::ReqStockInfo()
{
	const SCodeInfo* pSendCode;
	pSendCode = GetDefaultStock();
	if(NULL == pSendCode)return;
	memcpy(&m_oCurrentCode,pSendCode,sizeof(SCodeInfo));
	CWnd* pView;
	pView = GetView(/*VIEWINFO_STOCK*//**/10);
	if(pView)
	{
		pView->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode,RealObj);
	}
	pView = GetView(/*VIEWINFO_STOCK*//**/6);
	if (pView)
	{
		pView->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode);
	}

	pView = GetView(VIEWINFO_STOCK);
	if (pView)
	{
		pView->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode);
	}
}

int CPublic::GetNeedRegRealQuoSymbol(STUSSymbol * pstcSymbol)
{
	int iNum = m_vecSymbolInfo.size();

	if (pstcSymbol == NULL || iNum <= 0) 
	{
		return iNum;
	}
	else
	{
		int i = 0;
		vector <StuSymbolData>::iterator itVec; /*m_vecSymbolInfo*/;
		for (itVec = m_vecSymbolInfo.begin(); itVec != m_vecSymbolInfo.end();itVec++)
		{
			if (i < iNum)
			{
				memcpy(pstcSymbol[i].m_szSymbol,itVec->sCode.szCode,CLIENT_SYMBOL_MAX_LEN+1);
			}
		}
	}
}

const SCodeInfo* CPublic::GetDefaultStock()
{
	const SCodeInfo* pCode = NULL;
	if(m_ssDefaultCode.empty())
	{
		if(m_vecSymbolInfo.size())
		{
			for(int i = 0; i < m_vecSymbolInfo.size();i++)
			{
				if (m_vecSymbolInfo[i].sCode.szCode[0] != 0&&m_vecSymbolInfo[i].cTrade != SYMBOLTYPE_NOTTRADEPRODUCT)
				{
					pCode = &m_vecSymbolInfo[i].sCode;
					break;
				}
			}
			//pCode = &m_vecSymbolInfo[0].sCode;
			if (pCode && pCode->szCode)
			{
				m_ssDefaultCode = pCode->szCode;
			}
		}		
	}
	else
	{
		SCodeInfo code;
		strcpy(code.szCode,m_ssDefaultCode.c_str());
		pCode = &GetStockInfo(&code)->sCode;
	}
	return pCode;
}

//功能：初始化系统路径
void CPublic::InitSysPath()
{
	//取得应用程序所在路径
	TCHAR exeFullPath[MAX_PATH];
	memset(exeFullPath,0,MAX_PATH);
	
	GetModuleFileName(AfxGetInstanceHandle(), exeFullPath, MAX_PATH);
	m_szSysPath = (CString)exeFullPath;
	int position = m_szSysPath.ReverseFind('\\');
	m_szSysPath = m_szSysPath.Left(position+1);
}

//功能：获取系统路径
CString CPublic::GetSysPath()
{
	return m_szSysPath;
}

//功能：设置请求索引
short CPublic::AddRequestIndex()
{ 
	  m_sRequestIndex++;
	 return m_sRequestIndex;  
}

short CPublic::GetRequestIndex() 
{ 
	return m_sRequestIndex;
}

void CPublic::SetRequestHisDataFlag(bool bGetting)
{
	m_bIsGettingHisData = bGetting;
}

bool CPublic::GetRequestHisDataFlag()
{
	return m_bIsGettingHisData;
}

//判断矩形是否有效
BOOL CPublic::IsRectValid(CRect rect)
{
	//判断矩形是否为空。空的矩形的宽和高都为0
	if (rect.IsRectEmpty()) 
		return FALSE;
	
	//判断矩形的top、bottom、left和right成员变量是否全都为0
	if (rect.IsRectNull()) 
		return FALSE;
	
	if (rect.right - rect.left <= 0)
		return FALSE;
	
	if (rect.bottom - rect.top <= 0) 
		return FALSE;
	
	return TRUE;
}

BOOL CPublic::IsSameWeek(unsigned int uiRefDate,unsigned int uiCurDate)
{//比较今日和所给定日期是否在同一周内
	tm	stPrevTime,stNowTime;
	memset(&stPrevTime,0,sizeof(tm));
	stPrevTime.tm_year=uiCurDate/10000-1900;
	stPrevTime.tm_mon=uiCurDate%10000/100-1;
	stPrevTime.tm_mday=uiCurDate%100;
	time_t	stPrev=mktime(&stPrevTime);
	
	memset(&stNowTime,0,sizeof(tm));
	stNowTime.tm_year=uiRefDate/10000-1900;
	stNowTime.tm_mon=uiRefDate%10000/100-1;
	stNowTime.tm_mday=uiRefDate%100;
	time_t	stNow=mktime(&stNowTime);
	
	if(stNow>stPrev && difftime(stNow,stPrev)>=7*24*3600)
		return	FALSE;
	else if(stPrev>stNow && difftime(stPrev,stNow)>=7*24*3600)
		return	FALSE;
	if(stNowTime.tm_wday<stPrevTime.tm_wday && stNow>stPrev)
		return	FALSE;
	else if(stNowTime.tm_wday>stPrevTime.tm_wday && stNow<stPrev)
		return	FALSE;
	return	TRUE;
}

BOOL CPublic:: IsSameMonth(unsigned int uiRefDate,unsigned int uiCurDate)
{//比较今日和所给定日期是否在同一月内
	unsigned char	ucNowMonth=uiRefDate%10000/100;
	unsigned char	ucPrevMonth=uiCurDate%10000/100;
	if(uiRefDate/10000 != uiCurDate/10000)
		return	FALSE;
	if(ucNowMonth!=ucPrevMonth)
		return	FALSE;
	return	TRUE;
}

bool CPublic::InitBalanceServer()
{
	if((m_nSelServer <0)&&(m_nSelServer >= m_vecServerList.size()))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return false;
	}
	struct StuCustomLoginInfo
	{//客户端发送信息
		char szTradeServer[16];
		short sTradePort;
		char szQuoteServer[16];
		short sQuotePort;
		char szTradeServer1[16];
		short sTradePort1;
		char szQuoteServer1[16];
		short sQuotePort1;
		StuCustomLoginInfo()
		{
			memset(this,0,sizeof(this));
		}
	};

	struct StuCustomLoginInfoNew
	{//客户端发送信息
		char szTradeServer[16];
		short sTradePort;
		char szQuoteServer[16];
		short sQuotePort;

		char szTradeServer1[16];
		short sTradePort1;
		char szQuoteServer1[16];
		short sQuotePort1;

		char szTradeServer2[16];
		short sTradePort2;
		char szQuoteServer2[16];
		short sQuotePort2;

		char szTradeServer3[16];
		short sTradePort3;
		char szQuoteServer3[16];
		short sQuotePort3;

		char szTradeServer4[16];
		short sTradePort4;
		char szQuoteServer4[16];
		short sQuotePort4;

		StuCustomLoginInfoNew()
		{
			memset(this,0,sizeof(this));
		}
	};


	m_strQuoteIP = "";
	m_nQuotePort = 0;
	m_strTradeIP = "";
	m_nTradePort = 0;
	//备份服务器
	m_strQuoteIP1 = "";
	m_nQuotePort1 = 0;
	m_strTradeIP1 = "";
	m_nTradePort1 = 0;

	//备份服务器2
	m_strQuoteIP2 = "";
	m_nQuotePort2 = 0;
	m_strTradeIP2 = "";
	m_nTradePort2 = 0;

	//备份服务器3
	m_strQuoteIP3 = "";
	m_nQuotePort3 = 0;
	m_strTradeIP3 = "";
	m_nTradePort3 = 0;

	//备份服务器4
	m_strQuoteIP4 = "";
	m_nQuotePort4 = 0;
	m_strTradeIP4 = "";
	m_nTradePort4 = 0;

	CString	 strServerIp;
	int nServerPort = 0;
	char cServerType;
	strServerIp = m_vecServerList[m_nSelServer].strServerUrl;
	nServerPort = m_vecServerList[m_nSelServer].nServerPort;
	cServerType = m_vecServerList[m_nSelServer].nServerType;

	char* szDataBuffer = NULL;
	if(!m_QuoteDataSource.GetBalanceServer((char*)(LPCTSTR)strServerIp,nServerPort,cServerType,&szDataBuffer))
	{	
		return false;
	}
	if(NULL == szDataBuffer)return false;

	int nDataLen = 0;
	memcpy(&nDataLen,szDataBuffer,4);

	if (nDataLen == sizeof(StuCustomLoginInfo))
	{
		StuCustomLoginInfo* pRecv = (StuCustomLoginInfo*)(szDataBuffer+4);
		m_strQuoteIP = pRecv->szQuoteServer;
		m_nQuotePort = pRecv->sQuotePort;
		m_strTradeIP = pRecv->szTradeServer;
		m_nTradePort = pRecv->sTradePort;
		//备份服务器
		m_strQuoteIP1 = pRecv->szQuoteServer1;
		m_nQuotePort1 = pRecv->sQuotePort1;
		m_strTradeIP1 = pRecv->szTradeServer1;
		m_nTradePort1 = pRecv->sTradePort1;
	}
	else
	{
		StuCustomLoginInfoNew * pRecv = (StuCustomLoginInfoNew*)(szDataBuffer+4);
		m_strQuoteIP = pRecv->szQuoteServer;
		m_nQuotePort = pRecv->sQuotePort;
		m_strTradeIP = pRecv->szTradeServer;
		m_nTradePort = pRecv->sTradePort;
		//备份服务器
		m_strQuoteIP1 = pRecv->szQuoteServer1;
		m_nQuotePort1 = pRecv->sQuotePort1;
		m_strTradeIP1 = pRecv->szTradeServer1;
		m_nTradePort1 = pRecv->sTradePort1;

		//备份服务器2
		m_strQuoteIP2 = pRecv->szQuoteServer2;
		m_nQuotePort2 = pRecv->sQuotePort2;
		m_strTradeIP2 = pRecv->szTradeServer2;
		m_nTradePort2 = pRecv->sTradePort2;

		//备份服务器3
		m_strQuoteIP3 = pRecv->szQuoteServer3;
		m_nQuotePort3 = pRecv->sQuotePort3;
		m_strTradeIP3 = pRecv->szTradeServer3;
		m_nTradePort3 = pRecv->sTradePort3;

		//备份服务器4
		m_strQuoteIP4 = pRecv->szQuoteServer4;
		m_nQuotePort4 = pRecv->sQuotePort4;
		m_strTradeIP4 = pRecv->szTradeServer4;
		m_nTradePort4 = pRecv->sTradePort4;
	}
	m_QuoteDataSource.FreeBuffer(szDataBuffer);
	return true;
}

int CPublic::InitTrade(HWND hWnd)
{	
//	TRACE("CURSEL:%s,%d\r\n%s,%d\r\n%s,%d\r\n%s,%d\r\n%s,%d\r\n",
//				(char*)(LPCTSTR)m_strTradeIP,m_nTradePort,
//		(char*)(LPCTSTR)m_strTradeIP1,m_nTradePort1,
//		(char*)(LPCTSTR)m_strTradeIP2,m_nTradePort2,
//		(char*)(LPCTSTR)m_strTradeIP3,m_nTradePort3,
//		(char*)(LPCTSTR)m_strTradeIP4,m_nTradePort4);
	m_strUserName.Trim();
	return m_dllTrade.m_pLink2Server(hWnd,WM_TRADELOGININFO,
		(char*)(LPCTSTR)m_strUserName,(char*)(LPCTSTR)m_strPassword,
		(char*)(LPCTSTR)m_strTradeIP,m_nTradePort,
		(char*)(LPCTSTR)m_strTradeIP1,m_nTradePort1,
		(char*)(LPCTSTR)m_strTradeIP2,m_nTradePort2,
		(char*)(LPCTSTR)m_strTradeIP3,m_nTradePort3,
		(char*)(LPCTSTR)m_strTradeIP4,m_nTradePort4,
		m_bNeedCFCA);
}

int CPublic::InitQuote(HWND hWnd)
{
	return m_QuoteDataSource.Link((char*)(LPCTSTR)m_strQuoteIP,m_nQuotePort
		,(char*)(LPCTSTR)m_strQuoteIP1,m_nQuotePort1
		,(char*)(LPCTSTR)m_strQuoteIP2,m_nQuotePort2
		,(char*)(LPCTSTR)m_strQuoteIP3,m_nQuotePort3
		,(char*)(LPCTSTR)m_strQuoteIP4,m_nQuotePort4
		,(unsigned int )hWnd,WM_DATAARRIVE);
}

void CPublic::UnLinkTrade()
{
	m_dllTrade.m_pUnLink();
}

void CPublic::UnLinkQuote()
{
	m_QuoteDataSource.UnLink();
}
void CPublic::WriteSkinType(int iSkinType)
{
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (iSkinType >= 0)
	{
		inifile.SetVarInt(CString("DEFAULT"), CString("SKINTYPE"),iSkinType);
		m_iSkinType = iSkinType;
	}
}
void CPublic::WritePriceHight(int iHight)
{
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (iHight > 0)
	{
		inifile.SetVarInt(CString("DEFAULT"), CString("PRICEHIGHT"),iHight);
	}
}
void CPublic::WriteTrendChartRatio(int iRatio)
{
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (iRatio > 0)
	{
		inifile.SetVarInt(CString("DEFAULT"), CString("TRENDCHARTRATIO"),iRatio);
	}
}
void CPublic::WriteQuoteLstItemWith(int iItem,int iItemWith)
{
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (iItemWith >= 0&&iItem>=0)
	{
		CString strTmp;
		strTmp.Format(_T("QUOTELSTITEMW%d"),iItem);
		inifile.SetVarInt(CString("OTHER"), strTmp,iItemWith);
	}
}
void CPublic::WriteIniInfo(CString strUserName,CString strPwd,BOOL bSavePwd,BOOL bSaveAccount,BOOL bNeedCFCA)
{
	m_strUserName = strUserName;
	m_strUserName.Trim();
	m_strPassword = strPwd;
	m_bSavePassword = bSavePwd;
	m_bSaveAccount = bSaveAccount;
	m_bNeedCFCA = bNeedCFCA;
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}
	inifile.SetVarInt(CString("USERINFO"),CString("SavePwd"),m_bSavePassword);
	inifile.SetVarInt(CString("USERINFO"),CString("SaveAccount"),m_bSaveAccount);

	inifile.SetVarInt(CString("USERINFO"),CString("UseCFCA"),m_bNeedCFCA);

	if(bSaveAccount)
	{
		inifile.SetVarStr(CString("USERINFO"),CString("UserID"),CString(m_strUserName));
	}
	else
	{
		inifile.SetVarStr(CString("USERINFO"),CString("UserID"),CString(""));
	}
	if(bSavePwd)
	{
		inifile.SetVarStr(CString("USERINFO"),CString("UserPwd"),CString(m_strPassword));
	}
	else
	{
		inifile.SetVarStr(CString("USERINFO"),CString("UserPwd"),CString(""));
	}
	inifile.SetVarInt(CString("BALANCE"),CString("DEFAULTSERVER"),m_nSelServer);
}

//读取配置文件
void CPublic::ReadIniInfo()
{
	//读取行情的地址和端口
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}

	CString strPort;
	inifile.GetVarStr(CString("USERINFO"),CString("UserID"),m_strUserName);
	m_strUserName.Trim();
	inifile.GetVarStr(CString("USERINFO"),CString("UserPwd"),m_strPassword);
	inifile.GetVarInt(CString("USERINFO"),CString("SavePwd"),m_bSavePassword);
	inifile.GetVarInt(CString("USERINFO"),CString("SaveAccount"),m_bSaveAccount);
	inifile.GetVarInt(CString("USERINFO"),CString("UseCFCA"),m_bNeedCFCA);

	int nServerNum;
	inifile.GetVarInt(CString("BALANCE"),CString("SERVERNUM"),nServerNum);
	inifile.GetVarInt(CString("BALANCE"),CString("DEFAULTSERVER"),m_nSelServer);

	if(nServerNum > 100)
		nServerNum = 100;
	StuServerInfo serverinfo;
	m_vecServerList.clear();
	CString ssTitleInfo;
	for(int i= 1;i<=nServerNum;i++)
	{
		ssTitleInfo.Format(_T("SERVERNAME%d"),i);
		inifile.GetVarStr(CString("BALANCE"),ssTitleInfo,serverinfo.strServerName);
		ssTitleInfo.Format(_T("SERVERIP%d"),i);
		inifile.GetVarStr(CString("BALANCE"),ssTitleInfo,serverinfo.strServerUrl);
		ssTitleInfo.Format(_T("SERVERPORT%d"),i);
		inifile.GetVarInt(CString("BALANCE"),ssTitleInfo,serverinfo.nServerPort);
		ssTitleInfo.Format(_T("SERVERTYPE%d"),i);
		inifile.GetVarInt(CString("BALANCE"),ssTitleInfo,serverinfo.nServerType);
		m_vecServerList.push_back(serverinfo);
	}
	inifile.GetVarInt(CString("SYSTEM"),CString("SHOWMINUTEDATA"),m_bIsMinuteMode);
	inifile.GetVarInt(CString("SYSTEM"),CString("NEEDKEYGHOST"),m_nNeedKeyGhost);
	inifile.GetVarStr(CString("SYSTEM"),CString("SYSTEMTITLE"),m_strProductName);
	inifile.GetVarStr(CString("SYSTEM"),CString("REGISTLINK"),m_strDemoLink);
	inifile.GetVarInt(CString("SYSTEM"),CString("LANGUAGETYPE"),m_nLanguageType);
	inifile.GetVarInt(CString("SYSTEM"),CString("SHOWWARNINFO"),m_nShowWarningInfo);
	inifile.GetVarInt(CString("SYSTEM"),CString("LOGINMODE"),m_nLoginMode);
	inifile.GetVarInt(CString("SYSTEM"),CString("MAINVIEWMODE"),m_nMainViewMode);
	m_IsTrendChart = m_nMainViewMode;
	m_nMainViewMode = 0;
	inifile.GetVarInt(CString("SYSTEM"),CString("MONEYINOUTENABLE"),m_nMoneyInOutEnable);
	inifile.GetVarStr(CString("SYSTEM"),CString("HOWTOUSE"),m_strHow2UsePage);
	inifile.GetVarInt(CString("SYSTEM"),CString("SUPPORTCFCA"),m_bSupportCFCA);
	inifile.GetVarStr(CString("SYSTEM"),CString("EDUCATION"),m_strEducation);
	inifile.GetVarStr(CString("MENU"),  CString("APPENDMENU"),m_strAppendMenu);
	inifile.GetVarStr(CString("MENU"),  CString("APPENDMENULINK"),m_strAppendMenuLink);

	inifile.GetVarInt(CString("UPDATE"), CString("UPDATEPORT"),m_nUpdateServerPort);
	inifile.GetVarStr(CString("UPDATE"), CString("UPDATESERVER"),m_strUpdateServer);
	inifile.GetVarStr(CString("UPDATE"), CString("UPDATETAG"),m_strUpdateTag);
	m_iUpdateTmOut = 20000;
	inifile.GetVarInt(CString("UPDATE"), CString("UPDATETMOUT"),m_iUpdateTmOut);

	//m_iClientType=0;
	//inifile.GetVarInt(CString("SYSTEM"), CString("CLIENTTYP"),m_iClientType);
	m_iFundPos = 0;
	inifile.GetVarInt(CString("SYSTEM"), CString("FUNDPOS"),m_iFundPos);

	CString strTmp;
	for (int i=0; i < 7;i++)
	{
		m_iQuoteLstItem[i]=100;
		strTmp.Format(_T("QUOTELSTITEMW%d"),i);
		inifile.GetVarInt(CString("OTHER"), strTmp,m_iQuoteLstItem[i]);
	}

	m_iTrendChart = 650;
	inifile.GetVarInt(CString("DEFAULT"), CString("TRENDCHARTRATIO"),m_iTrendChart);
	if (m_iTrendChart<15||m_iTrendChart>5000)
	{
		m_iTrendChart = 650;
	}

	m_iPriceHight = 300;
	inifile.GetVarInt(CString("DEFAULT"), CString("PRICEHIGHT"),m_iPriceHight);
	if (m_iPriceHight<15||m_iPriceHight>2000)
	{
		m_iTrendChart = 300;
	}

	m_iSkinType = 1;
	inifile.GetVarInt(CString("DEFAULT"), CString("SKINTYPE"),m_iSkinType);

	m_iDPUnTrade = 1;
	inifile.GetVarInt(CString("DEFAULT"), CString("DPUNTRADE"),m_iDPUnTrade);

	m_bIsClear = FALSE;
	inifile.GetVarInt(CString("DEFAULT"), CString("ISCLEAR"),m_bIsClear);

	m_nShowEducationMenu = 0;
	inifile.GetVarInt(CString("DEFAULT"), CString("INVESTEDUCATIONMENU"), m_nShowEducationMenu);
	m_nShowUseHelpMenu = 0;
	inifile.GetVarInt(CString("DEFAULT"), CString("USEHELPMENU"), m_nShowUseHelpMenu);

	m_iIsNeedLock = 0;
	m_bLock = 0;
	inifile.GetVarInt(CString("SYSTEM"), CString("ISNEEDLOCK"),m_iIsNeedLock);

	m_iUseLgOld = 0;
	inifile.GetVarInt(CString("INTERFACE"), CString("USLGOLD"),m_iUseLgOld);

	CString strUrl = _T("");
	m_mapUrl.clear();
	inifile.GetVarStr(CString("URL"), CString("F8ZYQ"),strUrl);
	m_mapUrl[VK_F8] = strUrl;
	strUrl.Empty();
	inifile.GetVarStr(CString("URL"), CString("F10ZYQ"),strUrl);
	m_mapUrl[VK_F10] = strUrl;

	m_bIsShowMall = 0;
	m_strMallUrl.Empty();
	inifile.GetVarInt(CString("SYSTEM"), CString("ISSHOWMALL"),m_bIsShowMall);
	inifile.GetVarStr(CString("SYSTEM"), CString("MALLURL"),m_strMallUrl);

	int iCusType = -1;
	inifile.GetVarInt(CString("CUSTOM"), CString("CustomType"),iCusType);
	if (iCusType == 1)
	{
		CCustomUser_Type1 *pCusUser = new CCustomUser_Type1;
		if (pCusUser)
		{
			pCusUser->m_iType = iCusType;
			inifile.GetVarStr(CString("CUSTOM"),  CString("CustomMenuText"),pCusUser->m_strCustomMenuText);
        	inifile.GetVarStr(CString("CUSTOM"),  CString("CustomTips"),pCusUser->m_strCustomTips);
			m_pCusUser = pCusUser;
		}
	}

	m_iInitiAtiveShowWarn = 0;
	inifile.GetVarInt(CString("SYSTEM"), CString("INITIATIVESHOWWARN"),m_iInitiAtiveShowWarn);

	//add by witming 
	inifile.GetVarStr(CString("TOPTOOLBAR"), CString("MAILNAME"), m_strMallName);
	inifile.GetVarColor(CString("TOPTOOLBAR"), CString("FONTCOLOR"), m_crMallFontColor);
	//end by witming 


}
CString CPublic::GetURL(UINT uiKey)
{
	CString strRet = _T("");
	std::map<UINT,CString>::iterator itMap;
	itMap = m_mapUrl.find(uiKey);
	if (itMap != m_mapUrl.end())
	{
		strRet = itMap->second;
	}
	return strRet;
}
void CPublic::GetServerNameList(vector<CString>& vecServer)
{
	vecServer.clear();
	vector<StuServerInfo>::iterator it;
	for(it = m_vecServerList.begin();it != m_vecServerList.end();it++)
	{
		vecServer.push_back(it->strServerName);
	}
}

void  CPublic::GetLoginInfo(CString& strUserName,CString& strPwd,BOOL& bSavePwd,BOOL& bSaveAccount,BOOL& bNeedCFCA,BOOL& bSupportCFCA)
{
	m_strUserName.Trim();
	strUserName = m_strUserName;
	//strPwd = m_strPassword;
	bSavePwd = m_bSavePassword;
	bSaveAccount = m_bSaveAccount;
	bNeedCFCA = m_bNeedCFCA;
	bSupportCFCA = m_bSupportCFCA;
}
void CPublic::NotifyWnd(const char* lpData,int nDataLen,UINT iNotifyMode,UINT iNotifyWndType)
{
	std::map<UINT,vector<SViewInfo>>::iterator itMap;
	itMap = m_mapNotifyWnd.find(iNotifyMode);
	if (itMap == m_mapNotifyWnd.end())return;
	vector<SViewInfo>::iterator itVec;
	vector<SViewInfo> vecNotifyWnd = itMap->second;
	for (itVec =vecNotifyWnd.begin();itVec != vecNotifyWnd.end();itVec++)
	{
		if (iNotifyWndType != 0)
		{
			if (iNotifyWndType == itVec->nViewType)
			{
				if(itVec->pWnd&&IsWindow(itVec->pWnd->GetSafeHwnd()))
					itVec->pWnd->SendMessage(WM_DATAARRIVE,(WPARAM)lpData,nDataLen);
				break;
			}
		}
		else
		{
			if(itVec->pWnd&&IsWindow(itVec->pWnd->GetSafeHwnd()))
				itVec->pWnd->SendMessage(WM_DATAARRIVE,(WPARAM)lpData,nDataLen);
		}
	}
}

void CPublic::NotifyWnd(UINT iNotifyMode,int iTypeMsg)
{
	std::map<UINT,vector<SViewInfo>>::iterator itMap;
	itMap = m_mapNotifyWnd.find(iNotifyMode);
	if (itMap == m_mapNotifyWnd.end())return;
	vector<SViewInfo>::iterator itVec;
	vector<SViewInfo> vecNotifyWnd = itMap->second;
	for (itVec =vecNotifyWnd.begin();itVec != vecNotifyWnd.end();itVec++)
	{
		if(itVec->pWnd&&IsWindow(itVec->pWnd->GetSafeHwnd()))
			itVec->pWnd->SendMessage(WM_DATAARRIVE,(WPARAM)iTypeMsg,0);
	}
}
//函数功能：处理更新数据
//函数参数：lpData 接收到数据
//函数返回：无
void CPublic::UpdateData(const char* lpData,int nDataLen,UINT iNotifyMode)
{
	SNormaHead* pDeataHead = (SNormaHead*)lpData; 

	//if(pDeataHead->m_usType == HISKDATA)
	//{
		//CTime time = CTime::GetCurrentTime();
		//TRACE("服务器返回历史数据 Index = %d  %02d:%02d:%02d\r\n", pDeataHead->m_usIndex, time.GetHour(), time.GetMinute(), time.GetSecond()); 
	//}

	if(pDeataHead->m_usType == INITMARKET)
	{//初始化数据
		InitialStockInfo(lpData);
	}

	if (pDeataHead->m_usType == HISKDATA)
		CPublic::Instance()->SetRequestHisDataFlag(false);

	if((pDeataHead->m_usType == HISKDATA)
		&&(pDeataHead->m_usIndex != GetRequestIndex()))
	{
		TRACE("无效数据 抛弃之 m_usIndex = %d\r\n", pDeataHead->m_usIndex);
		return;//无效数据 抛弃之
	}
	/////
	NotifyWnd(lpData,nDataLen,iNotifyMode);
	
	//////
//	vector<SViewInfo>::iterator it;
////	char * pTempData;
//	for(it = m_vecViewInfo.begin();it != m_vecViewInfo.end();it++)
//	{
//// 		if(it->pWnd)
//// 		{
//// 			pTempData = new char[nDataLen];
//// 			memcpy(pTempData,lpData,nDataLen);
//// 			it->pWnd->PostMessage(WM_DATAARRIVE,(WPARAM)pTempData,nDataLen);
//// 		}
//		if(it->pWnd)it->pWnd->SendMessage(WM_DATAARRIVE,(WPARAM)lpData,nDataLen);
//	}
}
const SCodeInfo* CPublic::GetCurStock()
{
	return &m_oCurrentCode;
}

void CPublic::ChangeStockEvent(const SCodeInfo* pCodeInfo,UINT uiInPage)
{	
	if(NULL ==  pCodeInfo)
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10005),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}

	memcpy(&m_oCurrentCode,pCodeInfo,sizeof(SCodeInfo));
	const CWnd* pActivateWnd = GetView(10/*VIEWINFO_STOCK*/);
	CDlgTrendChart* pView = (CDlgTrendChart*)pActivateWnd;
	if(NULL == pView)return;
	SCodeInfo* pCode= pView->GetCurStock();
	INT	uiPage = pView->GetAnalisysMode();
	if((*pCode == *pCodeInfo)&&(uiPage == uiInPage))
	{//代码相同
		return ;
	}
	const SCodeInfo* pSendCode = &GetStockInfo(pCodeInfo)->sCode;

	if(pActivateWnd)
		pActivateWnd->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode,uiInPage);

	pActivateWnd = GetView(VIEWINFO_STOCK);
	CStockView* pStockView = (CStockView*)pActivateWnd;
	if(NULL == pStockView)return;
	pCode= pStockView->GetCurStock();
	uiPage = pStockView->GetAnalisysMode();
	if((*pCode == *pCodeInfo)&&(uiPage == uiInPage))
	{//代码相同
		return ;
	}

	pSendCode = &GetStockInfo(pCodeInfo)->sCode;

	if(pActivateWnd)
		pActivateWnd->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode,uiInPage);
}

void CPublic::OnFormulaCommand(char *pCommand)
{
	//CStockView* pView = (CStockView*)GetView();
	//if(pView)pView->SetFormulaCommand(pCommand);

	CDlgTrendChart* pView = (CDlgTrendChart*)(CPublic::Instance()->GetView(10));
	if (pView)
	{
		pView->SetFormulaCommand(pCommand);
	}
}

int CPublic::GetCustomUnit(const char* pCode,float& fBuyUnit,float& fSellUnit,int& nSort)
{
	float fBuy,fSell;
	fBuy= fSell = 0;
	
	m_dllTrade.m_pGetCustomUnit(pCode,fBuy,fSell,nSort);
	fBuyUnit = fBuy;
	fSellUnit = fSell;
	return 0;
}

//快速交易
int CPublic::CallQuickTrade(const SCodeInfo* pCode,int nMode,int nType,float fPrice)
{
	const StuSymbolData* pSymbolInfo;
	pSymbolInfo = GetStockInfo(pCode);
	if (pSymbolInfo == NULL)return 0;
	if((pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT))//&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
	{
		nMode = (nType<<1)+nMode;
		if(0 == m_dllTrade.m_pQuickOrder(nMode,pCode->szCode,fPrice,0))
		{
			MessageBox(GetView(7)->GetSafeHwnd(),CLanguageInterface::Instance()->GetStr(20067),CLanguageInterface::Instance()->GetStr(20068),MB_OK|MB_ICONWARNING);
		}
	}
	return 0;
}

void CPublic::RegistWnd(int nViewType,CWnd* pWnd,UINT iNotifyMode)
{
	SViewInfo viewinfo;
	viewinfo.nViewType = nViewType;
	viewinfo.pWnd = pWnd;
	//m_vecViewInfo.push_back(viewinfo);
	m_mapNotifyWnd[iNotifyMode].push_back(viewinfo);
}

void CPublic::UnRegistWnd(int nViewType,CWnd* pWnd,UINT iNotifyMode)
{	
	////////
	std::map<UINT,vector<SViewInfo>>::iterator itMap;
	itMap = m_mapNotifyWnd.find(iNotifyMode);
	if (itMap == m_mapNotifyWnd.end())return;

	vector<SViewInfo>::iterator itVec;
	vector<SViewInfo> &vecNotifyWnd = itMap->second;
	for (itVec =vecNotifyWnd.begin();itVec != vecNotifyWnd.end();itVec++)
	{
		if((itVec->nViewType == nViewType)&&(itVec->pWnd == pWnd))
		{
			itMap->second.erase(itVec);
			return;
		}
	}
	/////////////
	//vector<SViewInfo>::iterator it;
	//for(it = m_vecViewInfo.begin();it != m_vecViewInfo.end();it++)
	//{
	//	if((it->nViewType == nViewType)&&(it->pWnd == pWnd))
	//	{
	//		m_vecViewInfo.erase(it);
	//		return;
	//	}
	//}
}

void CPublic::CallStockMenu(UINT uID,const SCodeInfo* pCode)
{
	if(ID_SYSTEM_NEWORDER == uID)
	{
		CallQuickTrade(pCode);
	}
	if(ID_QUOTE_ANALYSISTPIC  == uID)
	{
		ChangeStockEvent(pCode,TechObj);
	}
	if(ID_QUOTE_REALRUNPIC == uID)
	{
		ChangeStockEvent(pCode,RealObj);
	}
}

CWnd*	CPublic::GetView(int nType,UINT iNotifyMode)
{
	CWnd* pActivateWnd = NULL;
	//return pActivateWnd;
//	pActivateWnd = ((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	////////
	std::map<UINT,vector<SViewInfo>>::iterator itMap;
	itMap = m_mapNotifyWnd.find(iNotifyMode);
	if (itMap == m_mapNotifyWnd.end())return pActivateWnd;

	vector<SViewInfo>::iterator itVec;
	vector<SViewInfo> vecNotifyWnd = itMap->second;
	for (itVec =vecNotifyWnd.begin();itVec != vecNotifyWnd.end();itVec++)
	{
		if(itVec->nViewType == nType)
		{
			pActivateWnd = (CWnd*) itVec->pWnd;
			break;
		}
	}
	/////////////
////	if(NULL == pActivateWnd)
//	{
//		vector<SViewInfo>::iterator it;
//		for(it = m_vecViewInfo.begin();it != m_vecViewInfo.end();it++)
//		{
//			if(it->nViewType == nType)
//			{
//				pActivateWnd =(CView*) it->pWnd;
//				break;
//			}
//		}		
//	}
	return pActivateWnd;
}

BOOL CPublic::ConfirmPwd(CString strPwd)
{
	return m_strPassword ==strPwd;
}

void CPublic::ChangePwd(CString strPwd)
{
	m_strPassword = strPwd;
	CString strIniPath;
	strIniPath = m_szSysPath+_T("\\koa.ini");
	CIniFile inifile;
	if(!inifile.Create(strIniPath))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10004),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}
	inifile.SetVarInt(_T("USERINFO"),_T("SavePwd"),0);
	inifile.SetVarStr(_T("USERINFO"),_T("UserPwd"),_T(""));
}

void	CPublic::GetUpdateServer(CString& strServer,WORD& wServerPort,CString& strTag,DWORD &dwOutTm)
{
	if(!m_strUpdateServer.IsEmpty())
	{
		strServer = m_strUpdateServer;
		wServerPort = m_nUpdateServerPort;
		strTag = m_strUpdateTag;
		dwOutTm = m_iUpdateTmOut;
	}
	else
	{
		char szLink[1024];
		memset(szLink,0,sizeof(szLink));
		if(0 == m_dllTrade.m_pGetUpdateLink(szLink))return;
		if(0 == strlen(szLink))return;//接受错误
		char szServer[1024];
		char szTag[100];
		char szPort[8];
		memset(szServer,0,sizeof(szServer));
		memset(szTag,0,sizeof(szTag));
		memset(szPort,0,sizeof(szPort));
		sscanf(szLink,"%127[^:]:%7[^/]/%19[^ ]",szServer,&szPort,szTag);
		strServer = szServer;
		wServerPort = atoi(szPort);
		strTag = szTag;
		dwOutTm = m_iUpdateTmOut;
	}
}

CString	CPublic::GetSoftwareVersion()
{
	CStdioFile versionFile;
	CString strPath;
	strPath = GetSysPath()+"version.txt";
	CString strVersion;
	if(versionFile.Open(strPath,CFile::modeRead|CFile::typeText))
	{
		versionFile.ReadString(strVersion);
		versionFile.Close();
	}
	return strVersion;
}

BOOL CPublic::SendLinkStatus(int iServerId,BOOL bLink)
{
	if (!IsWindow(m_pStatusWnd->GetSafeHwnd()))return FALSE;

	::SendMessage(m_pStatusWnd->GetSafeHwnd(),WM_UPDATE_LINKSTATUS,iServerId,bLink);

	return TRUE;
}

BOOL CPublic::SendUserSGRNChangeMsg(BOOL bChange)
{
	if (!IsWindow(m_pStatusWnd->GetSafeHwnd()))return FALSE;

	::PostMessage(m_pStatusWnd->GetSafeHwnd(),WM_UPDATE_SGRN,0,bChange);

	return TRUE;
}
BOOL CPublic::SendUserChangeMsg(BOOL bChange)
{
	//if (!IsWindow(m_pToolBarWnd->GetSafeHwnd()))return FALSE;

	//::PostMessage(m_pToolBarWnd->GetSafeHwnd(),WM_UPDATE_USERINFO,0,bChange);

	if (!IsWindow(m_pStatusWnd->GetSafeHwnd()))return FALSE;

	::PostMessage(m_pStatusWnd->GetSafeHwnd(),WM_UPDATE_USERINFO,0,bChange);

	return TRUE;
}
int CPublic::GetClientType()
{
	int iAccessCtrl = GetTradeDll()->m_pGetAccessCtrl();
	//if ((iAccessCtrl&0x1000)&&(iAccessCtrl&0x0100))m_iClientType = 2;
	//else if (!(iAccessCtrl&0x1000)&&(iAccessCtrl&0x0100))m_iClientType = 2;
	//else if ((iAccessCtrl&0x1000)&&!(iAccessCtrl&0x0100))m_iClientType = 0;
	m_iClientType = iAccessCtrl;
	return m_iClientType;
}
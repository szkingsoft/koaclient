#include "StdAfx.h"
#include "DateManage.h"


CDateManage* CDateManage::_instance = NULL;
int CDateManage::g_iProtocolVersion = V1;
CDateManage::CDateManage(void)
{
	Init();
}
void CDateManage::Init()
{
	m_pPublic = CPublic::Instance();
	m_sRequestIndex = 1;
	m_mapNotifyWnd.clear();
	m_vecSymbol.clear();
	m_mapQuotationLst.clear();
	//m_mapRealQuoteInfo.clear();
	m_stcQuoteInfo.Init();

	TCHAR exeFullPath[MAX_PATH]; 
	CString strPath;
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strPath = exeFullPath;
	int pos = strPath.ReverseFind(('\\'));
	CString strImgDirc = strPath.Left(pos);
	m_strImgPath = strImgDirc +  _T("\\interface\\");

	m_ImgBottomHide = m_ImgTopHide = m_ImgLeftide = m_ImgRightHide = NULL;
	InitImg();

	m_iMarketType = 0;
	AddTestDate();

	m_clrFill = Color::MakeARGB(255,255,255,255);
	m_mapSkin.clear();
	m_oCurrentCode = NULL;
	SKININFO stSkin;
	stSkin.iId = 0;
	stSkin.strSkinName = _T("深黑");
	m_mapSkin[0] = stSkin;
	stSkin.iId = 1;
	stSkin.strSkinName = _T("银白");
	m_mapSkin[1] = stSkin;
	stSkin.iId = 2;
	stSkin.strSkinName = _T("雪白");
	m_mapSkin[2] = stSkin;
}
CDateManage::~CDateManage(void)
{
	m_stcQuoteInfo.Clear();
}

void CDateManage::ExitInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;
	}
	CSkinTransform::ExitInstance();
}

CDateManage* CDateManage::Instance()
{
	if(!_instance)
	{
		_instance = new CDateManage;
	}
	return _instance;
}

short CDateManage::AddRequestIndex()
{ 
	m_sRequestIndex++;
	return m_sRequestIndex;  
}

short CDateManage::GetRequestIndex() 
{ 
	return m_sRequestIndex;
}

void CDateManage::ReqInitMarket_Sp()
{
	int nDataLen;
	nDataLen = sizeof(SNormaHead);
	char * pData = new char[nDataLen];
	SNormaHead* pHead = (SNormaHead*)(pData);
	pHead->m_usIndex = GetRequestIndex();
	pHead->m_usSize = 0;
	pHead->m_usType = INITMARKET_ZYQ;
	m_pPublic->SendRequestData(pData,nDataLen);
	delete []pData;
}

void CDateManage::ReqStockInfo_SP()
{
	return;
	const STUSymbolInfo* pSendCode;
	pSendCode = GetDefaultStock_SP();
	if(NULL == pSendCode)return;
	memcpy(&m_oCurrentCode,pSendCode,sizeof(SCodeInfo));
	CWnd* pView;
	pView = CPublic::Instance()->GetView(12,2);
	if(pView)
	{
		pView->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode,RealObj);
	}
}
void CDateManage::RequestData_SP(SCodeInfo *pCodeInfo)
{
	if(pCodeInfo == NULL)
		return;
	int iBufferSize;
	iBufferSize = sizeof(SCodeInfo)+sizeof(SNormaHead); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	{
		pRrequest->m_usType = STKMINUTEDATA_ZYQ; 
	}
	pRrequest->m_usIndex = GetRequestIndex();
	pRrequest->m_usSize = 1;

	SCodeInfo *pCode = (SCodeInfo *)(pRrequest+1);
	memcpy(pCode,pCodeInfo,sizeof(SCodeInfo));
	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}
const STUSymbolInfo* CDateManage::GetDefaultStock_SP()
{
	const STUSymbolInfo* pCode = NULL;
	if(m_ssDefaultCode.IsEmpty())
	{
		if(m_vecSymbol.size())
		{
			for(int i = 0; i < m_vecSymbol.size();i++)
			{
				if (m_vecSymbol[i].m_oSymbolInfo.m_szSymbol[0] != 0)
				{
					pCode = &m_vecSymbol[i].m_oSymbolInfo;
					break;
				}
			}
			if (pCode && pCode->m_szSymbol)
			{
				m_ssDefaultCode = pCode->m_szSymbol;
			}
		}		
	}
	else
	{
		STUSymbolInfo code;
		strcpy(code.m_szSymbol,m_ssDefaultCode.GetBuffer(0));
		m_ssDefaultCode.ReleaseBuffer(0);
		pCode = &((STUSymbolDataZYQ *)(GetStockInfo(&code,2)))->m_oSymbolInfo;
	}
	return pCode;
}
void CDateManage::ReqReportData_Sp()
{
	if(CDateManage_ZYQ::Instance()->GetSymbolNum() <=0)return;
	STUReportDataIndex_ZYQ stReportDataIndex_ZYQ;
	stReportDataIndex_ZYQ.m_usMarketType = MARKET_TYPE_ALL; 
	stReportDataIndex_ZYQ.m_ucOrderBy = RPDATA_ORDER_BY_SYMOBL; 
	stReportDataIndex_ZYQ.m_ucSeqType = REDATA_SEQ_TYPE_ASC;        
	stReportDataIndex_ZYQ.m_usBeginPos = 1;           
	stReportDataIndex_ZYQ.m_usReqNum = 50;                
	stReportDataIndex_ZYQ.m_usReqDesSymoblNum = 0;     
	CDateManage_ZYQ::Instance()->ReqReportDate_ZYQ(&stReportDataIndex_ZYQ);
	return;
	if(GetSymbolNum() <=0)return;
	SNormaHead  tRequest;
	tRequest.m_usIndex = AddRequestIndex();
	tRequest.m_usSize = 0;
	tRequest.m_usType = REPORTDATA_ZYQ;
	m_pPublic->SendRequestData((char*)&tRequest,sizeof(SNormaHead));
}

void CDateManage::ReqReportData_Settle_SP()
{
	if(GetSymbolNum() <=0)return;
	SNormaHead  tRequest;
	tRequest.m_usIndex = AddRequestIndex();
	tRequest.m_usSize = 0;
	tRequest.m_usType = REPORTDATA_SETTLE_SP;
	m_pPublic->SendRequestData((char*)&tRequest,sizeof(SNormaHead));
}

int  CDateManage::GetNeedRegRealQuoSymbol(STUSSymbol * pstcSymbol)
{
	int iNum = m_vecSymbol.size();

	if (pstcSymbol == NULL || iNum <= 0) 
	{
		return iNum;
	}
	else
	{
		int i = 0;
		vector <STUSymbolDataZYQ>::iterator itVec; /*m_vecSymbolInfo*/;
		for (itVec = m_vecSymbol.begin(); itVec != m_vecSymbol.end();itVec++)
		{
			if (i < iNum)
			{
				memcpy(pstcSymbol[i].m_szSymbol,itVec->m_oSymbolInfo.m_szSymbol,CLIENT_SYMBOL_MAX_LEN+1);
			}
		}
	}
}

void CDateManage::RegRealMetalsQuotationDate()
{
	//PackSend(REPORTDATA,m_pPublic->AddRequestIndex());
	if(GetSymbolNum() <=0)return;
	STUSSymbol *pstcSymbol = NULL;
	int iNum = m_pPublic->GetNeedRegRealQuoSymbol(pstcSymbol);
	char *pSend = NULL;
	int iSendLen = 0;
	if (iNum > 0)
	{
		iSendLen = sizeof(SNormaHead)+sizeof(STUSSymbol)*iNum;
		pSend = new char(iSendLen);
		m_pPublic->GetNeedRegRealQuoSymbol((STUSSymbol*)(pSend+sizeof(SNormaHead)));
	}
	else
	{
		iSendLen = sizeof(SNormaHead);
		pSend = new char(iSendLen);
	}
	if (pSend)
	{
		((SNormaHead*)pSend)->m_usIndex = REGISTER_REALQUOTE;
		((SNormaHead*)pSend)->m_usType = m_pPublic->AddRequestIndex();
	}

	m_pPublic->SendRequestData((char*)&pSend,iSendLen);

	delete []pSend;
}

void CDateManage::RegRealSpotQuotationDate()
{
	//PackSend(REPORTDATA_SP,AddRequestIndex());
	if(GetSymbolNum() <=0)return;
	STUSSymbol *pstcSymbol = NULL;
	int iNum = GetNeedRegRealQuoSymbol(pstcSymbol);
	char *pSend = NULL;
	int iSendLen = 0;
	if (iNum > 0)
	{
		iSendLen = sizeof(SNormaHead)+sizeof(STUSSymbol)*iNum;
		pSend = new char(iSendLen);
		GetNeedRegRealQuoSymbol((STUSSymbol*)(pSend+sizeof(SNormaHead)));
	}
	else
	{
		iSendLen = sizeof(SNormaHead);
		pSend = new char(iSendLen);
	}
	if (pSend)
	{
		((SNormaHead*)pSend)->m_usIndex = REGISTER_REALQUOTE_SP;
		((SNormaHead*)pSend)->m_usType = AddRequestIndex();
	}

	m_pPublic->SendRequestData((char*)&pSend,iSendLen);

	delete []pSend;
}

void CDateManage::PackSend(int iMsgType,int MsgIndex)
{
	if(GetSymbolNum() <=0)return;
	SNormaHead  tRequest;
	tRequest.m_usIndex = MsgIndex;
	tRequest.m_usSize = 0;
	tRequest.m_usType = iMsgType;
	m_pPublic->SendRequestData((char*)&tRequest,sizeof(SNormaHead));
}

void CDateManage::SetNotifyWnd(int iWndType,CWnd *pWnd)
{
	m_mapNotifyWnd[iWndType] = pWnd;
}

void CDateManage::ManageMsg(const char* lpData,int nDataLen)
{
	CPublic::Instance()->UpdateData(lpData,nDataLen);

	AnalysisMsg(lpData,nDataLen);
}

void CDateManage::InitSymbolInfo(const char* lpData)
{
	m_vecSymbol.clear();
	const SNormaHead* pNoramlHead = (SNormaHead*)lpData;
	const STUSymbolDataZYQ* pCodeInfo =(STUSymbolDataZYQ*)(pNoramlHead+1);
	for(int nCounter = 0;nCounter<pNoramlHead->m_usSize;nCounter++)
	{
		if((pCodeInfo[nCounter].m_oSymbolInfo.m_cPriceUnit<0))continue;
		if((pCodeInfo[nCounter].m_cTrade !='0')&&(pCodeInfo[nCounter].m_cTrade !='1'))continue;
		if(strlen(pCodeInfo[nCounter].m_szName) == 0)continue;
		m_vecSymbol.push_back(pCodeInfo[nCounter]);
	}
}

void CDateManage::AnalysisMsg(const char* lpData,int nDataLen)
{
	if (nDataLen <=0)return;

	SNormaHead* pDeataHead = (SNormaHead*)lpData; 

	CDateManage_ZYQ::Instance()->AnalysisMsg(lpData,nDataLen);
	return;
	switch(pDeataHead->m_usType)
	{
	case INITMARKET_ZYQ:
		{
			InitSymbolInfo(lpData);
			CDateManage::Instance()->ReqStockInfo_SP();
		}
		break;
	case REPORTDATA_ZYQ:
		{
			if (CDateManage::g_iProtocolVersion == V1)
			{
				STUNowDataZYQ* pNow = (STUNowDataZYQ*)(pDeataHead+1);
				int nCount;
				m_lockRealQuoteLst.Lock();
				m_mapQuotationLst.clear();
				m_stcQuoteInfo.ReSetSize(pDeataHead->m_usSize);
				int i=0;
				for(nCount =0;nCount<pDeataHead->m_usSize;nCount++)
				{	
					CString strSymbol = pNow[nCount].m_oSymbolInfo.m_szSymbol;
					strSymbol.Trim();
					m_mapQuotationLst[pNow[nCount].m_oSymbolInfo.m_szSymbol] = pNow[nCount];

					memcpy(m_stcQuoteInfo.pstcQuote[i].sCode.szCode,pNow[nCount].m_oSymbolInfo.m_szSymbol,11);
					m_stcQuoteInfo.pstcQuote[i].sCode.cPriceUnit = pNow[nCount].m_oSymbolInfo.m_cPriceUnit;
					m_stcQuoteInfo.pstcQuote[i].lastTick = pNow[nCount].m_timeUpdate;
					m_stcQuoteInfo.pstcQuote[i].uiPrice = pNow[nCount].m_uiNewPrice;
					if ((int)pNow[nCount].m_cVolPriceCount > 0)
					{
						m_stcQuoteInfo.pstcQuote[i].uiBuyPrice1 = pNow[nCount].m_arrVolPrice[0].m_buyVol.m_uiPrice;
						m_stcQuoteInfo.pstcQuote[i].uiSellPrice1 = pNow[nCount].m_arrVolPrice[0].m_sellVol.m_uiPrice;
					}
					i++;
				}
				m_stcQuoteInfo.iNum = pDeataHead->m_usSize*sizeof(SPOTQUOTEDATE);
				m_lockRealQuoteLst.Unlock();
				m_pPublic->NotifyWnd(2,REPORTDATA_ZYQ);
			}
			else if(CDateManage::g_iProtocolVersion == V2)
			{
				STUNowDataZYQ_NEW* pNow = (STUNowDataZYQ_NEW*)(pDeataHead+1);
				int nCount;
				m_lockRealQuoteLst.Lock();
				m_mapQuotationLst.clear();
				m_stcQuoteInfo.ReSetSize(pDeataHead->m_usSize);
				int i=0;
				for(nCount =0;nCount<pDeataHead->m_usSize;nCount++)
				{	
					CString strSymbol = pNow[nCount].m_oSymbolInfo.m_szSymbol;
					strSymbol.Trim();
					m_mapQuotationLst[pNow[nCount].m_oSymbolInfo.m_szSymbol] = pNow[nCount];

					memcpy(m_stcQuoteInfo.pstcQuote[i].sCode.szCode,pNow[nCount].m_oSymbolInfo.m_szSymbol,11);
					m_stcQuoteInfo.pstcQuote[i].sCode.cPriceUnit = pNow[nCount].m_oSymbolInfo.m_cPriceUnit;
					m_stcQuoteInfo.pstcQuote[i].lastTick = pNow[nCount].m_iUpdateDateTime;
					m_stcQuoteInfo.pstcQuote[i].uiPrice = pNow[nCount].m_uiNewPrice;
					if ((int)pNow[nCount].m_cVolPriceCount > 0)
					{
						m_stcQuoteInfo.pstcQuote[i].uiBuyPrice1 = pNow[nCount].m_arrVolPrice[0].m_buyVol.m_uiPrice;
						m_stcQuoteInfo.pstcQuote[i].uiSellPrice1 = pNow[nCount].m_arrVolPrice[0].m_sellVol.m_uiPrice;
					}
					i++;
				}
				m_stcQuoteInfo.iNum = pDeataHead->m_usSize*sizeof(SPOTQUOTEDATE);
				m_lockRealQuoteLst.Unlock();
				m_pPublic->NotifyWnd(2,REPORTDATA_ZYQ);
			}
		}
		break;
	case PUSH_REALDATA_ZYQ:
		{
			if (CDateManage::g_iProtocolVersion == V1)
			{
				STURealQuoteInfoZQY *pRealData = (STURealQuoteInfoZQY*)(pDeataHead+1);
				m_lockRealQuoteLst.Lock();
				m_stcQuoteInfo.ReSetSize(pDeataHead->m_usSize);
				int i=0;
				for (int iNum = 0;iNum < pDeataHead->m_usSize;iNum++)
				{
					CString strSymbol = pRealData[iNum].m_oSymbol.m_szSymbol;
					strSymbol.Trim();
					m_mapQuotationLst[strSymbol] = pRealData[iNum];

					memcpy(&m_stcQuoteInfo.pstcQuote[i].sCode.szCode,pRealData[iNum].m_oSymbol.m_szSymbol,11);
					m_stcQuoteInfo.pstcQuote[i].sCode.cPriceUnit = pRealData[iNum].m_oSymbol.m_cPriceUnit;
					m_stcQuoteInfo.pstcQuote[i].lastTick = pRealData[iNum].m_iUpdateDateTime;
					m_stcQuoteInfo.pstcQuote[i].uiPrice = pRealData[iNum].m_uiNewPrice;
					if ((int)pRealData[iNum].m_cVolPriceCount > 0)
					{
						m_stcQuoteInfo.pstcQuote[i].uiBuyPrice1 = pRealData[iNum].m_arrVolPrice[0].m_buyVol.m_uiPrice;
						m_stcQuoteInfo.pstcQuote[i].uiSellPrice1 = pRealData[iNum].m_arrVolPrice[0].m_sellVol.m_uiPrice;
					}
					i++;
				}
				m_stcQuoteInfo.iNum = pDeataHead->m_usSize*sizeof(SPOTQUOTEDATE);
				m_lockRealQuoteLst.Unlock();
				m_pPublic->NotifyWnd(2,PUSH_REALDATA_ZYQ);
				//m_pPublic->NotifyWnd(lpData,nDataLen,2,12);
			}
			else if(CDateManage::g_iProtocolVersion == V2)
			{
				STURealQuoteInfoZQY_NEW *pRealData = (STURealQuoteInfoZQY_NEW*)(pDeataHead+1);
				m_lockRealQuoteLst.Lock();
				m_stcQuoteInfo.ReSetSize(pDeataHead->m_usSize);
				int i=0;
				for (int iNum = 0;iNum < pDeataHead->m_usSize;iNum++)
				{
					CString strSymbol = pRealData[iNum].m_oSymbol.m_szSymbol;
					strSymbol.Trim();
					m_mapQuotationLst[strSymbol] = pRealData[iNum];

					memcpy(&m_stcQuoteInfo.pstcQuote[i].sCode.szCode,pRealData[iNum].m_oSymbol.m_szSymbol,11);
					m_stcQuoteInfo.pstcQuote[i].sCode.cPriceUnit = pRealData[iNum].m_oSymbol.m_cPriceUnit;
					m_stcQuoteInfo.pstcQuote[i].lastTick = pRealData[iNum].m_iUpdateDateTime;
					m_stcQuoteInfo.pstcQuote[i].uiPrice = pRealData[iNum].m_uiNewPrice;
					if ((int)pRealData[iNum].m_cVolPriceCount > 0)
					{
						m_stcQuoteInfo.pstcQuote[i].uiBuyPrice1 = pRealData[iNum].m_arrVolPrice[0].m_buyVol.m_uiPrice;
						m_stcQuoteInfo.pstcQuote[i].uiSellPrice1 = pRealData[iNum].m_arrVolPrice[0].m_sellVol.m_uiPrice;
					}
					i++;
				}
				m_stcQuoteInfo.iNum = pDeataHead->m_usSize*sizeof(SPOTQUOTEDATE);
				m_lockRealQuoteLst.Unlock();
				m_pPublic->NotifyWnd(2,PUSH_REALDATA_ZYQ);

				m_pPublic->NotifyWnd(lpData,nDataLen,2,13);
			}
		}
		break;
	case REPORTDATA_SETTLE_SP:
		{
			STUSettlQuoteInfo* pSettleQuoInfo = (STUSettlQuoteInfo*)(pDeataHead+1);
			for (int iNum = 0;iNum < pDeataHead->m_usSize;iNum++)
			{
				m_mapSettleQuoteLst[pSettleQuoInfo[iNum].m_iCommTrunoverQty] = pSettleQuoInfo[iNum];
			}
		}
		break;
	case PUSH_SETTLEDATA_SP:
		{
			STUSettlQuoteInfo* pSettleQuoInfo = (STUSettlQuoteInfo*)(pDeataHead+1);
			for (int iNum = 0;iNum < pDeataHead->m_usSize;iNum++)
			{
				m_mapSettleQuoteLst[pSettleQuoInfo[iNum].m_iCommTrunoverQty] = pSettleQuoInfo[iNum];
			}
		}
		break;
	case OPEN_MAKRKET_ZYQ:
		{
			if (CDateManage::g_iProtocolVersion == V1)
			{
				STUOpenQuoteInfoZYQ* pOpenQuoteInfo = (STUOpenQuoteInfoZYQ*)(pDeataHead+1);
				STUNowDataZYQ stcDate;
				m_lockRealQuoteLst.Lock();
				m_mapQuotationLst.clear();
				for (int iNum = 0;iNum < pDeataHead->m_usSize;iNum++)
				{
					memset(&stcDate,0,sizeof(STUNowDataZYQ));
					CString strSymbol = pOpenQuoteInfo[iNum].m_oSymbol.m_szSymbol;
					strSymbol.Trim();
					stcDate.m_usTradeDay = pOpenQuoteInfo[iNum].m_iTradeDay;			
					stcDate.m_timeUpdate = pOpenQuoteInfo[iNum].m_iUpdateDateTime;		
					stcDate.m_oSymbolInfo = pOpenQuoteInfo[iNum].m_oSymbol;
					stcDate.m_uiYesClosePrice = pOpenQuoteInfo[iNum].m_uiYesClosePrice;     
					stcDate.m_uiYesSettlPrice = pOpenQuoteInfo[iNum].m_uiYesSettlPrice;    
					stcDate.m_iTotalOrderQty = pOpenQuoteInfo[iNum].m_iTotalOrderQty;         
					m_mapQuotationLst[strSymbol] = stcDate;
				}
				m_lockRealQuoteLst.Unlock();
				m_pPublic->NotifyWnd(2,OPEN_MAKRKET_ZYQ);
			}
			else if(CDateManage::g_iProtocolVersion == V2)
			{
				STUOpenQuoteInfoZYQ_NEW* pOpenQuoteInfo = (STUOpenQuoteInfoZYQ_NEW*)(pDeataHead+1);
				STUNowDataZYQ stcDate;
				m_lockRealQuoteLst.Lock();
				m_mapQuotationLst.clear();
				for (int iNum = 0;iNum < pDeataHead->m_usSize;iNum++)
				{
					memset(&stcDate,0,sizeof(STUNowDataZYQ));
					CString strSymbol = pOpenQuoteInfo[iNum].m_oSymbol.m_szSymbol;
					strSymbol.Trim();
					stcDate.m_usTradeDay = pOpenQuoteInfo[iNum].m_iTradeDay;					
					stcDate.m_timeUpdate = pOpenQuoteInfo[iNum].m_iUpdateDateTime;			 
					stcDate.m_oSymbolInfo = pOpenQuoteInfo[iNum].m_oSymbol;
					stcDate.m_uiYesClosePrice = pOpenQuoteInfo[iNum].m_uiYesClosePrice;      
					stcDate.m_uiYesSettlPrice = pOpenQuoteInfo[iNum].m_uiYesSettlPrice;     
					stcDate.m_iTotalOrderQty = pOpenQuoteInfo[iNum].m_iTotalOrderQty;       
					m_mapQuotationLst[strSymbol] = stcDate;
				}
				m_lockRealQuoteLst.Unlock();
				m_pPublic->NotifyWnd(2,OPEN_MAKRKET_ZYQ);
			}
		}
		break;
	case CLOSE_MAKRKET_ZYQ:
		{
			m_pPublic->NotifyWnd(2,CLOSE_MAKRKET_ZYQ);
		}
		break;
	case STKMINUTEDATA_ZYQ:
		{
			//STUSTKMinUnitZYQ* pOpenQuoteInfo = (STUSTKMinUnitZYQ*)(pDeataHead+1);
			m_pPublic->NotifyWnd(lpData,nDataLen,2,13);
		}
		break;
	case HISKDATA_ZYQ:
		{
			m_pPublic->NotifyWnd(lpData,nDataLen,2,12);
		}
		break;
	default:
		break;
	}
}

void CDateManage::ReqRegRealQuotationDate(int iType)
{
	if (iType == 0)
	{
		RegRealMetalsQuotationDate();
	}
	else if (iType == 1)
	{
		RegRealSpotQuotationDate();
	}
}

void CDateManage::GetRealQuoteInfo(mapTuNowDateZYQ & maplst)
{
	maplst.clear();
	m_lockRealQuoteLst.Lock();
	do 
	{
		int iNum = m_mapQuotationLst.size();
		if (iNum<=0)break;

		maplst.insert(m_mapQuotationLst.begin(),m_mapQuotationLst.end());
	} while (0);
	m_lockRealQuoteLst.Unlock();
}
CString CDateManage::GetSymbolName(CString strCode)
{
	CString strName = _T("");
	STUSymbolDataZYQ stcSymbol;
	vector <STUSymbolDataZYQ>::iterator itvec;
	for (itvec = m_vecSymbol.begin(); itvec != m_vecSymbol.end(); itvec++)
	{
		if (strCode.CompareNoCase(itvec->m_oSymbolInfo.m_szSymbol) == 0)
		{
			strName = itvec->m_szName;
			break;
		}
	}

	return strName;
}

void CDateManage::AddTestDate()
{
	return;
}

int CDateManage::ExecuteUrl(CString strUrl)
{
	HANDLE hRet;
	hRet = ShellExecute(NULL, _T("open"), strUrl,NULL, NULL, SW_SHOWNORMAL);
	return (int)hRet;
}
STUSymbolDataZYQ CDateManage::GetSymbolInfo(CString strCode)
{
	STUSymbolDataZYQ stcSymbolInfo;
	ZeroMemory(&stcSymbolInfo,sizeof(STUSymbolDataZYQ));

	vector <STUSymbolDataZYQ>::iterator vIt;
	for (vIt = m_vecSymbol.begin(); vIt != m_vecSymbol.end(); vIt++)
	{
		if (strCode.CompareNoCase(vIt->m_oSymbolInfo.m_szSymbol) == 0)
		{
			stcSymbolInfo = *vIt;
			break;
		}
	}
	return stcSymbolInfo;
}
CString CDateManage::DoubleToString( double dblValue,int nDec )
{
	CString strValue=_T("");
	switch(nDec)
	{
	case 0 :
		strValue.Format(_T("%.f"),dblValue);
		break;
	case 1 :
		strValue.Format(_T("%.1f"),dblValue);
		break;
	case 2 :
		strValue.Format(_T("%.2f"),dblValue);
		break;
	case 3 :
		strValue.Format(_T("%.3f"),dblValue);
		break;
	case 4 :
		strValue.Format(_T("%.4f"),dblValue);
		break;
	case 5 :
		strValue.Format(_T("%.5f"),dblValue);
		break;
	case 6 :
		strValue.Format(_T("%.6f"),dblValue);
		break;
	default :
		strValue.Format(_T("%.f"),dblValue);
		break;
	}
	if ( strValue.Find(_T(".")) == 0 )
	{
		strValue.Insert(0,_T("0")) ;
	}

	return strValue;
}
void CDateManage::NotifyTrade(UINT iMsg,WPARAM wPar, LPARAM lPar,int iNotifyWnd)
{
	CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(iMsg,iNotifyWnd,0);
}
void CDateManage::GetFontFamily()
{
	int count=0;  
	int found=0;  

	WCHAR familyName[100];
	WCHAR *familyList=NULL;  
	FontFamily pFontFamily[500];

	InstalledFontCollection installedFontCollection;  
	count=installedFontCollection.GetFamilyCount();  
	installedFontCollection.GetFamilies(count,pFontFamily,&found);  

	familyList=new WCHAR[count*sizeof(familyName)];  
	wcscpy(familyList,L""); 
	for(int j=0;j<count;j++){  
		pFontFamily[j].GetFamilyName(familyName);  
		wcscat(familyList,familyName);
		wcscat(familyList,L",");
	}  

	delete[] familyList; 
}
void CDateManage::DrawTriangle(Graphics &GraphCach,CRect rcDraw)
{
	GraphicsPath gPath;
	gPath.StartFigure();
	Gdiplus::Point pt[3] = {Point(rcDraw.left,rcDraw.top),Point(rcDraw.right,rcDraw.top)
		,Point(rcDraw.left+rcDraw.Width()/2,rcDraw.bottom)};
	gPath.AddLines(pt,3);
	gPath.CloseFigure();
	gPath.SetFillMode(FillModeWinding);
	SolidBrush slb(Color(255,255,0,0));
	GraphCach.FillPath(&slb,&gPath);
}
void CDateManage::ResImg(Gdiplus::Image ** pImg)
{
	if (*pImg)
	{
		delete *pImg;
		*pImg = NULL;
	}
}
void CDateManage::ReleaseImg()
{
	ResImg(&m_ImgBottomHide);
	ResImg(&m_ImgTopHide);
	ResImg(&m_ImgLeftide);
	ResImg(&m_ImgRightHide);
}
void CDateManage::InitImg()
{
	ReleaseImg();
	USES_CONVERSION;
	CString strImgBottomHide,strImgTopHide,strImgLeftide,strImgRightHide;
	strImgBottomHide = m_strImgPath + _T("BottomHide.png");
	strImgTopHide = m_strImgPath + _T("TopHide.png");
	strImgLeftide = m_strImgPath + _T("LeftHide.png");
	strImgRightHide = m_strImgPath + _T("RightHide.png");

	m_ImgBottomHide = Image::FromFile(T2W(strImgBottomHide));
	m_ImgTopHide = Image::FromFile(T2W(strImgTopHide));
	m_ImgLeftide = Image::FromFile(T2W(strImgLeftide));
	m_ImgRightHide = Image::FromFile(T2W(strImgRightHide));
}

void CDateManage::DrawBottomHide(CWnd *pWnd,CRect rcDraw,int iStat)
{
	if (!m_ImgBottomHide)return;

	HDC hDc = ::GetDC(pWnd->GetSafeHwnd());
	Gdiplus::Bitmap cachBmp(rcDraw.Width(),rcDraw.Height());
	Graphics gpDraw(hDc);
	Graphics gpCach(&cachBmp);

	int iEveryH = m_ImgBottomHide->GetHeight()/2;
	int iEveryW = m_ImgBottomHide->GetWidth();
	gpCach.DrawImage(m_ImgBottomHide,Rect(0,0,iEveryW,iEveryH),
		0,iEveryH*iStat,iEveryW,iEveryH,UnitPixel);

	gpDraw.DrawImage(&cachBmp,rcDraw.left,rcDraw.top);
	gpDraw.ReleaseHDC(hDc);
	ReleaseDC(pWnd->GetSafeHwnd(),hDc);
}
void CDateManage::DrawTopHide(CWnd *pWnd,CRect rcDraw,int iStat)
{
	if (!m_ImgTopHide)return;

	HDC hDc = ::GetDC(pWnd->GetSafeHwnd());
	Gdiplus::Bitmap cachBmp(rcDraw.Width(),rcDraw.Height());
	Graphics gpDraw(hDc);
	Graphics gpCach(&cachBmp);

	int iEveryH = m_ImgTopHide->GetHeight()/2;
	int iEveryW = m_ImgTopHide->GetWidth();
	gpCach.DrawImage(m_ImgTopHide,Rect(0,0,iEveryW,iEveryH),
		0,iEveryH*iStat,iEveryW,iEveryH,UnitPixel);

	gpDraw.DrawImage(&cachBmp,rcDraw.left,rcDraw.top);
	gpDraw.ReleaseHDC(hDc);
	ReleaseDC(pWnd->GetSafeHwnd(),hDc);
}
void CDateManage::DrawLeftHide(CWnd *pWnd,CRect rcDraw,int iStat)
{
	if (!m_ImgLeftide)return;

	HDC hDc = ::GetDC(pWnd->GetSafeHwnd());
	Gdiplus::Bitmap cachBmp(rcDraw.Width(),rcDraw.Height());
	Graphics gpDraw(hDc);
	Graphics gpCach(&cachBmp);

	int iEveryH = m_ImgLeftide->GetHeight()/2;
	int iEveryW = m_ImgLeftide->GetWidth();
	gpCach.DrawImage(m_ImgLeftide,Rect(0,0,iEveryW,iEveryH),
		0,iEveryH*iStat,iEveryW,iEveryH,UnitPixel);

	gpDraw.DrawImage(&cachBmp,rcDraw.left,rcDraw.top);
	gpDraw.ReleaseHDC(hDc);
	ReleaseDC(pWnd->GetSafeHwnd(),hDc);
}
void CDateManage::DrawRightHide(CWnd *pWnd,CRect rcDraw,int iStat)
{
	if (!m_ImgRightHide)return;

	HDC hDc = ::GetDC(pWnd->GetSafeHwnd());
	Gdiplus::Bitmap cachBmp(rcDraw.Width(),rcDraw.Height());
	Graphics gpDraw(hDc);
	Graphics gpCach(&cachBmp);

	int iEveryH = m_ImgRightHide->GetHeight()/2;
	int iEveryW = m_ImgRightHide->GetWidth();
	gpCach.DrawImage(m_ImgRightHide,Rect(0,0,iEveryW,iEveryH),
		0,iEveryH*iStat,iEveryW,iEveryH,UnitPixel);

	gpDraw.DrawImage(&cachBmp,rcDraw.left,rcDraw.top);
	gpDraw.ReleaseHDC(hDc);
	ReleaseDC(pWnd->GetSafeHwnd(),hDc);
}

int CDateManage::GetDaysInMonth(int iYear,int iMonth)
{
	int d;
	int day[]= {31,28,31,30,31,30,31,31,30,31,30,31};
	if (2 == iMonth)d=(((0 == iYear%4)&&(0 != iYear%100)||(0 == iYear%400))?29:28);
	else d=day[iMonth-1];
	return d;
}
CString CDateManage::FormatTime(int iDate)
{
	CString strRet=_T("");
	((iDate < 10)?strRet.Format(_T("0%d"),iDate):strRet.Format(_T("%d"),iDate));
	return strRet;
}
CString CDateManage::FormatTime(int iDate,int iType)
{
	CString strRet=_T("");
	if (iDate > 2400)
	{
		strRet += FormatTime(iDate/10000);
		strRet += _T(":");
		strRet += FormatTime((iDate%10000)/100);
		strRet += _T(":");
		strRet += FormatTime((iDate%10000)%100);
	}
	else
	{
		strRet += FormatTime((iDate)/100);
		strRet += _T(":");
		strRet += FormatTime((iDate)%100);
	}
	return strRet;
}
CString CDateManage::FormatString(int iDate,int iFormatType)
{
	CString strRet=_T("");
	if(iFormatType >= DAY_HISK)
	{
// 		int nYear = iDate/10000;
// 		int nMonth = (iDate%10000)/100;
// 		int nDay = (iDate%10000)%100;
// 
// 		if( nYear < 1900 || nMonth < 1 ||  nMonth > 12
// 			|| nDay < 1 || nDay > 31)
// 			return "";

		CTime tmFormat(iDate/10000,(iDate%10000)/100,((iDate%10000)%100)==0?GetDaysInMonth(iDate/10000,(iDate%10000)/100):((iDate%10000)%100),0,0,0);
		strRet = tmFormat.Format(_T("%Y/%m/%d/"));//%w
		do 
		{
			CString strTemp = _T("");
			int iWeek = tmFormat.GetDayOfWeek()-1;
			if (iWeek == 0)strTemp = _T("日");
			if (iWeek == 1)strTemp = _T("一");
			if (iWeek == 2)strTemp = _T("二");
			if (iWeek == 3)strTemp = _T("三");
			if (iWeek == 4)strTemp = _T("四");
			if (iWeek == 5)strTemp = _T("五");
			if (iWeek == 6)strTemp = _T("六");
			strRet += strTemp;
		} while (0);
	}
	else
	{
// 		int nMonth = iDate/1000000;
// 		int nDay = (iDate%1000000)/10000;
// 		int nHour =((iDate%1000000)%10000)/100;
// 		int nMin =((iDate%1000000)%10000)%100;
// 
// 		if( nMonth < 1 ||  nMonth > 12
// 			|| nDay < 1 || nDay > 31
// 			|| nHour > 23 || nMin > 59)
// 			return "";

		CTime tmFormat(2014,iDate/1000000,(iDate%1000000)/10000,((iDate%1000000)%10000)/100,((iDate%1000000)%10000)%100,0);
		strRet = tmFormat.Format(_T("%m/%d %H:%M"));
	}
	return strRet;
}
#define D_Z_T_O(x)  ((x<=0)?1:x)
CString CDateManage::FormatTimeY(INT64 uiDate,int iMode)
{
	CString strRet=_T("");
	do 
	{
		if (uiDate < 10000000000)break;
		int iYear = D_Z_T_O(uiDate/10000000000);
		int iMonth = D_Z_T_O((uiDate%10000000000)/100000000);
		int iDay = D_Z_T_O(uiDate%100000000/1000000);
		int iHour = D_Z_T_O(uiDate%1000000/10000);
		int iMinit = D_Z_T_O(uiDate%10000/100);
		int iSecond = D_Z_T_O(uiDate%100);
		CTime tmFormat(iYear,iMonth,iDay,iHour,iMinit,iSecond);
		if(iMode == 2)
		    strRet = tmFormat.Format(_T("%Y/%m/%d %H:%M"));
		else if (iMode == 1)
		{
			strRet = tmFormat.Format(_T("%Y/%m/%d"));
		}
		else if(iMode == 0)
		{
			strRet = tmFormat.Format(_T("%H:%M"));
		}
	} while (0);
	
	return strRet;
}
void CDateManage::DrawBtnByString(Graphics & gpCach,STRBTN &stcStrBtn,int iCheckId)
{
	if (stcStrBtn.iBtnId <= 0)return;

	CSkinAttribute_General *pSkinGn = (CSkinAttribute_General *)GetSkinAttribute(7);

	Gdiplus::SolidBrush sb(Color(255,0,255,255));
	Gdiplus::Pen penBd(Color(255,125,0,0),0.6);
	Gdiplus::SolidBrush sbrFill(Color(255,0,0,0));
	GraphicsPath gppRound;
	AddRoundRect(gppRound,stcStrBtn.rcBtn.left,stcStrBtn.rcBtn.top,stcStrBtn.rcBtn.Width(),stcStrBtn.rcBtn.Height(),2,2);
	gpCach.DrawPath(&penBd,&gppRound);
	if (stcStrBtn.iBtnId == iCheckId)
	{
		sbrFill.SetColor(Color(255,143,149,243)/*pSkinGn->clrBkSel*/);
		sb.SetColor(Color(255,0,0,255));
	}
	gpCach.FillRectangle(&sbrFill,Rect(stcStrBtn.rcBtn.left+1,stcStrBtn.rcBtn.top+1,stcStrBtn.rcBtn.Width()-2,stcStrBtn.rcBtn.Height()-2));
	if (stcStrBtn.pImg)
	{
		gpCach.DrawImage(stcStrBtn.pImg,Rect(stcStrBtn.rcBtn.left,stcStrBtn.rcBtn.top,stcStrBtn.rcBtn.Width(),stcStrBtn.rcBtn.Height()));
	}

	if (stcStrBtn.strText.IsEmpty()) return;
	USES_CONVERSION;
	
    //Gdiplus::FontFamily fm(_T("宋体"));
	Gdiplus::Font ft(FontFamily::GenericSansSerif(),stcStrBtn.iTextSize,FontStyleRegular,UnitPixel);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	gpCach.DrawString(T2W(stcStrBtn.strText),-1,&ft,RectF(stcStrBtn.rcBtn.left,stcStrBtn.rcBtn.top,
		stcStrBtn.rcBtn.Width(),stcStrBtn.rcBtn.Height()),&format,&sb);
}
void CDateManage::AddRoundRect(GraphicsPath &gp,INT x, INT y, INT width, INT height, INT cornerX, INT cornerY)
{
	INT elWid = 2*cornerX;
	INT elHei = 2*cornerY;

	gp.AddArc(x,y,elWid,elHei,180,90); 
	gp.AddLine(x+cornerX,y,x+width-cornerX,y); 

	gp.AddArc(x+width-elWid,y, elWid,elHei,270,90); 
	gp.AddLine(x+width,y+cornerY, x+width,y+height-cornerY);

	gp.AddArc(x+width-elWid,y+height-elHei, elWid,elHei,0,90); 
	gp.AddLine(x+width-cornerX,y+height, x+cornerX,y+height); 

	gp.AddArc(x,y+height-elHei, elWid,elHei,90,90); 
	gp.AddLine(x,y+cornerY, x, y+height-cornerY);
}
void CDateManage::DrawToolBar(Graphics & gpCach,DRAWTOOLBAR stcDraw)
{
	if (stcDraw.iStartId <= 0 || 
		stcDraw.iTotalBtn <= 0 ||
		stcDraw.pImg == NULL)return;

	CSkinAttribute_General *pSkinGn = (CSkinAttribute_General *)GetSkinAttribute(7);
	Gdiplus::Pen penBd(Color(255,175,99,99),1.0);
	Gdiplus::SolidBrush sbrFillAll(pSkinGn->clrBkNormal);
	Gdiplus::SolidBrush sbrFillSel(pSkinGn->clrBkSel);
	CRect rcItem = stcDraw.rcItem;
	GraphicsPath gppRound;
	AddRoundRect(gppRound,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),2,2);
	gpCach.DrawPath(&penBd,&gppRound);

	gpCach.FillRectangle(&sbrFillAll,Rect(rcItem.left+1,rcItem.top+1,rcItem.Width()-2,rcItem.Height()-2));
	if (stcDraw.iCurSel >= 0)
	{
		gpCach.FillRectangle(&sbrFillSel,Rect(rcItem.left+stcDraw.iEveryWith*stcDraw.iCurSel,rcItem.top,stcDraw.iEveryWith,rcItem.Height()));
	}

	gpCach.DrawImage(stcDraw.pImg,Rect(stcDraw.rcItem.left,stcDraw.rcItem.top,rcItem.Width(),rcItem.Height()));

}
void CDateManage::DrawBkRect(Graphics &gpCach,CRect rcDraw,DWORD clrFill)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0 ||
		!gpCach.IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;

	Gdiplus::SolidBrush sbrzFill(clrFill);

	gpCach.FillRectangle(&sbrzFill,Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height()));
}
void CDateManage::DrawBkRect(Graphics &gpCach,CRect rcDraw)
{
	DrawBkRect(gpCach,rcDraw,m_clrFill);
}
void CDateManage::DrawBkRect(CDC * pDc,CRect rcDraw,DWORD clrFill)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0 || pDc == NULL)return;
	Gdiplus::Bitmap cachBmp(rcDraw.Width(),rcDraw.Height());
	Graphics gpDraw(pDc->m_hDC);
	Graphics gpCach(&cachBmp);
	Gdiplus::SolidBrush sbBk(clrFill);

	gpCach.FillRectangle(&sbBk,Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height()));
	gpDraw.DrawImage(&cachBmp,0,0);
	gpDraw.ReleaseHDC(pDc->m_hDC);
}
void CDateManage::DrawBkRect_DC(CDC * pDc,CRect rcDraw,DWORD clrFill)
{
	HDC hMemDc = CreateCompatibleDC(pDc->m_hDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(pDc->m_hDC, rcDraw.Width(), rcDraw.Height());
	SelectObject(hMemDc,hMemBitmap);
	Graphics gpDraw(hMemDc);
	Gdiplus::SolidBrush sbBk(clrFill);
	gpDraw.FillRectangle(&sbBk,Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height()));

	::BitBlt(pDc->m_hDC,0,0,rcDraw.Width(),rcDraw.Height(),
		hMemDc,0,0,SRCCOPY);
	DeleteObject(hMemBitmap);
	DeleteDC(hMemDc);
}
void CDateManage::DrawBkRect(CDC * pDc,CRect rcDraw)
{
	DrawBkRect_DC(pDc,rcDraw,m_clrFill);
}

void CDateManage::InitSkinAttribute_Black()
{
	CSkinAttribute_Gap SkinGap;
	CSkinAttribute_Gap *pSkinGap = &SkinGap;//new CSkinAttribute_Gap;
	pSkinGap->clrBkAll = Color::MakeARGB(255,31,34,41);
	pSkinGap->clrBk1 = Color::MakeARGB(255,31,34,41);
	pSkinGap->clrBk2 = Color::MakeARGB(255,31,34,41);
	pSkinGap->clrBk3 = Color::MakeARGB(255,0,0,255);
	pSkinGap->clrBk4 = Color::MakeARGB(255,88,88,88);
	CSkinTransform::Instance()->SetSkinAttribute(2,pSkinGap);


	CSkinAttribute_List SKinLst;
	CSkinAttribute_List *pSkinLst = &SKinLst;//new CSkinAttribute_List;
	pSkinLst->clrTitleBkNormal = Color::MakeARGB(255,38,38,38);
	pSkinLst->clrTitleBkOver = Color::MakeARGB(255,66,66,66);
	pSkinLst->clrTitleBkSel = Color::MakeARGB(255,88,88,88);
	pSkinLst->clrTitleBkDisable = Color::MakeARGB(255,128,128,128);
	pSkinLst->clrTitleTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextOver = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextSel = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextDisable = Color::MakeARGB(255,128,128,128);
	pSkinLst->clrTitleSort = Color::MakeARGB(255,255,0,0);
	pSkinLst->clrTextMark = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrGapLine = Color::MakeARGB(255,33,33,33);
	pSkinLst->clrBkAll = Color::MakeARGB(255,31,34,41);
	pSkinLst->clrBkNormal = Color::MakeARGB(255,42,43,45);
	pSkinLst->clrBkOver = Color::MakeARGB(255,42,43,45);
	pSkinLst->clrBkSel = Color::MakeARGB(255,62,63,65);
	pSkinLst->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinLst->clrTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinLst->clrFrameIn = Color::MakeARGB(255,9,9,9);
	pSkinLst->clrFrameOut = Color::MakeARGB(255,55,55,55);
	pSkinLst->clrTotal = Color::MakeARGB(255,255,255,255);
	m_clrFill = pSkinLst->clrBkAll;
	CSkinTransform::Instance()->SetSkinAttribute(1,pSkinLst);

	CSkinAttribute_Fund SkinFund;
	CSkinAttribute_Fund *pSkinFund = &SkinFund;//new CSkinAttribute_Fund;
	pSkinFund->clrBkAll = Color::MakeARGB(255,21,21,21);
	pSkinFund->clrBkNormal = Color::MakeARGB(255,21,21,21);
	pSkinFund->clrTextDown = Color::MakeARGB(255,20,255,20);
	pSkinFund->clrTextUp = Color::MakeARGB(255,255,20,20);
	pSkinFund->clrTextNormal = Color::MakeARGB(255,255,255,255);
	pSkinFund->clrTextMark = Color::MakeARGB(255,255,255,255);
	CSkinTransform::Instance()->SetSkinAttribute(4,pSkinFund);

	CSkinAttribute_General SkinGn3;
	CSkinAttribute_General *pSkinGn3 = &SkinGn3;//new CSkinAttribute_General;
	pSkinGn3->clrBkAll = Color::MakeARGB(255,21,21,21);
	pSkinGn3->clrAllFrameIn = Color::MakeARGB(255,66,66,66);
	pSkinGn3->clrAllFrameOut = Color::MakeARGB(255,0,0,0);
	pSkinGn3->clrBkNormal = Color::MakeARGB(255,21,21,21);
	pSkinGn3->clrBkOver = Color::MakeARGB(255,42,43,45);
	pSkinGn3->clrBkSel = Color::MakeARGB(255,62,63,65);
	pSkinGn3->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn3->clrTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrTextSel = Color::MakeARGB(255,21,25,39);
	pSkinGn3->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn3->clrTextMark = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn3->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn3->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(8,pSkinGn3);

	CSkinAttribute_ScrollBar SkinScroll;
	CSkinAttribute_ScrollBar *pSkinScroll = &SkinScroll;//new CSkinAttribute_ScrollBar;
	pSkinScroll->clrBkAll = Color::MakeARGB(255,31,34,41);
	pSkinScroll->clrAllLineOut = Color::MakeARGB(255,55,55,55);
	pSkinScroll->clrAllLineIn = Color::MakeARGB(255,9,9,9);
	pSkinScroll->clrBoxBkNormal = Color::MakeARGB(255,42,43,52/*41,49,61*/);
	pSkinScroll->clrBtnBkNormal = Color::MakeARGB(255,42,43,45);
	pSkinScroll->clrLineOut = Color::MakeARGB(255,9,9,9);
	pSkinScroll->clrLineIn = Color::MakeARGB(255,55,55,55);
	pSkinScroll->clrArrow = Color::MakeARGB(199,188,188,188);
	CSkinTransform::Instance()->SetSkinAttribute(5,pSkinScroll);

	CSkinAttribute_General SkinGn1;
	CSkinAttribute_General *pSkinGn1 = &SkinGn1;//new CSkinAttribute_General;
	pSkinGn1->clrBkAll = Color::MakeARGB(255,72,0,1);
	pSkinGn1->clrAllFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn1->clrAllFrameOut = Color::MakeARGB(255,0,0,0);

	pSkinGn1->clrBkNormal = Color::MakeARGB(255,72,0,1);
	pSkinGn1->clrBkOver = Color::MakeARGB(255,246,87,87);
	pSkinGn1->clrBkSel = Color::MakeARGB(255,155,5,5);
	pSkinGn1->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn1->clrTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn1->clrTextMark = Color::MakeARGB(255,255,255,255);

	pSkinGn1->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn1->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn1->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(6,pSkinGn1);

	CSkinAttribute_General SkinGn2;
	CSkinAttribute_General *pSkinGn2 = &SkinGn2;//new CSkinAttribute_General;
	pSkinGn2->clrBkAll = Color::MakeARGB(255,31,34,41);
	pSkinGn2->clrAllFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn2->clrAllFrameOut = Color::MakeARGB(255,0,0,0);
	pSkinGn2->clrBkNormal = Color::MakeARGB(255,31,34,41);
	pSkinGn2->clrBkOver = Color::MakeARGB(255,42,43,45);
	pSkinGn2->clrBkSel = Color::MakeARGB(255,62,63,65);
	pSkinGn2->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn2->clrTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn2->clrTextMark = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn2->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn2->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(7,pSkinGn2);

	CSkinAttribute_FrameLine SkinFrameLine;
	CSkinAttribute_FrameLine *pSkinFrameLine = &SkinFrameLine;//new CSkinAttribute_FrameLine;
	pSkinFrameLine->clrBkAll = Color::MakeARGB(255,31,34,41);
	pSkinFrameLine->clrBkLineIn = Color::MakeARGB(255,9,9,9);
	pSkinFrameLine->clrBkLineOut = Color::MakeARGB(255,55,55,55);
	CSkinTransform::Instance()->SetSkinAttribute(3,pSkinFrameLine);

	CSkinAttribute_QuoteCard SkinQC;
	CSkinAttribute_QuoteCard *pSkinQC = &SkinQC;
	pSkinQC->clrBkAll = Color::MakeARGB(255,66,69,74);
	pSkinQC->clrAllFrameIn = Color::MakeARGB(255,111,111,111);
	pSkinQC->clrAllFrameOut = Color::MakeARGB(255,9,9,9);
	pSkinQC->clrText1 = Color::MakeARGB(255,255,255,64);
	pSkinQC->clrText2 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrPart2 = Color::MakeARGB(255,161,193,255);
	pSkinQC->clrText3 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrPart3 = Color::MakeARGB(255,255,129,129);
	pSkinQC->clrPartBordeNomal = Color::MakeARGB(255,148,148,148);
	pSkinQC->clrPartBordeOver = Color::MakeARGB(255,222,222,222);
	pSkinQC->clrFillItemNomal = Color::MakeARGB(255,66,69,74);
	pSkinQC->clrFillItemOver = Color::MakeARGB(255,89,91,94);
	pSkinQC->clrText6 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText7 = Color::MakeARGB(255,255,96,94);
	pSkinQC->clrText8 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText9 = Color::MakeARGB(255,255,96,94);
	CSkinTransform::Instance()->SetSkinAttribute(9,pSkinQC);
}
void CDateManage::InitSkinAttribute_White()
{
	CSkinAttribute_General SkinGn3;
	CSkinAttribute_General *pSkinGn3 = &SkinGn3;//new CSkinAttribute_General;
	pSkinGn3->clrBkAll = Color::MakeARGB(255,235,235,235);
	pSkinGn3->clrAllFrameIn = Color::MakeARGB(255,111,111,111);
	pSkinGn3->clrAllFrameOut = Color::MakeARGB(255,0,0,0);
	pSkinGn3->clrBkNormal = Color::MakeARGB(255,235,235,235);
	pSkinGn3->clrBkOver = Color::MakeARGB(255,42,43,45);
	pSkinGn3->clrBkSel = Color::MakeARGB(255,62,63,65);
	pSkinGn3->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn3->clrTextNomal = Color::MakeARGB(255,0,0,0);
	pSkinGn3->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrTextSel = Color::MakeARGB(255,21,25,39);
	pSkinGn3->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn3->clrTextMark = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn3->clrFrameIn = Color::MakeARGB(255,22,22,22);
	pSkinGn3->clrFrameOut = Color::MakeARGB(255,255,255,255);
	CSkinTransform::Instance()->SetSkinAttribute(8,pSkinGn3);

	CSkinAttribute_List SkinLst;
    CSkinAttribute_List *pSkinLst = &SkinLst;//new CSkinAttribute_List;
	pSkinLst->clrTitleBkNormal = Color::MakeARGB(255,67,87,123);
	pSkinLst->clrTitleBkOver = Color::MakeARGB(255,235,235,235);
	pSkinLst->clrTitleBkSel = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleBkDisable = Color::MakeARGB(255,128,128,128);
	pSkinLst->clrTitleTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextOver = Color::MakeARGB(255,80,100,118);
	pSkinLst->clrTitleTextSel = Color::MakeARGB(255,80,100,118);
	pSkinLst->clrTitleTextDisable = Color::MakeARGB(255,80,100,118);
	pSkinLst->clrTitleSort = Color::MakeARGB(255,255,0,0);
	pSkinLst->clrTextMark = Color::MakeARGB(255,80,100,118/*220,0,220*/);
	pSkinLst->clrGapLine = Color::MakeARGB(255,200,200,200);

	pSkinLst->clrBkAll = Color::MakeARGB(255,235,235,235);
	pSkinLst->clrBkNormal = Color::MakeARGB(255,235,235,235);
	pSkinLst->clrBkOver = Color::MakeARGB(255,200,200,200);
	pSkinLst->clrBkSel = Color::MakeARGB(255,200,200,200);
	pSkinLst->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinLst->clrTextNomal = Color::MakeARGB(255,80,100,118);
	pSkinLst->clrTextOver = Color::MakeARGB(255,80,100,118);
	pSkinLst->clrTextSel = Color::MakeARGB(255,80,100,118);
	pSkinLst->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinLst->clrFrameIn = Color::MakeARGB(255,22,22,22);
	pSkinLst->clrFrameOut = Color::MakeARGB(255,0,0,0);
	pSkinLst->clrTotal = Color::MakeARGB(255,80,100,118);

	m_clrFill = pSkinLst->clrBkAll;
	CSkinTransform::Instance()->SetSkinAttribute(1,pSkinLst);

	CSkinAttribute_ScrollBar SkinScroll;
	CSkinAttribute_ScrollBar *pSkinScroll = &SkinScroll;//new CSkinAttribute_ScrollBar;
	pSkinScroll->clrBkAll = Color::MakeARGB(255,235,235,235);
	pSkinScroll->clrAllLineOut = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrAllLineIn = Color::MakeARGB(255,188,188,188);
	pSkinScroll->clrBoxBkNormal = Color::MakeARGB(255,200,200,200);
	pSkinScroll->clrBtnBkNormal = Color::MakeARGB(255,200,200,200);
	pSkinScroll->clrLineOut = Color::MakeARGB(255,188,188,188);
	pSkinScroll->clrLineIn = Color::MakeARGB(255,151,151,151);
	pSkinScroll->clrArrow = Color::MakeARGB(199,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(5,pSkinScroll);

	CSkinAttribute_FrameLine SkinFrameLine;
	CSkinAttribute_FrameLine *pSkinFrameLine = &SkinFrameLine;//new CSkinAttribute_FrameLine;
	pSkinFrameLine->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinFrameLine->clrBkLineIn = Color::MakeARGB(255,255,255,255);
	pSkinFrameLine->clrBkLineOut = Color::MakeARGB(255,66,66,66);
	CSkinTransform::Instance()->SetSkinAttribute(3,pSkinFrameLine);

	CSkinAttribute_Fund SkinFund;
	CSkinAttribute_Fund *pSkinFund = &SkinFund;//new CSkinAttribute_Fund;
	pSkinFund->clrBkAll = Color::MakeARGB(255,235,235,235);
	pSkinFund->clrBkNormal = Color::MakeARGB(255,235,235,235);
	pSkinFund->clrTextDown = Color::MakeARGB(255,20,255,20);
	pSkinFund->clrTextUp = Color::MakeARGB(255,255,20,20);
	pSkinFund->clrTextNormal = Color::MakeARGB(255,80,100,118);
	pSkinFund->clrTextMark = Color::MakeARGB(255,80,100,118);
	CSkinTransform::Instance()->SetSkinAttribute(4,pSkinFund);

	CSkinAttribute_Gap SkinGap;
	CSkinAttribute_Gap *pSkinGap = &SkinGap;//new CSkinAttribute_Gap;
	pSkinGap->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinGap->clrBk1 = Color::MakeARGB(120,18,36,57);
	pSkinGap->clrBk2 = Color::MakeARGB(120,48,66,97/*63,83,115*/);
	pSkinGap->clrBk3 = Color::MakeARGB(210,0,0,255);
	pSkinGap->clrBk4 = Color::MakeARGB(255,88,88,88);
	CSkinTransform::Instance()->SetSkinAttribute(2,pSkinGap);

	CSkinAttribute_General SkinGn1;
	CSkinAttribute_General *pSkinGn1 = &SkinGn1;//new CSkinAttribute_General;
	pSkinGn1->clrBkAll = Color::MakeARGB(255,72,0,1);
	pSkinGn1->clrAllFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn1->clrAllFrameOut = Color::MakeARGB(255,0,0,0);

	pSkinGn1->clrBkNormal = Color::MakeARGB(255,72,0,1);
	pSkinGn1->clrBkOver = Color::MakeARGB(255,246,87,87);
	pSkinGn1->clrBkSel = Color::MakeARGB(255,155,5,5);
	pSkinGn1->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn1->clrTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn1->clrTextMark = Color::MakeARGB(255,255,255,255);

	pSkinGn1->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn1->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn1->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(6,pSkinGn1);

	CSkinAttribute_General SkinGn2;
	CSkinAttribute_General *pSkinGn2 = &SkinGn2;//new CSkinAttribute_General;
	pSkinGn2->clrBkAll = Color::MakeARGB(255,235,235,235);
	pSkinGn2->clrAllFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn2->clrAllFrameOut = Color::MakeARGB(255,0,0,0);

	pSkinGn2->clrBkNormal = Color::MakeARGB(255,235,235,235);
	pSkinGn2->clrBkOver = Color::MakeARGB(255,200,200,200);
	pSkinGn2->clrBkSel = Color::MakeARGB(255,188,188,188);
	pSkinGn2->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn2->clrTextNomal = Color::MakeARGB(255,80,100,118);
	pSkinGn2->clrTextOver = Color::MakeARGB(255,80,100,255);
	pSkinGn2->clrTextSel = Color::MakeARGB(255,80,100,118);
	pSkinGn2->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn2->clrTextMark = Color::MakeARGB(255,255,255,255);

	pSkinGn2->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn2->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn2->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(7,pSkinGn2);

	CSkinAttribute_QuoteCard SkinQC;
	CSkinAttribute_QuoteCard *pSkinQC = &SkinQC;
	pSkinQC->clrBkAll = Color::MakeARGB(255,66,69,74);
	pSkinQC->clrAllFrameIn = Color::MakeARGB(255,111,111,111);
	pSkinQC->clrAllFrameOut = Color::MakeARGB(255,9,9,9);
	pSkinQC->clrText1 = Color::MakeARGB(255,255,255,64);
	pSkinQC->clrText2 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrPart2 = Color::MakeARGB(255,161,193,255);
	pSkinQC->clrText3 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrPart3 = Color::MakeARGB(255,255,129,129);
	pSkinQC->clrPartBordeNomal = Color::MakeARGB(255,148,148,148);
	pSkinQC->clrPartBordeOver = Color::MakeARGB(255,222,222,222);
	pSkinQC->clrFillItemNomal = Color::MakeARGB(255,66,69,74);
	pSkinQC->clrFillItemOver = Color::MakeARGB(255,89,91,94);
	pSkinQC->clrText6 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText7 = Color::MakeARGB(255,255,96,94);
	pSkinQC->clrText8 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText9 = Color::MakeARGB(255,255,96,94);
	CSkinTransform::Instance()->SetSkinAttribute(9,pSkinQC);
}
void CDateManage::InitSkinAttribute_White1()
{
	CSkinAttribute_General SkinGn3;
	CSkinAttribute_General *pSkinGn3 = &SkinGn3;//new CSkinAttribute_General;
	pSkinGn3->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrAllFrameIn = Color::MakeARGB(255,111,111,111);
	pSkinGn3->clrAllFrameOut = Color::MakeARGB(255,0,0,0);
	pSkinGn3->clrBkNormal = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrBkOver = Color::MakeARGB(255,8,53,82);
	pSkinGn3->clrBkSel = Color::MakeARGB(255,8,53,82);
	pSkinGn3->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn3->clrTextNomal = Color::MakeARGB(255,0,0,0);
	pSkinGn3->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn3->clrTextMark = Color::MakeARGB(255,255,255,255);
	pSkinGn3->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn3->clrFrameIn = Color::MakeARGB(255,22,22,22);
	pSkinGn3->clrFrameOut = Color::MakeARGB(255,255,255,255);
	CSkinTransform::Instance()->SetSkinAttribute(8,pSkinGn3);

	CSkinAttribute_List SkinLst;
	CSkinAttribute_List *pSkinLst = &SkinLst;//new CSkinAttribute_List;
	pSkinLst->clrTitleBkNormal = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTitleBkOver = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTitleBkSel = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTitleBkDisable = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTitleTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextOver = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextSel = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrTitleTextDisable = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTitleSort = Color::MakeARGB(255,255,0,0);
	pSkinLst->clrTextMark = Color::MakeARGB(255,8,53,82/*220,0,220*/);
	pSkinLst->clrGapLine = Color::MakeARGB(255,200,200,200);

	pSkinLst->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrBkNormal = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrBkOver = Color::MakeARGB(255,255,255,255);
	pSkinLst->clrBkSel = Color::MakeARGB(255,230,230,230);
	pSkinLst->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinLst->clrTextNomal = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTextOver = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTextSel = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinLst->clrFrameIn = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrFrameOut = Color::MakeARGB(255,8,53,82);
	pSkinLst->clrTotal = Color::MakeARGB(255,8,53,82);

	m_clrFill = pSkinLst->clrBkAll;
	CSkinTransform::Instance()->SetSkinAttribute(1,pSkinLst);

	CSkinAttribute_ScrollBar SkinScroll;
	CSkinAttribute_ScrollBar *pSkinScroll = &SkinScroll;//new CSkinAttribute_ScrollBar;
	pSkinScroll->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrAllLineOut = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrAllLineIn = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrBoxBkNormal = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrBtnBkNormal = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrLineOut = Color::MakeARGB(255,255,255,255);
	pSkinScroll->clrLineIn = Color::MakeARGB(255,8,53,82);
	pSkinScroll->clrArrow = Color::MakeARGB(199,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(5,pSkinScroll);

	CSkinAttribute_FrameLine SkinFrameLine;
	CSkinAttribute_FrameLine *pSkinFrameLine = &SkinFrameLine;//new CSkinAttribute_FrameLine;
	pSkinFrameLine->clrBkAll = Color::MakeARGB(255,8,53,82);
	pSkinFrameLine->clrBkLineIn = Color::MakeARGB(255,8,53,82);
	pSkinFrameLine->clrBkLineOut = Color::MakeARGB(255,8,53,82);
	CSkinTransform::Instance()->SetSkinAttribute(3,pSkinFrameLine);

	CSkinAttribute_Fund SkinFund;
	CSkinAttribute_Fund *pSkinFund = &SkinFund;//new CSkinAttribute_Fund;
	pSkinFund->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinFund->clrBkNormal = Color::MakeARGB(255,255,255,255);
	pSkinFund->clrTextDown = Color::MakeARGB(255,20,255,20);
	pSkinFund->clrTextUp = Color::MakeARGB(255,255,20,20);
	pSkinFund->clrTextNormal = Color::MakeARGB(255,8,53,82);
	pSkinFund->clrTextMark = Color::MakeARGB(255,255,255,255);
	CSkinTransform::Instance()->SetSkinAttribute(4,pSkinFund);

	CSkinAttribute_Gap SkinGap;
	CSkinAttribute_Gap *pSkinGap = &SkinGap;//new CSkinAttribute_Gap;
	pSkinGap->clrBkAll = Color::MakeARGB(255,0,33,62);
	pSkinGap->clrBk1 = Color::MakeARGB(120,255,255,255);
	pSkinGap->clrBk2 = Color::MakeARGB(120,255,255,255/*63,83,115*/);
	pSkinGap->clrBk3 = Color::MakeARGB(210,8,53,82);
	pSkinGap->clrBk4 = Color::MakeARGB(255,8,53,82);
	CSkinTransform::Instance()->SetSkinAttribute(2,pSkinGap);

	CSkinAttribute_General SkinGn1;
	CSkinAttribute_General *pSkinGn1 = &SkinGn1;//new CSkinAttribute_General;
	pSkinGn1->clrBkAll = Color::MakeARGB(255,72,0,1);
	pSkinGn1->clrAllFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn1->clrAllFrameOut = Color::MakeARGB(255,0,0,0);

	pSkinGn1->clrBkNormal = Color::MakeARGB(255,72,0,1);
	pSkinGn1->clrBkOver = Color::MakeARGB(255,246,87,87);
	pSkinGn1->clrBkSel = Color::MakeARGB(255,155,5,5);
	pSkinGn1->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn1->clrTextNomal = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextOver = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinGn1->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn1->clrTextMark = Color::MakeARGB(255,255,255,255);

	pSkinGn1->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn1->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn1->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(6,pSkinGn1);

	CSkinAttribute_General SkinGn2;
	CSkinAttribute_General *pSkinGn2 = &SkinGn2;//new CSkinAttribute_General;
	pSkinGn2->clrBkAll = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrAllFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn2->clrAllFrameOut = Color::MakeARGB(255,0,0,0);

	pSkinGn2->clrBkNormal = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrBkOver = Color::MakeARGB(255,150,150,150);
	pSkinGn2->clrBkSel = Color::MakeARGB(255,8,53,82);
	pSkinGn2->clrBkDisable = Color::MakeARGB(255,125,125,125);
	pSkinGn2->clrTextNomal = Color::MakeARGB(255,8,53,82);
	pSkinGn2->clrTextOver = Color::MakeARGB(255,200,200,200);
	pSkinGn2->clrTextSel = Color::MakeARGB(255,255,255,255);
	pSkinGn2->clrTextDisable = Color::MakeARGB(255,150,150,150);
	pSkinGn2->clrTextMark = Color::MakeARGB(255,255,255,255);

	pSkinGn2->clrGapLine = Color::MakeARGB(255,31,34,41);
	pSkinGn2->clrFrameIn = Color::MakeARGB(255,100,100,100);
	pSkinGn2->clrFrameOut = Color::MakeARGB(255,0,0,0);
	CSkinTransform::Instance()->SetSkinAttribute(7,pSkinGn2);

	CSkinAttribute_QuoteCard SkinQC;
	CSkinAttribute_QuoteCard *pSkinQC = &SkinQC;
	pSkinQC->clrBkAll = Color::MakeARGB(255,8,53,82);
	pSkinQC->clrAllFrameIn = Color::MakeARGB(255,45,96,125);
	pSkinQC->clrAllFrameOut = Color::MakeARGB(255,9,9,9);
	pSkinQC->clrText1 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText2 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrPart2 = Color::MakeARGB(255,79,191,205);
	pSkinQC->clrText3 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrPart3 = Color::MakeARGB(255,230,31,24);
	pSkinQC->clrPartBordeNomal = Color::MakeARGB(255,148,148,148);
	pSkinQC->clrPartBordeOver = Color::MakeARGB(255,222,222,222);
	pSkinQC->clrFillItemNomal = Color::MakeARGB(255,8,53,82);
	pSkinQC->clrFillItemOver = Color::MakeARGB(255,89,91,94);
	pSkinQC->clrText6 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText7 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText8 = Color::MakeARGB(255,255,255,255);
	pSkinQC->clrText9 = Color::MakeARGB(255,255,255,255);
	CSkinTransform::Instance()->SetSkinAttribute(9,pSkinQC);
};
void CDateManage::InitSkinAttribute()
{
	if(CPublic::Instance()->GetSkinType() == 1) InitSkinAttribute_White();
	else if(CPublic::Instance()->GetSkinType() == 2) InitSkinAttribute_White1();
	else InitSkinAttribute_Black();
	CPublic::Instance()->GetTradeDll()->m_pSetSkinAttribute(CSkinTransform::Instance());
}
CSkinAttribute * CDateManage::GetSkinAttribute(UINT iId)
{
	return CSkinTransform::Instance()->GetSkinAttribute(iId);
}

void CDateManage::GetSkinInfo(std::vector<SKININFO> &vecSkin)
{
	vecSkin.clear();
	m_lockMapSkin.Lock();
	do 
	{
		std::vector<SKININFO> vecSkin7;
		int iNum = m_mapSkin.size();
		if (iNum<=0)break;
		std::map<int,SKININFO>::iterator itMap;
		for (itMap = m_mapSkin.begin();itMap != m_mapSkin.end();itMap++)
		{
			vecSkin.push_back(itMap->second);
		}
	} while (0);
	m_lockMapSkin.Unlock();
}

const long * CDateManage::GetStockInfo(const void * pCode, UINT iMode)
{
	if (iMode == 1)
	{
		return (long *)CPublic::Instance()->GetStockInfo((SCodeInfo*)pCode);
	}
	if (iMode == 2)
	{
		int nStockNum;
		//vector <STUSymbolDataZYQ> 
		nStockNum = m_vecSymbol.size();
		if( nStockNum == 0)return NULL;

		for(int nCount =0;nCount <nStockNum ;nCount++)
		{
			if(0 == strcmp(m_vecSymbol[nCount].m_oSymbolInfo.m_szSymbol,((STUSymbolInfo*)pCode)->m_szSymbol))
			{
				return (long *)&m_vecSymbol[nCount];
			}
		}
		return NULL;	
	}
}
void CDateManage::ChangeStockEvent(const void* pCodeInfo,UINT uiInPage,UINT iMode)
{
	if (iMode == 1)
	{
		CPublic::Instance()->ChangeStockEvent((SCodeInfo*)pCodeInfo,uiInPage);
	}
	else if (iMode == 2)
	{
		if(NULL ==  pCodeInfo)
		{
			MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10005),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
			return;
		}

		memcpy(&m_oCurrentCode,pCodeInfo,sizeof(SCodeInfo));
		const CWnd* pView = CPublic::Instance()->GetView(10/*VIEWINFO_STOCK*/);
		//CDlgTrendChart* pView = (CDlgTrendChart*)pActivateWnd;
		if(NULL == pView)return;
		//SCodeInfo* pCode= pView->GetCurStock();
		//INT	uiPage = pView->GetAnalisysMode();
		//if((*pCode == *pCodeInfo)&&(uiPage == uiInPage))
		//{//代码相同
		//	return ;
		//}
		//STUSymbolInfo* pSendCode = (());
		STUSymbolInfo*  pSendCode = &((STUSymbolDataZYQ *)(GetStockInfo((void *)pCodeInfo,2)))->m_oSymbolInfo;

		if(pView)
			pView->SendMessage(WM_STOCKCHANGE,(WPARAM)pSendCode,uiInPage);
	}
}
void CDateManage::ReArrayVector(vector<int> &vecRA)
{
	vector<int> vecTemp;
	vector<int>::iterator itVec;
	if (vecRA.size() <= 0)return;
	vecTemp.insert(vecTemp.end(),vecRA.begin(),vecRA.end());
	int iMin = 100;
	int iTempIndex = 0;
	int iValue = 0;
	int iNum = vecTemp.size();
	for (int i = 0; i < iNum;i++)
	{
		int j = 0;
		iMin = 100;
		for (itVec = vecTemp.begin();itVec != vecTemp.end();itVec++)
		{
			iValue = *itVec;
			if (iValue < iMin &&iValue != -1)
			{
				iMin = iValue;
				iTempIndex = j;
			}
			j++;
		}
		vecTemp[iTempIndex] = -1;
		vecRA[iTempIndex] = i;
	}
}
void CDateManage::NotifyWndReLayout()
{
	///m_pPublic->NotifyWnd(NULL,0,2,11);
	CWnd * pWnd = m_pPublic->GetView(11,2);
	if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->PostMessage(WM_RELAYOUT);
	}
}
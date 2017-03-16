#include "StdAfx.h"
#include "DateManage_ZYQ.h"
#include "DlgTrendChart_D.h"
#include "inputkey.h"
#include "input.h"

CDateManage_ZYQ* CDateManage_ZYQ::_instance = NULL;
CDateManage_ZYQ::CDateManage_ZYQ(void)
{
	Init();
}


CDateManage_ZYQ::~CDateManage_ZYQ(void)
{
	//if (m_pstQuoteLstDate)
	//{
	//	delete m_pstQuoteLstDate;
	//	m_pstQuoteLstDate = NULL;
	//}
}
void CDateManage_ZYQ::ExitInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

CDateManage_ZYQ* CDateManage_ZYQ::Instance()
{
	if(!_instance)
	{
		_instance = new CDateManage_ZYQ;
	}
	return _instance;
}

void CDateManage_ZYQ::Init()
{
	m_pPublic = CPublic::Instance();

	//m_pstQuoteLstDate = new QuoteLstDate;
}
int  CDateManage_ZYQ::GetSymbolNum()
{
	TmapSymbolClassInfo::iterator itMap;
	int iNum = 0;
	itMap = m_stInitMarketDate.mapSymbolClass.begin();
	for (;itMap != m_stInitMarketDate.mapSymbolClass.end();itMap++)
	{
		iNum += (itMap->second.mapInitSymbol.size());
	}
	return iNum;
}
void CDateManage_ZYQ::AnalysisMsg(const char* lpData,int nDataLen)
{
	if (nDataLen <=0)return;

	SNormaHead* pDeataHead = (SNormaHead*)lpData; 

	STUAllTypeResp_ZYQ_Client stDateZYQ;
	stDateZYQ.m_usRespType = 0;
	CRespStructTrans::TransRespStruct(lpData,nDataLen,stDateZYQ);

	switch(stDateZYQ.m_usRespType)
	{
	case INITMARKET_ZYQ:
		{
			TranslateInitMarketDate_ZYQ(m_stInitMarketDate,stDateZYQ.m_oInitMarket);
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransInitMarket_ZYQ(stDateZYQ.m_oInitMarket.m_vMarketInfo);
			}
		}
		break;
	case REPORTDATA_ZYQ:
		{
			//TranslateReportDate_ZYQ(m_pstQuoteLstDate,stDateZYQ.m_oReportData);
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransReportDate_ZYQ(stDateZYQ.m_oReportData);
			}
		}
		break;
	case REPORTDATA_DETAIL_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransReportDetailDate_ZYQ(stDateZYQ.m_oReportDateDetail);
			}
		}
		break;
	case STKTRACEDATA_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransDealDetailDate_ZYQ(stDateZYQ.m_oTraceUnit);
			}
		}
		break;
	case STKMINUTEDATA_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransTimeTrendDate_ZYQ(stDateZYQ.m_oMinUnit);
			}	
		}
		break;
	case HISKDATA_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransKDate_ZYQ(stDateZYQ.m_oHisKUnit);
			}	
		}
		break;
	case PUSH_REALDATA_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransPushReportDate_ZYQ(stDateZYQ.m_oRealData);
			}	
		}
		break;
	case PUSH_REALDATA_DETAIL_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransPushReportDetailDate_ZYQ(stDateZYQ.m_oRealDataDetail);
			}	
		}
		break;
	case OPEN_MAKRKET_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransOpenMarket_ZYQ(stDateZYQ.m_oOpenMarket);
			}		
		}
		break;
	case CLOSE_MAKRKET_ZYQ:
		{
			CWnd *pWnd = m_pPublic->GetView(13,2);
			if (pWnd&&IsWindow(pWnd->GetSafeHwnd()))
			{
				((CDlgTrendChart_D*)pWnd)->TransCloseMarket_ZYQ(stDateZYQ.m_oCloseMarket);
			}		
		}
		break;
	default:
		break;
	}
}
BOOL CDateManage_ZYQ::TranslateInitMarketDate_ZYQ(InitMarket_ZYQ &stInitMarketDate,STUInitMarket_ZYQ_Client &stInitMarket_ZYQ_ClientT)
{
	int ivMarketInfoNum = stInitMarket_ZYQ_ClientT.m_vMarketInfo.size();
	int ivSymbolInfo = stInitMarket_ZYQ_ClientT.m_vSymbolInfo.size();
	int ivTradeTime = stInitMarket_ZYQ_ClientT.m_vTradeTime.size();

	if ( ivMarketInfoNum <= 0 || ivSymbolInfo <=0 || ivTradeTime <=0)return FALSE;
	SymbolClassInfo stSymbolClassInfo;

	stInitMarketDate.mapSymbolClass.clear();
	for (int i = 0; i < ivMarketInfoNum; i++)
	{
		stSymbolClassInfo.mapInitSymbol.clear();
		stSymbolClassInfo.usSymbolClassType = stInitMarket_ZYQ_ClientT.m_vMarketInfo[i].m_usMarketType;
		stSymbolClassInfo.strSymbolClassName = stInitMarket_ZYQ_ClientT.m_vMarketInfo[i].m_ssMarketName.c_str();
		stInitMarketDate.mapSymbolClass[stSymbolClassInfo.usSymbolClassType] = stSymbolClassInfo;
	}

	for (int i = 0; i < ivTradeTime; i++)
	{
		stInitMarketDate.stTradeTime.cTradeTimeCount = ivTradeTime;
		stInitMarketDate.stTradeTime.stTradeTimeSlot[i].inBeginTime = stInitMarket_ZYQ_ClientT.m_vTradeTime[i].m_nBeginTime;
		stInitMarketDate.stTradeTime.stTradeTimeSlot[i].cTheDay = stInitMarket_ZYQ_ClientT.m_vTradeTime[i].m_ucTheDay;
		stInitMarketDate.stTradeTime.stTradeTimeSlot[i].inEndTime = stInitMarket_ZYQ_ClientT.m_vTradeTime[i].m_nEndTime;
	}

	//STUSymbolInfo_ZYQ_Client stTUSymbolInfo_ZYQ_Client;
	SymbolTradeInfo_ZYQ stSymbolTradeInfo;
	TmapSymbolClassInfo::iterator itMap;
	int x = 0;
	int y = 0;
	int iIndexDc = 0x0001;
	for (int i = 0; i < ivSymbolInfo; i++)
	{
		//stTUSymbolInfo_ZYQ_Client = stTUInitMarket_ZYQ_ClientT.m_vSymbolInfo[i];
		itMap = stInitMarketDate.mapSymbolClass.find(stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_usMarketType);
		if (itMap != stInitMarketDate.mapSymbolClass.end())
		{
			stSymbolTradeInfo.stSymbolInfo.cMarket = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_usMarketType;
			stSymbolTradeInfo.stSymbolInfo.cPriceUnit = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_ucPriceUnit;
			stSymbolTradeInfo.stSymbolInfo.cTrade = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_ucTrade;
			stSymbolTradeInfo.stSymbolInfo.usTradeTimeIndex = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_usTradeTimeIndex;              // 交易节索引 (用位表示)
			stSymbolTradeInfo.stSymbolInfo.strSymbolName = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_ssSymbolName.c_str();
			stSymbolTradeInfo.stSymbolInfo.strSymbolCode = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_ssSymbolCode.c_str();
			stSymbolTradeInfo.stSymbolInfo.uiTotalShares = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_uiTotalShares;                   // 流通股数(总股本数)
			stSymbolTradeInfo.stSymbolInfo.iAgreeunit = 1;
			if (stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_iAgreeunit >= 10000.0)
			{
				stSymbolTradeInfo.stSymbolInfo.iAgreeunit = stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_iAgreeunit/10000;
			}

			y = 0;
			x = 0;
			iIndexDc = 0x0001;
			while (x < ivTradeTime)
			{
				iIndexDc = 0x0001<<x;
				if (stSymbolTradeInfo.stSymbolInfo.usTradeTimeIndex & iIndexDc)
				{
					stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[y] = stInitMarketDate.stTradeTime.stTradeTimeSlot[x];
					y++;
				}
				x++;
			}
			stSymbolTradeInfo.stTradeTime.cTradeTimeCount = y;
			itMap->second.mapInitSymbol[stSymbolTradeInfo.stSymbolInfo.strSymbolCode] = stSymbolTradeInfo;

			CInputKey::Instance()->AddStockCode_ZYQ(stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_ssSymbolCode.c_str(),
				stInitMarket_ZYQ_ClientT.m_vSymbolInfo[i].m_ssSymbolName.c_str());
		}
	}
	return TRUE;
}
BOOL CDateManage_ZYQ::GetSymbolTradeInfo_ZYQ(SymbolTradeInfo_ZYQ &stSymbolTradeInfo_ZYQ)
{
	TmapSymbolClassInfo::iterator itMap;
	if (stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket == 0)
	{
		itMap = m_stInitMarketDate.mapSymbolClass.begin();
		for (;itMap != m_stInitMarketDate.mapSymbolClass.end();itMap++)
		{
			TmapIntiSymbolInfo::iterator itMapSymbol;
			itMapSymbol = itMap->second.mapInitSymbol.find(stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode);
			if (itMapSymbol != itMap->second.mapInitSymbol.end())
			{
				stSymbolTradeInfo_ZYQ = itMapSymbol->second;
				stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket = 0;
				return TRUE;
			}
		}
		return FALSE;
	}

	itMap = m_stInitMarketDate.mapSymbolClass.find(stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket);
	if (itMap != m_stInitMarketDate.mapSymbolClass.end())
	{
		TmapIntiSymbolInfo::iterator itMapSymbol;
		itMapSymbol = itMap->second.mapInitSymbol.find(stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode);
		if (itMapSymbol != itMap->second.mapInitSymbol.end())
		{
			stSymbolTradeInfo_ZYQ = itMapSymbol->second;
			return TRUE;
		}
	}

	return FALSE;
}
void CDateManage_ZYQ::ReqReportDate_ZYQ(STUReportDataIndex_ZYQ * pstReportDataIndex_ZYQ)
{
	if(pstReportDataIndex_ZYQ == NULL)return;
	int iBufferSize;
	iBufferSize = sizeof(STUReportDataIndex_ZYQ)+sizeof(SNormaHead); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	{
		pRrequest->m_usType = REPORTDATA_ZYQ;
	}
	pRrequest->m_usIndex = CDateManage::Instance()->GetRequestIndex();
	pRrequest->m_usSize = 1;

	STUReportDataIndex_ZYQ *pReportDataIndex_ZYQ = (STUReportDataIndex_ZYQ *)(pRrequest+1);

	memcpy(pReportDataIndex_ZYQ,pstReportDataIndex_ZYQ,sizeof(STUReportDataIndex_ZYQ));
	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}
void CDateManage_ZYQ::ReqInitMarket_ZYQ()
{

}
void CDateManage_ZYQ::ReqReportDateDetail_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ)
{
	if(pstSimpleSymbol_ZYQ == NULL)return;
	int iBufferSize;
	iBufferSize = sizeof(STUSimpleSymbol_ZYQ)+sizeof(SNormaHead); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	{
		pRrequest->m_usType = REPORTDATA_DETAIL_ZYQ;
	}
	pRrequest->m_usIndex = CDateManage::Instance()->GetRequestIndex();
	pRrequest->m_usSize = 1;

	STUSimpleSymbol_ZYQ *pSimpleSymbol_ZYQ = (STUSimpleSymbol_ZYQ *)(pRrequest+1);
	memcpy(pSimpleSymbol_ZYQ,pstSimpleSymbol_ZYQ,sizeof(STUSimpleSymbol_ZYQ));

	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}
void CDateManage_ZYQ::ReqDealDetail_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ,UINT iReqNum)
{
	if(pstSimpleSymbol_ZYQ == NULL)return;
	int iBufferSize;
	iBufferSize = sizeof(STUSimpleSymbol_ZYQ)+sizeof(SNormaHead) + sizeof(STUCommLenDirec); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	{
		pRrequest->m_usType = STKTRACEDATA_ZYQ;
	}
	pRrequest->m_usIndex = CDateManage::Instance()->GetRequestIndex();
	pRrequest->m_usSize = 1;

	STUSimpleSymbol_ZYQ *pSimpleSymbol_ZYQ = (STUSimpleSymbol_ZYQ *)(pRrequest+1);
	memcpy(pSimpleSymbol_ZYQ,pstSimpleSymbol_ZYQ,sizeof(STUSimpleSymbol_ZYQ));

	STUCommLenDirec *pstCommLenDirec = (STUCommLenDirec *)(pSimpleSymbol_ZYQ+1);
	pstCommLenDirec->m_lenDirec = iReqNum;

	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}

void CDateManage_ZYQ::ReqTimeTrendDate_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ)
{
	if(pstSimpleSymbol_ZYQ == NULL)return;
	int iBufferSize;
	iBufferSize = sizeof(STUSimpleSymbol_ZYQ)+sizeof(SNormaHead); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	{
		pRrequest->m_usType = STKMINUTEDATA_ZYQ;
	}
	pRrequest->m_usIndex = CDateManage::Instance()->GetRequestIndex();
	pRrequest->m_usSize = 1;

	STUSimpleSymbol_ZYQ *pSimpleSymbol_ZYQ = (STUSimpleSymbol_ZYQ *)(pRrequest+1);
	memcpy(pSimpleSymbol_ZYQ,pstSimpleSymbol_ZYQ,sizeof(STUSimpleSymbol_ZYQ));

	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}
void CDateManage_ZYQ::ReqKDate_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ,STUHisKIndexDirec *pstHisKIndexDirec)
{
	if(pstSimpleSymbol_ZYQ == NULL)return;
	int iBufferSize;
	iBufferSize = sizeof(STUSimpleSymbol_ZYQ)+sizeof(SNormaHead)+sizeof(STUHisKIndexDirec);
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	{
		pRrequest->m_usType = HISKDATA_ZYQ;
	}
	pRrequest->m_usIndex = CDateManage::Instance()->GetRequestIndex();
	pRrequest->m_usSize = 1;

	STUSimpleSymbol_ZYQ *pSimpleSymbol_ZYQ = (STUSimpleSymbol_ZYQ *)(pRrequest+1);
	memcpy(pSimpleSymbol_ZYQ,pstSimpleSymbol_ZYQ,sizeof(STUSimpleSymbol_ZYQ));

	STUHisKIndexDirec *pHisKIndexDirec = (STUHisKIndexDirec *)(pSimpleSymbol_ZYQ+1);
	*pHisKIndexDirec = *pstHisKIndexDirec;

	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}
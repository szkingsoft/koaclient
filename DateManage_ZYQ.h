#pragma once
#include "PubZYQ.h"
#include "translateStruct.h"

////////////////
class CDateManage_ZYQ
{
public:
	CDateManage_ZYQ(void);
	~CDateManage_ZYQ(void);

	static void ExitInstance();
	static CDateManage_ZYQ* Instance();
	static CDateManage_ZYQ* _instance;

public:
	void Init();
	void AnalysisMsg(const char* lpData,int nDataLen);
public:
	void ReqReportDate_ZYQ(STUReportDataIndex_ZYQ * pstReportDataIndex_ZYQ);
	void ReqInitMarket_ZYQ();
	void ReqReportDateDetail_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ);
	void ReqDealDetail_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ,UINT iReqNum = 11);
	void ReqTimeTrendDate_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ);
	void ReqKDate_ZYQ(STUSimpleSymbol_ZYQ * pstSimpleSymbol_ZYQ,STUHisKIndexDirec *pstHisKIndexDirec);

	int  GetSymbolNum();
private:
	CPublic * m_pPublic;

	InitMarket_ZYQ m_stInitMarketDate;
	BOOL TranslateInitMarketDate_ZYQ(InitMarket_ZYQ &stInitMarketDate,STUInitMarket_ZYQ_Client &stInitMarket_ZYQ_ClientT);
public:
	BOOL GetSymbolTradeInfo_ZYQ(SymbolTradeInfo_ZYQ &stSymbolTradeInfo_ZYQ);
};


#include "stdafx.h"
#include "translateStruct.h"

#define NEXT_FIRED(POS, LEN, SKIP) do { POS += SKIP; LEN -= SKIP; } while (0)



bool CRespStructTrans::TransRespStruct(const char* from,  int len,STUAllTypeResp_ZYQ_Client& to)
{
    if(len < sizeof(SNormaHead)) return false;
    const char* pos = from;
    SNormaHead*  pRespHead = (SNormaHead*)pos;
    NEXT_FIRED(pos, len, sizeof(SNormaHead));

    switch(pRespHead->m_usType)
    {
    case OPEN_MAKRKET_ZYQ:
        {
            to.m_usRespType = OPEN_MAKRKET_ZYQ;
            if (len < pRespHead->m_usSize * sizeof(STUOpenQuoteInfo_ZYQ))
            {
                return false;
            }

            STUOpenQuoteInfo_ZYQ* pOpenMarket = (STUOpenQuoteInfo_ZYQ*)(pRespHead + 1);
            to.m_oOpenMarket.m_oSSymobl.ssSymbolCode = string(pOpenMarket->m_oSymbol.m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oOpenMarket.m_oSSymobl.ssSymbolCode.resize(strlen(to.m_oOpenMarket.m_oSSymobl.ssSymbolCode.c_str()));
            to.m_oOpenMarket.m_oSSymobl.m_ucPriceUnit = pOpenMarket->m_oSymbol.m_ucPriceUnit;
            to.m_oOpenMarket.m_oSSymobl.m_usMarketType = pOpenMarket->m_oSymbol.m_usMarketType;
            to.m_oOpenMarket.m_oTDateTime = pOpenMarket->m_oTDateTime;
            to.m_oOpenMarket.m_oOpenInfo.m_uiYesClosePrice = pOpenMarket->m_uiYesClosePrice;
            to.m_oOpenMarket.m_oOpenInfo.m_uiYesSettlPrice = pOpenMarket->m_uiYesSettlPrice;
            to.m_oOpenMarket.m_oOpenInfo.m_iTotalIssueQty = pOpenMarket->m_iTotalIssueQty;

            return true;
        }
        break;
    case CLOSE_MAKRKET_ZYQ:
        {
            to.m_usRespType = CLOSE_MAKRKET_ZYQ;

            if (len < pRespHead->m_usSize * sizeof(STUCloseQuoteInfo_ZYQ))
            {
                return false;
            }

            STUCloseQuoteInfo_ZYQ* pCloseMarket = (STUCloseQuoteInfo_ZYQ*)(pRespHead + 1);
            to.m_oCloseMarket.m_oSSymobl.ssSymbolCode = string(pCloseMarket->m_oSymbol.m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oCloseMarket.m_oSSymobl.ssSymbolCode.resize(strlen(to.m_oCloseMarket.m_oSSymobl.ssSymbolCode.c_str()));
            to.m_oCloseMarket.m_oSSymobl.m_ucPriceUnit = pCloseMarket->m_oSymbol.m_ucPriceUnit;
            to.m_oCloseMarket.m_oSSymobl.m_usMarketType = pCloseMarket->m_oSymbol.m_usMarketType;
            to.m_oCloseMarket.m_oTDateTime = pCloseMarket->m_oTDateTime;


            to.m_oCloseMarket.m_iNextTradeDay     = pCloseMarket->m_iNextTick;   
            to.m_oCloseMarket.m_oCloseInfo.m_uiYesClosePrice     = pCloseMarket->m_uiYesClosePrice;         // 昨收价
            to.m_oCloseMarket.m_oCloseInfo.m_uiYesSettlPrice     = pCloseMarket->m_uiYesSettlPrice;         // 昨结价
            to.m_oCloseMarket.m_oCloseInfo.m_uiOpenPrice     = pCloseMarket->m_uiOpenPrice;             // 开盘价
            to.m_oCloseMarket.m_oCloseInfo.m_uiMaxPrice     = pCloseMarket->m_uiMaxPrice;              // 最高价
            to.m_oCloseMarket.m_oCloseInfo.m_uiMinPrice     = pCloseMarket->m_uiMinPrice;              // 最低价
            to.m_oCloseMarket.m_oCloseInfo.m_uiClosePrice     = pCloseMarket->m_uiClosePrice;            // 今天收盘价
            to.m_oCloseMarket.m_oCloseInfo.m_iTotalTrunoverQty     = pCloseMarket->m_iTotalTrunoverQty;             // 总成交量
            to.m_oCloseMarket.m_oCloseInfo.m_iTotalTrunoverMoney     = pCloseMarket->m_iTotalTrunoverMoney;           // 总成交额
            to.m_oCloseMarket.m_oCloseInfo.m_iTotalIssueQty     = pCloseMarket->m_iTotalIssueQty;               // 总发行量

            return true;
        }
        break;
    case INITMARKET_ZYQ:
        {
            // 请求:SNormaHead
            // 回应:SNormaHead + STUInitMarketLenDirec_ZYQ + STUMarketInfo_ZYQ[N] + STUSymbolTradeTime_ZYQ[N] + STUSymbolInfo_ZYQ[N];
            to.m_usRespType = INITMARKET_ZYQ;
            STUInitMarketLenDirec_ZYQ* pInitMarketLenDirec = (STUInitMarketLenDirec_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUInitMarketLenDirec_ZYQ));
            if (len < 0) return false;

            if(len < 
                (pInitMarketLenDirec->m_usMarkets*sizeof(STUMarketInfo_ZYQ) 
                + pInitMarketLenDirec->m_usTradeTimeNum*sizeof(STUSymbolTradeTime_ZYQ)
                + pInitMarketLenDirec->m_usSymboNum*sizeof(STUSymbolInfo_ZYQ))
                )
            {
                return false;
            }

            STUMarketInfo_ZYQ* pMarketInfo = (STUMarketInfo_ZYQ*)pos;

            for(int i = 0 ;i < pInitMarketLenDirec->m_usMarkets; i++)
            {
                STUMarketInfo_ZYQ_Client marketTmp;
                marketTmp.m_ssMarketName = string(pMarketInfo[i].m_szMarketName,MARKET_NAME_ZYQ_LEN);
                marketTmp.m_ssMarketName.resize(strlen(marketTmp.m_ssMarketName.c_str()));
                marketTmp.m_usMarketType = pMarketInfo[i].m_usMarketType;
                to.m_oInitMarket.m_vMarketInfo.push_back(marketTmp);
                NEXT_FIRED(pos, len, sizeof(STUMarketInfo_ZYQ));
            }

            STUSymbolTradeTime_ZYQ* pTradeTime = (STUSymbolTradeTime_ZYQ*)(pos);

            for(int i = 0 ;i < pInitMarketLenDirec->m_usTradeTimeNum; i++)
            {
                to.m_oInitMarket.m_vTradeTime.push_back(pTradeTime[i]);
                NEXT_FIRED(pos, len, sizeof(STUSymbolTradeTime_ZYQ));
            }

            STUSymbolInfo_ZYQ* pSymbolInfo = (STUSymbolInfo_ZYQ*)(pos);

            for(int i = 0; i < pInitMarketLenDirec->m_usSymboNum; i++)
            {
                STUSymbolInfo_ZYQ_Client symTmp;
                symTmp.m_ssSymbolCode = string(pSymbolInfo[i].m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
                symTmp.m_ssSymbolCode.resize(strlen(symTmp.m_ssSymbolCode.c_str()));
                symTmp.m_ssSymbolName = string(pSymbolInfo[i].m_szSymbolName,SYMBOL_NAME_ZYQ_LEN);
                symTmp.m_ssSymbolName.resize(strlen(symTmp.m_ssSymbolName.c_str()));
                symTmp.m_ucPriceUnit = pSymbolInfo[i].m_ucPriceUnit;
                symTmp.m_ucTrade = pSymbolInfo[i].m_ucTrade;
                symTmp.m_uiTotalShares = pSymbolInfo[i].m_uiTotalShares;
                symTmp.m_usMarketType = pSymbolInfo[i].m_usMarketType;
                symTmp.m_iAgreeunit = pSymbolInfo[i].m_iAgreeunit;
                symTmp.m_usTradeTimeIndex = pSymbolInfo[i].m_usTradeTimeIndex;
                to.m_oInitMarket.m_vSymbolInfo.push_back(symTmp);
                NEXT_FIRED(pos, len, sizeof(STUSymbolInfo_ZYQ));
            }
            return true;
        }
        break;
    case REPORTDATA_ZYQ:
        {
            // 请求: SNormaHead + STUReportDataIndex_ZYQ + STUSimpleSymbol_ZYQ[N]
            // 回应: SNormaHead + STUReportDataIndex_ZYQ + STUCommLenDirec + STUReportDataResp_ZYQ[N]
            to.m_usRespType = REPORTDATA_ZYQ;
            STUReportDataIndex_ZYQ* pReportDataIndex = (STUReportDataIndex_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUReportDataIndex_ZYQ));
            if (len < 0) return false;

            to.m_oReportData.m_oRepIndex = *pReportDataIndex;

            STUCommLenDirec* pLenDirec = (STUCommLenDirec*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUCommLenDirec));

            if (len < 0 || len < pLenDirec->m_lenDirec * sizeof(STUReportDataResp_ZYQ))
            {
                // 如果 len < 0 , 那么pLenDirec->m_lenDirec 非法访问内存
                return false;
            }


            STUReportDataResp_ZYQ* pReportData = (STUReportDataResp_ZYQ*)(pos);

            for(int i = 0; i < pLenDirec->m_lenDirec; i++)
            {
                STUReportDataResp_ZYQ_Client_Vec reporttmp;
                reporttmp.m_oQtyPriceV1 = pReportData[i].m_oQtyPriceV1;
                reporttmp.m_oReportPublicData = pReportData[i].m_oReportPublicData;
                reporttmp.m_oTDateTime = pReportData[i].m_oTDateTime;
                reporttmp.m_oSSymobl.m_usMarketType = pReportData[i].m_oSSymobl.m_usMarketType;
                reporttmp.m_oSSymobl.m_ucPriceUnit = pReportData[i].m_oSSymobl.m_ucPriceUnit;
                reporttmp.m_oSSymobl.ssSymbolCode = string(pReportData[i].m_oSSymobl.m_szSymbolCode,CLIENT_SYMBOL_MAX_LEN);
                reporttmp.m_oSSymobl.ssSymbolCode.resize(strlen(reporttmp.m_oSSymobl.ssSymbolCode.c_str()));
                to.m_oReportData.m_vReportData.push_back(reporttmp);
                NEXT_FIRED(pos, len, sizeof(STUReportDataResp_ZYQ));
            }
            return true;
        }
        break;
    case REPORTDATA_DETAIL_ZYQ:
        {
            // 请求: SNormaHead + STUSimpleSymbol_ZYQ
            // 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUReportDataDetailResp_ZYQ + STUNowDataVolPirce[N]

            STUSimpleSymbol_ZYQ* pSSymbol = (STUSimpleSymbol_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUSimpleSymbol_ZYQ));

            if(len < 0) return false;

            to.m_usRespType = REPORTDATA_DETAIL_ZYQ;
            STUReportDataDetailResp_ZYQ* pReportData = (STUReportDataDetailResp_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUReportDataDetailResp_ZYQ));

            if(len < 0 || len < sizeof(STUNowDataVolPirce)*pReportData->m_usVolNum) 
            {
                return false;
            }

            to.m_oReportDateDetail.m_oSSymobl.m_ucPriceUnit = pSSymbol->m_ucPriceUnit;
            to.m_oReportDateDetail.m_oSSymobl.m_usMarketType = pSSymbol->m_usMarketType;
            to.m_oReportDateDetail.m_oSSymobl.ssSymbolCode = string(pSSymbol->m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oReportDateDetail.m_oSSymobl.ssSymbolCode.resize(strlen(to.m_oReportDateDetail.m_oSSymobl.ssSymbolCode.c_str()));

            to.m_oReportDateDetail.m_oExt = pReportData->m_oExt;
            to.m_oReportDateDetail.m_oReportPublicData = pReportData->m_oReportPublicData;
            to.m_oReportDateDetail.m_oTDateTime = pReportData->m_oTDateTime;

            STUNowDataVolPirce* pVol = (STUNowDataVolPirce*)pos;
            for(int i = 0; i < pReportData->m_usVolNum; i++)
            {
                to.m_oReportDateDetail.m_vQtyPriceV.push_back(pVol[i]);
                NEXT_FIRED(pos, len, sizeof(STUNowDataVolPirce));
            }
            return true;
        }
        break;
    case HISKDATA_ZYQ:
        {
            // 请求: SNormaHead + STUSimpleSymbol_ZYQ + STUHisKIndexDirec
            // 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUHisKUnit_ZYQ  

            to.m_usRespType = HISKDATA_ZYQ;

            STUSimpleSymbol_ZYQ* pSymbol = (STUSimpleSymbol_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUSimpleSymbol_ZYQ));
            if (len < 0)
            {
                return false;
            }

            
            to.m_oHisKUnit.m_oSymbol.m_ucPriceUnit = pSymbol->m_ucPriceUnit;
            to.m_oHisKUnit.m_oSymbol.m_usMarketType = pSymbol->m_usMarketType;
            to.m_oHisKUnit.m_oSymbol.ssSymbolCode = string(pSymbol->m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oHisKUnit.m_oSymbol.ssSymbolCode.resize(strlen(to.m_oHisKUnit.m_oSymbol.ssSymbolCode.c_str()));

            STUCommLenDirec* pLenDirec = (STUCommLenDirec*)pos;
            NEXT_FIRED(pos, len, sizeof(STUCommLenDirec));
            if (len < 0 || len < pLenDirec->m_lenDirec * sizeof(STUHisKUnit_ZYQ))
            {
                return false;
            }

            STUHisKUnit_ZYQ* pKisUnit = (STUHisKUnit_ZYQ*)pos;
            for(int i = 0; i < pLenDirec->m_lenDirec; i++)
            {
                to.m_oHisKUnit.m_vHisKUnit.push_back(pKisUnit[i]);
                NEXT_FIRED(pos, len, sizeof(STUHisKUnit_ZYQ));
            }
            return true;
        }
        break;
    case STKMINUTEDATA_ZYQ:
        {
            // 请求: SNormaHead + STUSimpleSymbol_ZYQ
            // 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUSTKMinUnit_ZYQ  


            to.m_usRespType = STKMINUTEDATA_ZYQ;

            STUSimpleSymbol_ZYQ* pSymbol = (STUSimpleSymbol_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUSimpleSymbol_ZYQ));
            if (len < 0) return false;

            to.m_oMinUnit.m_oSymbol.m_usMarketType = pSymbol->m_usMarketType;
            to.m_oMinUnit.m_oSymbol.m_ucPriceUnit = pSymbol->m_ucPriceUnit;
            to.m_oMinUnit.m_oSymbol.ssSymbolCode = string(pSymbol->m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oMinUnit.m_oSymbol.ssSymbolCode.resize(strlen(to.m_oMinUnit.m_oSymbol.ssSymbolCode.c_str()));

            STUCommLenDirec* pLenDirec = (STUCommLenDirec*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUCommLenDirec));
            if (len < 0 || len < pLenDirec->m_lenDirec * sizeof(STUSTKMinUnit_ZYQ))
            {
                return false;
            }

            STUSTKMinUnit_ZYQ* pStkMinUnit = (STUSTKMinUnit_ZYQ*)pos;
            for(int i = 0; i < pLenDirec->m_lenDirec; i++)
            {
                to.m_oMinUnit.m_vMinUnit.push_back(pStkMinUnit[i]);
                NEXT_FIRED(pos, len, sizeof(STUSTKMinUnit_ZYQ));
            }
            return true;
        }
        break;
    case STKTRACEDATA_ZYQ:
        {
            // 请求: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec
            // 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUTraceUnit_ZYQ  

            to.m_usRespType = STKTRACEDATA_ZYQ;

            STUSimpleSymbol_ZYQ* pSymbol = (STUSimpleSymbol_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUSimpleSymbol_ZYQ));
            if (len < 0) return false;

            to.m_oTraceUnit.m_oSymbol.m_ucPriceUnit = pSymbol->m_ucPriceUnit;
            to.m_oTraceUnit.m_oSymbol.m_usMarketType = pSymbol->m_usMarketType;
            to.m_oTraceUnit.m_oSymbol.ssSymbolCode = string(pSymbol->m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oTraceUnit.m_oSymbol.ssSymbolCode.resize(strlen(to.m_oTraceUnit.m_oSymbol.ssSymbolCode.c_str()));

            STUCommLenDirec* pLenDirc = (STUCommLenDirec*)pos;
            NEXT_FIRED(pos, len, sizeof(STUCommLenDirec));
            if (len < 0 || len < pLenDirc->m_lenDirec * sizeof(STUTraceUnit_ZYQ))
            {
                return false;
            }

            STUTraceUnit_ZYQ* pTraceUnit = (STUTraceUnit_ZYQ*)pos;
            

            for(int i = 0; i < pLenDirc->m_lenDirec; i++)
            {
                to.m_oTraceUnit.m_vTraceUnit.push_back(pTraceUnit[i]);
                NEXT_FIRED(pos, len, sizeof(STUTraceUnit_ZYQ));
            }
            return true;
        }
        break;
    case PUSH_REALDATA_ZYQ:
        {
            // PUSH_REALDATA_ZYQ       =0x0ba0;
            // SNormaHead + STUSimpleSymbol_ZYQ + STURealDataPush_ZYQ(单个)

            to.m_usRespType = PUSH_REALDATA_ZYQ;

            STUSimpleSymbol_ZYQ* pSymbol = (STUSimpleSymbol_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUSimpleSymbol_ZYQ));
            if (len < 0) return false;

            to.m_oRealData.m_oSSymobl.m_ucPriceUnit = pSymbol->m_ucPriceUnit;
            to.m_oRealData.m_oSSymobl.m_usMarketType = pSymbol->m_usMarketType;
            to.m_oRealData.m_oSSymobl.ssSymbolCode = string(pSymbol->m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oRealData.m_oSSymobl.ssSymbolCode.resize(strlen(to.m_oRealData.m_oSSymobl.ssSymbolCode.c_str()));

            STURealDataPush_ZYQ* pRealData = (STURealDataPush_ZYQ*)pos;
            NEXT_FIRED(pos, len, sizeof(STURealDataPush_ZYQ));
            if (len < 0) return false;

            to.m_oRealData.m_oQtyPriceV1 = pRealData->m_oQtyPriceV1;
            to.m_oRealData.m_oRealPublicData = pRealData->m_oRealPublicData;
            to.m_oRealData.m_oTDateTime = pRealData->m_oTDateTime;
            return true;
        }
        break;
    case  PUSH_REALDATA_DETAIL_ZYQ:
        {
            // PUSH_REALDATA_DETAIL_ZYQ =0x0ba1;
            // SNormaHead + STUSimpleSymbol_ZYQ + STURealDataDetailPush_ZYQ + STUNowDataVolPirce[N]

            to.m_usRespType = PUSH_REALDATA_DETAIL_ZYQ;

            STUSimpleSymbol_ZYQ* pSymbol = (STUSimpleSymbol_ZYQ*)(pos);
            NEXT_FIRED(pos, len, sizeof(STUSimpleSymbol_ZYQ));
            if (len < 0) return false;

            to.m_oRealDataDetail.m_oSSymobl.m_ucPriceUnit = pSymbol->m_ucPriceUnit;
            to.m_oRealDataDetail.m_oSSymobl.m_usMarketType = pSymbol->m_usMarketType;
            to.m_oRealDataDetail.m_oSSymobl.ssSymbolCode = string(pSymbol->m_szSymbolCode,SYMBOL_CODE_ZYQ_LEN);
            to.m_oRealDataDetail.m_oSSymobl.ssSymbolCode.resize(strlen(to.m_oRealDataDetail.m_oSSymobl.ssSymbolCode.c_str()));


            STURealDataDetailPush_ZYQ* pRealData = (STURealDataDetailPush_ZYQ*)pos;
            NEXT_FIRED(pos, len, sizeof(STURealDataDetailPush_ZYQ));

            if (len < 0 || len < pRealData->m_usVolNum * sizeof(STUNowDataVolPirce))
            {
                return false;
            }

            to.m_oRealDataDetail.m_oExt = pRealData->m_oExt;
            to.m_oRealDataDetail.m_oRealPublicData = pRealData->m_oRealPublicData;
            to.m_oRealDataDetail.m_oTDateTime = pRealData->m_oTDateTime;

            STUNowDataVolPirce* pVol = (STUNowDataVolPirce*) pos;
            for(int i = 0;i < pRealData->m_usVolNum; i++)
            {
                to.m_oRealDataDetail.m_vQtyPriceV.push_back(pVol[i]);
            }
            return true;
        }
        break;
    default:
        return false;
    }
    return false;
}
